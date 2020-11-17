/*
    -----populators.c-----
    Functions here fill the data structures with required information for 
    the processing of input files, and the final output temporary directory
    structure before it is archived.
*/

#include "mergetars.h"

// Fills output temp directory with appropriate files
extern void populate_output(void)
{
    for (int i = 0; i < archives[MAX_TARS - 1].num_files; i++)
    {
        // source_path is path of the file we're copying over
        char *source_path = strdup(archives[MAX_TARS - 1].files[i].pathname);

        // dest_path is the path to the final output /tar/ folder
        char *bare_path = archives[MAX_TARS - 1].files[i].pathname;
        bare_path += TEMP_PATH_LEN - 1;

        char *dest_path = malloc(MAXPATHLEN); 
        strcpy(dest_path, archives[MAX_TARS - 1].tmp_folder);
        strcat(dest_path, bare_path);

        printf("\tpopulate_output(): File '%s' created.\n", dest_path);

        int source_descriptor, dest_descriptor, error;
        unsigned char buffer[BUF_SIZE];

        struct stat st;
        stat(source_path, &st);

        // in preparation for utimes() call
        struct timeval times[2];
        times[0].tv_sec = st.st_mtime;
        times[0].tv_usec = 0;
        times[1].tv_sec = st.st_mtime;
        times[1].tv_usec = 0;

        source_descriptor = open(source_path, O_RDONLY);
        dest_descriptor = open(dest_path, O_CREAT | O_WRONLY, 0600);

        chmod(dest_path, st.st_mode);

        //Possibly change to perror()?
        while (1)
        {
            error = read(source_descriptor, buffer, BUF_SIZE);
            if (error == -1)
            {
                printf("\tError reading file.\n");
                exit(EXIT_FAILURE);
            }

            if (error == 0)
                break;

            error = write(dest_descriptor, buffer, error);
            if (error == -1)
            {
                printf("\tError writing to file.\n");
                exit(EXIT_FAILURE);
            }
        }
        close(source_descriptor);
        close(dest_descriptor);
        utimes(dest_path, times);
    }
}

//---------------------- TMP & STRUCT POPULATION -----------------------------

// Recursively scans directories to populate Archive struct array
extern void find_files(char *dir_name, int tar_index)
{
    char path[MAXPATHLEN + 1];
    struct dirent *dp;
    DIR *dir = opendir(dir_name);

    if (dir == NULL)
    {
        perror(dir_name);
        exit(EXIT_FAILURE);
    }

    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            strcpy(path, dir_name);
            strcat(path, "/");
            strcat(path, dp->d_name);

            struct stat path_status;
            stat(path, &path_status);

            if (S_ISREG(path_status.st_mode)) // Is a file, store in struct
            {
                archives[tar_index].files[file_counter].pathname = strdup(path);
                archives[tar_index].num_files++;
                file_counter++;
            }

            else if (S_ISDIR(path_status.st_mode)) // If directory, store path and enter
            {
                archives[tar_index].directories[dir_counter].dir_name = strdup(path);
                archives[tar_index].directories[dir_counter].dir_name += TEMP_PATH_LEN - 1;
                archives[tar_index].num_dirs++;
                dir_counter++;
                find_files(path, tar_index);
            }

            // all other filetypes are ignored
        }
    }
    closedir(dir);
}


// Populates Item struct array, and relevant Instance substruct arrays
extern void add_instances(char *file_name)
{
    struct stat attr;
    stat(file_name, &attr);

    char *bare_path = file_name;
    bare_path += TEMP_PATH_LEN;

    int new_instance_index = 0;

    // For every file in items array
    for (int i = 0; i < MAX_FILES; i++)
    {
        // If the item_type i's file name is not null..
        if (items[i].file_name != NULL)
        {
            // ...if the array's name is the same as
            if (strcmp(bare_path, items[i].file_name) == 0)
            {
                new_instance_index = items[i].num_instances;

                items[i].instances[new_instance_index].full_path = file_name;
                items[i].instances[new_instance_index].mod_time = attr.st_mtime;
                items[i].instances[new_instance_index].size = attr.st_size;

                items[i].num_instances++;
                break;
            }
        }
        else if (items[i].file_name == NULL)
        {
            if (items[i].num_instances > 0)
            {
                exit(EXIT_FAILURE);
            }

            items[i].file_name = bare_path;

            items[i].instances[0].full_path = file_name;
            items[i].instances[0].mod_time = attr.st_mtime;
            items[i].instances[0].size = attr.st_size;

            items[i].num_instances++;

            break;
        }
    }
}

// Passes Archive struct File data to add_instances()
extern void scan_archives(void)
{
    for (int i = 0; i < MAX_TARS; i++)
    {
        for (int k = 0; k < MAX_FILES; k++)
        {
            if (archives[i].files[k].pathname != NULL)
            {
                add_instances(archives[i].files[k].pathname);
            }
        }
    }
}
