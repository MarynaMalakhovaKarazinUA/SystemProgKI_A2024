#!/bin/bash

# Specify the file name
file_name="new_file.txt"

# Create the file and write something to it
echo "Hello, this is a new file!" > "$file_name"

# Confirm file creation
echo "File '$file_name' has been created in the current directory."
