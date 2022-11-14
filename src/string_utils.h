#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef STRING_UTILS
#define STRING_UTILS

char* read_string(int fd) 
{
    int len = 0;
    int capacity = 1; 
    char c;
    char *string = NULL;

    ssize_t bytes_read = read(fd, &c, sizeof(char));
    if (bytes_read == 0) 
    {
        return NULL;
    }
    if (bytes_read > 0)
    {
        string = (char*) malloc(sizeof(char));
    } 
        
    while (1) 
    {
        if (c == '\n' || c == EOF) 
        {
            string[len] == '\0';
            return string;
        }
        string[len++] = c;
        if (len  >= capacity) 
        {
            capacity *= 2;
            string = (char*) realloc(string, capacity * sizeof(char));
        }
        read(fd, &c, sizeof(char));
    }
    return string;
}

#endif