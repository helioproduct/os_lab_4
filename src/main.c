#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>

#include "string_utils.h"

const int FILENAME_LIMIT = 255;

int main(void)
{
    printf("Enter path to file: ");
    char path_to_file[FILENAME_LIMIT];
    scanf("%s", path_to_file);

    int source_fd = open(path_to_file, O_RDONLY);
    if (source_fd == -1)
    {
        perror("File open error\n");
        return 3;
    }

    int protection = PROT_READ | PROT_WRITE;
    int flags = MAP_SHARED | MAP_ANONYMOUS;
    
    float *shared = mmap(NULL, sizeof(float), protection, flags, -1, 0);
    *shared = 0;

    int id = fork();
    if (id == -1)
    {
        perror("fork error\n");
        return 2;
    }
    if (id == 0)
    {
        float number = 0;
        char *string;
        while ( (string = read_string(source_fd)) != NULL )
        {
            char *token;
            while ( (token = strsep(&string, " ")) != NULL )
            {
                number = atof(token);
                shared[0] += number;
            }
        }
    }
    else
    {
        wait(NULL);
        printf("%f\n", shared[0]);
        munmap(shared, sizeof(float));
    }
    return 0;
}