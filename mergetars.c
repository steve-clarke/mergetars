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

    merge();

    populate_output(argv[argc-1]);

    cleanup(EXIT_SUCCESS);

}
