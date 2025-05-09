#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{   FILE *file;
    char buffer[16]; // Small buffer of 16 bytes
    const char *largeData = "This is a very large string that will definitely overflow the buffer.";

    // Open a file for writing
    file = fopen("test_overflow.txt", "w");
    if (file == NULL)
    {   perror("Failed to open file");
        return EXIT_FAILURE;
    }

    // Simulating buffer overflow
    printf("Buffer size: %zu\n", sizeof(buffer));
    printf("Attempting to write data of size %zu into the buffer.\n", strlen(largeData));

    strcpy(buffer, largeData); // This will cause buffer overflow
    fwrite(buffer, sizeof(char), strlen(buffer), file);

    printf("Data written to file.\n");

    fclose(file);
    return EXIT_SUCCESS;
}
