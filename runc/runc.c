#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
void print(const char *text)
{
    char start[] = "\033[0;34m";
    char end[] = "\033[0m";
    int all_length = strlen(start) + strlen(end) + strlen(text) + 1;
    char *all_text = (char *)malloc(all_length * sizeof(char));
    strcat(all_text, start);
    strcat(all_text, text);
    strcat(all_text, end);
    printf("%s\n", all_text);
}

void compile(char *current_working_directory, char *tmp_path, char *file)
{
    char input_path[8192];
    // Format input path
    snprintf(input_path, 8192, "%s/%s", current_working_directory, file);

    char output_path[8192];
    // Remove the file extension
    // Make a copy
    char filename_copy[4096];
    strcpy(filename_copy, file);
    char *dot_ptr = strrchr(filename_copy, '.');

    if (dot_ptr != NULL && strcmp(dot_ptr, ".c") == 0) {
        *dot_ptr = '\0';
    } else {
        printf("File %s does not have a .c extension.\n", input_path);
        exit(0);
    }

    // Format the output path
    snprintf(output_path, 8192, "%s/%s", tmp_path, filename_copy);

    if (access(input_path, F_OK) == -1) {
        printf("File %s does not exist\n", input_path);
        exit(0);
    }

    int command_length = 8192;
    char *command = (char *)malloc(command_length * sizeof(char));
    snprintf(command, command_length, "gcc -Wall -o \"%s\".o \"%s\"", output_path, input_path);

    int status = system(command);
    // if (status == 0) {
    //     print("Compiled Successfully");
    // } else {
    //     print("Failed to compile");
    // }
}
void run(char *tmp_path, char *file)
{
    // Remove the file extension
    // Make a copy
    char filename_copy[4096];
    strcpy(filename_copy, file);
    char *dot_ptr = strrchr(filename_copy, '.');

    if (dot_ptr != NULL && strcmp(dot_ptr, ".c") == 0) {
        *dot_ptr = '\0';
    } else {
        printf("File does not have a .c extension.\n");
        exit(0);
    }

    // Format the output path
    char output_path[8192];
    snprintf(output_path, 8192, "\"%s/%s\".o", tmp_path, filename_copy);

    printf("--------------------------------------------------\n");
    system(output_path);
    printf("\n--------------------------------------------------\n\n");
}

int main(int argc, char *argv[])
{
    // Check if arguments are passed
    if (argc < 2)
    {
        printf("Usage: %s <C source code file>\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "init") == 0)
    {
        FILE *file_pointer;
        char content[128] = "#include <stdio.h>\n"
                            "#include <stdlib.h>\n"
                            "int main(int argc, char *argv[])\n"
                            "{\n"
                            "\tprintf(\"Hello World\");\n"
                            "return 0;\n"
                            "}\n";
        file_pointer = fopen("hello_world.c", "w");
        // Write data to the file
        fprintf(file_pointer, "%s", content);
        fclose(file_pointer);
        printf("Successfully create a basic script\n");
        exit(0);
    }

    // Print compiling
    // printf("Compiling: %s\n", argv[1]);

    // Get working directory name
    char current_working_directory[1024];
    getcwd(current_working_directory, sizeof(current_working_directory));
    // printf("Current Working Directory: %s\n", current_working_directory);

    // Make a temp directory
    char tmp_path[8192];
    sprintf(tmp_path, "%s/compiled", current_working_directory);
    mkdir(tmp_path, 0777);

    // Get the file to compile
    for (int i = 1; i < argc; i++)
    {   
        printf("Compiling: %-20s\n", argv[i]);
        compile(current_working_directory, tmp_path, argv[i]);
        printf("Running: %-20s\n", argv[i]);
        run(tmp_path, argv[i]);
    }

    return 0;
}
