#include <stdio.h>

int main() {
    // Define the name of the file to be created
    const char *file_name = "new_file.txt";

    // Attempt to open the file in write mode
    FILE *file = fopen(file_name, "w");
    
    // Check if the file was successfully created
    if (file == NULL) {
        perror("Error creating file"); // Print error message using perror
        return 1; // Return a non-zero value to indicate failure
    }

    // Write a message to the newly created file
    fprintf(file, "Hello, this is a new file!\n");

    // Close the file to ensure data is written properly
    fclose(file);

    // Notify the user of successful file creation
    printf("File '%s' has been successfully created in the current directory.\n", file_name);
    
    return 0; // Return zero to indicate success
}
