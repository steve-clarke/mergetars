#include "mergetars.h"

int main(int argc, char *argv[])
{
    // Check number of arguments
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s input_archive1.tar [input_archive2.tar ... ] output_file.tar\n",
               argv[0]);
        exit(EXIT_FAILURE);
    }

    // Expand each .tar file into a new temp directory
    for(int i = 1; i < argc-1 ; i++)
    {
        if(expand_tarfile(argv[i]) != 0)
        {
            cleanup(EXIT_FAILURE);
        }
    }

    // populate struct with appropriate files and directory structure
    merge();

    // Make output file based on information from merge() call
    populate_output(argv[argc-1]);

    // Perform cleanup of all tmp directories and frees memory
    cleanup(EXIT_SUCCESS);
}
