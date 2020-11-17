/*
    -----dir_utilities.c-----
    Functions here process secondary-storage related tasks, related to the 
    interfacing between the program and the OS file system.
*/

#include "mergetars.h"

// Creates a directory on file system in /tmp/
extern char *make_temp_dir(void)
{
    char *output_directory;
    output_directory = strdup(TEMPLATE);
    mkdtemp(output_directory);
    printf("\t\tmake_temp_dir(): Temporary folder created at \"%s\"\n", output_directory);

    return output_directory;
}

// Archives output temp file using tar command
extern void archive_output(char *directory)
{
    printf("\tarchive_output(): Creating archive file '%s'.\n", output_archive_name);

    pid_t child = fork();

    if (child == -1)
    {
        exit(EXIT_FAILURE);
    }
    else if (child > 0)
    {
        int status;
        waitpid(child, &status, 0);
    }
    else
    {
        // Calls tar command with output tar name
        execl(TAR, TAR, "-cf", output_archive_name, "-C", directory, ".", (char *)NULL);
        perror("\tarchive_output() Error");
        exit(EXIT_FAILURE);
    }

    printf("\tarchive_output(): Archive successfully created!\n");
}

// Create temp directory and fill it with tar file's contents.
extern char *expand_tar(char *filename)
{
    // Create variable for temp directory, feed it TEMPLATE, give it to mkdtemp for allocation.
    char *temp_directory = make_temp_dir();

    printf("\t\t\texpand_tar(): Unpacking into \"%s\".\n", temp_directory);

    pid_t child = fork();   

    if (child == -1)
    {
        exit(EXIT_FAILURE); 
    }
    else if (child > 0)
    {
        int status;
        waitpid(child, &status, 0);
    }
    else
    { // fork() returns 0 when this process is the child. extract the tar; child killed after execution completes.
        execl(TAR, TAR, "-xf", filename, "-C", temp_directory, (char *)NULL);
        // All execl calls must be terminated by null pointer according to manpage.
        // -C flag sends contents to a given directory

        perror("\texpand_tar( ) Error"); // Child execution stops with execl. If execution makes it here, something failed.
    }

    printf("\t\t\texpand_tar(): Expansion to \"%s\" completed.\n\n", temp_directory);
    return temp_directory;
}

// Given an item type, performs appropriate removal function
extern int delete_item(const char *path, const struct stat *path_stat, int item_type, struct FTW *file_tree)
{
    switch (item_type)
    {                 // Gets the path's type
    case FTW_D:       // If a dir that has not been walked; skip it
    case FTW_DP:      // If a dir that has been walked...
        rmdir(path);  // ... remove the dir.
        break;
    default:          // If none of the above...
        unlink(path); // ...'deletes' the given file.
        break;
    }
    return 0;
}

// Calls the delete_item() function on all dir items and dir itself
extern void clean_temps(void)
{
    for (int i = 0; i < MAX_TARS; i++) // For every item in the archives array...
    {
        char *current_archive = archives[i].tmp_folder; // ...get the tmp folder path
        // If the path is not NULL...
        if (current_archive != NULL)
        {
            printf("\tclean_temps(): Removing directory '%s'...\n", current_archive);

            //...traverse the archive 'MAX_FILES' times, calling delete_item()
            if (nftw(current_archive, delete_item, MAX_FILES, FTW_DEPTH) == 0)
            {
                printf("\t\tDone.\n\n");
            }
            else
            {
                perror("nftw() Error");
                exit(EXIT_FAILURE);
            }
        }
    }
}

// Make the directory structure in the output temp folder
extern void init_output_directories(void)
{
    for (int i = 0; i < input_file_count; i++)
    {
        for (int f = 0; f < archives[i].num_dirs; f++)
        {
            char path[MAXPATHLEN + 1];
            strcpy(path, archives[MAX_TARS - 1].tmp_folder);
            strcat(path, "/");
            strcat(path, archives[i].directories[f].dir_name);

            struct stat path_status;
            if (stat(path, &path_status) == -1)
            {                      // If the path doesn't exist...
                mkdir(path, 0700); // ...make it.
            }
        }
    }
}

// Expands all archives and extracts to tmp folders
extern void expand_all(char *argv[], int input_file_count)
{
    for (int i = 0; i < input_file_count; i++)
    {
        if (check_extension(argv[i + 1]) == 0)
        {
            printf("\nInvalid archive extension found. Supported files: .tar, .targz, .tgz, .tar.gz\n");
            exit(EXIT_FAILURE);
        }

        // expand_tar() returns tmp folder, and then it is set to i's tmp_folder
        printf("\texpand_all(): Expanding archive '%s'...\n", argv[i + 1]);
        archives[i].tmp_folder = expand_tar(argv[i + 1]);

        // find_files() populates the files data struct array
        printf("\texpand_all(): Populating data structure with information from %s...\n", argv[i + 1]);
        find_files(archives[i].tmp_folder, i);
        printf("\t\tDone.\n\n");

        file_counter = 0;
        dir_counter = 0;
    }
}

// Checks that extension of filename is suitable for tar command input
extern int check_extension(char *filename)
{
    char *point = filename;

    if ((point = strchr(filename, '.')) != NULL)
    {
        if (!(strcmp(point, ".tar")) || !(strcmp(point, ".tar.gz")) || !(strcmp(point, ".targz")) || !(strcmp(point, ".tgz")))
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}
