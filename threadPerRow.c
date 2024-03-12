#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#define MAX_SIZE 10

int matrix1[MAX_SIZE][MAX_SIZE];
int matrix2[MAX_SIZE][MAX_SIZE];
int result[MAX_SIZE][MAX_SIZE];
int r1, c1, r2, c2;

void readMatrixFromFile(const char *filename, int matrix[][MAX_SIZE], int *r, int *c) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        printf("Error opening file.\n");
        return;
    }

    char buffer[1024];
    int bytesRead = read(fd, buffer, sizeof(buffer));
    if (bytesRead == -1) {
        printf("Error reading from file.\n");
        close(fd);
        return;
    }
   int temp_buff[400]; 
   int i=0;
   char *token = strtok(buffer," \n");
   while(token != NULL)
   {
      
      temp_buff[i]= atoi(token);
      token = strtok(NULL," \n");
      i++;
   }
   *r = temp_buff[0];
   *c = temp_buff[1];
   int index=2;
   for(int i=0;i< *r;i++)
   {
      for(int j=0;j< *c;j++)
      {
         matrix[i][j] = temp_buff[index];
         index++;
      }
      
   }


    close(fd);
}
void *multiplyRow(void *arg) {
    int row = *(int *)arg;
    for (int i = 0; i < c2; ++i) {
        result[row][i] = 0;
        for (int j = 0; j < c1; ++j) {
            result[row][i] += matrix1[row][j] * matrix2[j][i];
        }
    }
    pthread_exit(NULL);
}

void getMatrixElements(int matrix[][MAX_SIZE], int row, int column) {
    printf("\nEnter elements: \n");
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < column; ++j) {
            printf("Enter a%d%d: ", i + 1, j + 1);
            scanf("%d", &matrix[i][j]);
        }
    }
}

void display(int matrix[][MAX_SIZE], int row, int column) {
    printf("\nOutput Matrix:\n");
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < column; ++j) {
            printf("%d  ", matrix[i][j]);
            if (j == column - 1)
                printf("\n");
        }
    }
}

int main(int argc, char *argv[]) {
    pthread_t threads[MAX_SIZE];
    int row_args[MAX_SIZE];

   char *File[2];
   int fileNum=0;
   if(argc > 1)
   {

         File[0]=argv[1];
         File[1]=argv[2];
         printf("%s\n",File[0]);
         printf("%s\n",File[1]);
         readMatrixFromFile(File[0],matrix1,&r1,&c1);

         readMatrixFromFile(File[1],matrix2,&r2,&c2);
   }
   else{
      readMatrixFromFile("a.txt",matrix1,&r1,&c1);

      readMatrixFromFile("b.txt",matrix2,&r2,&c2);
   }

    if (c1 != r2) {
        printf("Error: Invalid matrix dimensions for multiplication.\n");
        return 1;
    }

 
    struct timeval start,stop;
    gettimeofday(&start,NULL);
    for (int i = 0; i < r1; ++i) {
        row_args[i] = i;
        pthread_create(&threads[i], NULL, multiplyRow, (void *)&row_args[i]);
    }

    for (int i = 0; i < r1; ++i) {
        pthread_join(threads[i], NULL);
    }

    display(result, r1, c2);

    gettimeofday(&stop,NULL);
    printf("seconds token %lu\n",stop.tv_sec - start.tv_sec);
    printf("Microseconds token %lu\n",stop.tv_usec - start.tv_usec); 
    return 0;
}
