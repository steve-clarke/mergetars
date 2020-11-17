/*
    -----print.c-----
    Functions here print data structures, and information relevant to 
    the user's understanding of the program's execution.
*/

#include "mergetars.h"

extern void print_input_data(char *argv[], int input_count)
{
    printf(".:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.\n");
    printf("  MERGETARS - Steve Clarke, Jamie McCullough (2020)\n");
    printf(".:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.:.\n");

    printf("\n%s: Beginning merge of archives:", argv[0]);
    for (int i = 1; i < input_count + 1; i++)
    {
        if (i < input_count)
        {
            printf(" '%s',", argv[i]);
        }
        else
        {
            printf(" and '%s' into '%s'...\n\n", argv[i], argv[i + 1]);
        }
    }
}

// Prints archives and their files within
extern void print_data_structure(void)
{
    for (int i = 0; i < input_file_count; i++)
    {
        printf("\t________________________________________\n");
        printf("\t|Temp Folder: '%s'\n", archives[i].tmp_folder);
        for (int k = 0; k < MAX_FILES; k++)
        {
            if (archives[i].files[k].pathname != NULL)
            {
                printf("\t|\t File: '%s'\n", archives[i].files[k].pathname);
            }
        }
        printf("\t>> Contains %d files.\n", archives[i].num_files);
    }
}

// Prints the values contained in the tmp folder that contains the files to archive
extern void print_outputs(void)
{
    printf("\t________________________________________\n");
    printf("\t|OUTPUT FOLDER: '%s'\n", archives[MAX_TARS - 1].tmp_folder);
    for (int k = 0; k < MAX_FILES; k++)
    {
        if (archives[MAX_TARS - 1].files[k].pathname != NULL)
        {
            printf("\t|\t File: '%s'\n", archives[MAX_TARS - 1].files[k].pathname);
        }
    }
    printf("\t>> Contains %d files.\n", archives[MAX_TARS - 1].num_files);
}

// Prints the array of unique files and their duplicate locations
extern void print_items_array(void)
{
    for (int i = 0; i < MAX_FILES; i++)
    {
        if (items[i].file_name != NULL)
        {
            printf("\tITEM [%d]  => '%s'\n", i, items[i].file_name);
            for (int j = 0; j < MAX_FILES; j++)
            {
                if (items[i].instances[j].full_path != NULL)
                {
                    printf("\t  \\-->FILE [%d] => %s mod: %s ", j, items[i].instances[j].full_path, ctime(&items[i].instances[j].mod_time));
                }
            }
            printf("\tNum Instances = %d\n\n", items[i].num_instances);
        }
    }
}
