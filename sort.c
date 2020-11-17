/*
    -----sort.c-----
    Functions here determine which files from the input archives are best
    fit for the final output archive.
*/

#include "mergetars.h"

// Determines which Instance was the latest input by the user.
extern int latest_tar(struct Instance x, int x_index, struct Instance y, int y_index)
{
    char x_tmp[TEMP_PATH_LEN + 1];
    strncpy(x_tmp, x.full_path, TEMP_PATH_LEN - 1);
    x_tmp[TEMP_PATH_LEN] = '\0';

    char y_tmp[TEMP_PATH_LEN + 1];
    strncpy(y_tmp, y.full_path, TEMP_PATH_LEN - 1);
    y_tmp[TEMP_PATH_LEN] = '\0';

    for (int j = 0; j < MAX_TARS; j++)
    {
        if (archives[j].tmp_folder != NULL)
        {
            if (strcmp(archives[j].tmp_folder, x_tmp) == 0)
            {
                return x_index;
            }
            else if (strcmp(archives[j].tmp_folder, y_tmp) == 0)
            {
                return y_index;
            }
        }
    }
    fprintf(stderr, "\tsort_files(): Error, cannot determine latest tar. Exiting.\n");
    exit(EXIT_FAILURE);
}

// Determines which Instance index value contains the largest file
extern int biggest_file(struct Instance x, int x_index, struct Instance y, int y_index)
{
    if (x.size > y.size)
    {
        return x_index;
    }
    else if (x.size < y.size)
    {
        return y_index;
    }
    else  // Where files are of same size and modification time, pass to lastest_tar()
    {
        return latest_tar(x, x_index, y, y_index);
    }
}

// Determines which Instance index value contains the most recent file
extern int newest_file(struct Instance x, int x_index, struct Instance y, int y_index)
{
    if (difftime(x.mod_time, y.mod_time) > 0)
    {
        return x_index;
    }
    else if (difftime(x.mod_time, y.mod_time) < 0)
    {
        return y_index;
    }
    else // Where files are of same modification time, pass to biggest_file()
    {
        return biggest_file(x, x_index, y, y_index);
    }
}

// Returns index of instance with most accurate file
extern void sort_files(void)
{
    printf("\tsort_files(): Detemining most appropriate files for output.\n");
    for (int i = 0; i < MAX_FILES; i++)
    {
        int best_file_index = 0;

        for (int j = 0; j < items[i].num_instances - 1; j++)
        {
            best_file_index = (newest_file(items[i].instances[best_file_index],best_file_index, items[i].instances[j + 1], j + 1));
        }

        archives[MAX_TARS - 1].files[i].pathname = items[i].instances[best_file_index].full_path;

        if (archives[MAX_TARS - 1].files[i].pathname != NULL)
        {
            archives[MAX_TARS - 1].num_files++;
        }
    }
    printf("\t\tDone\n");
}
