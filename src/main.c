#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/stat.h>

// linux limits
#define FILENAME_LIMIT 255
#define FILEPATH_LIMIT 4096

int main(void)
{
    char path_to_file[FILEPATH_LIMIT];

    printf("Enter path to file: ");
    scanf("%s", path_to_file);

    int source_fd = open(path_to_file, O_RDONLY);
    if (source_fd == -1)
    {
        perror("File open error\n");
        return 1;
    }

    struct stat st;
    if (fstat(source_fd, &st) == -1)
    {
        perror("couldn't get status\n");
        return 2;
    }
 
    char *mapped_file = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, source_fd, 0);
    float *shared_memory = mmap(NULL, sizeof(float), PROT_READ | PROT_WRITE, 
                                                     MAP_SHARED | MAP_ANONYMOUS, 
                                                     -1, 0);
    if (mapped_file == NULL)
    {
        perror("file map error\n");
        return 3;
    }
    if (shared_memory == NULL)
    {
        perror("memory map error\n");
        return 4;
    }
    *shared_memory = 0; // total sum

    pid_t pid = fork();
    if (pid == -1)
    {
        perror("fork error\n");
        return 5;
    }
    if (pid == 0) // child
    {
        int i = 0;
        char c;
        int capacity = 1;
        int len = 0;
        char *string;

        while (i < st.st_size)
        {
            c = mapped_file[i];

            if (c == ' ' || c == '\n' || c == EOF)
            {
                *shared_memory += atof(string);
                len = 0;
                free(string);
            }
            else
            {
                if (len == 0)
                {
                    string = malloc(sizeof(char) * capacity);
                }
                if (len >= capacity)
                {
                    capacity *= 2;
                    string = (char*)realloc(string, capacity * sizeof(char));
                }
                string[len++] = c;
            }
            i++;
        }
    }
    else
    {
        waitpid(pid, NULL, 0);
        printf("%f\n", *shared_memory);
        munmap(shared_memory, sizeof(float));
        munmap(mapped_file, st.st_size);
    }
    return 0;
}