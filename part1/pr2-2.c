#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{   char filename[100];
    int fd;
    struct stat file_stat;
    char buffer[1024];
    ssize_t bytesRead;


    printf("Enter the filename to read: ");
    scanf("%99s", filename);

    if (stat(filename, &file_stat) == -1)
    {   perror("Error checking file existence");
        return EXIT_FAILURE;
    }

    if (!S_ISREG(file_stat.st_mode))
    {   fprintf(stderr, "Error: The specified path is not a regular file.\n");
        return EXIT_FAILURE;
    }

    fd = open(filename, O_RDONLY);
    if (fd == -1)
    {   perror("Error opening file");
        return EXIT_FAILURE;
    }

    printf("\nContents of the file \"%s\":\n\n", filename);

    while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0)
    {   write(STDOUT_FILENO, buffer, bytesRead);
    }

    if (bytesRead == -1)
    {   perror("Error reading file");
        close(fd);
        return EXIT_FAILURE;
    }

    close(fd);
    printf("\n\nFile reading completed.\n");

    return EXIT_SUCCESS;
}
