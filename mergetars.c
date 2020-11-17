
#include "mergetars.h"

//  Written by Jamie McCullough and Steve Clarke

// Format Expected:     ./mergetars input_tarfile1 [, input_tarfile2...], output_tarfile
int main(int argc, char *argv[])
{
    // Checks that the inputs given are of a valid count (greater than 3)
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s input_tarfile1 [input_tarfile2 ... ] output_tarfile\n",
               argv[0]);
        exit(EXIT_FAILURE);
    }
    
    input_file_count = argc - 2;                        // Number of input files to merge               
    print_input_data(argv, input_file_count);           // Prints out input data in user-readable fashion
    output_archive_name = strdup(argv[argc - 1]);       // Name of final output archive

    // Creates the tmp directories to hold the (soon to be) expanded archives
    printf("%s: Creating temporary folder to-be-archived files...\n", argv[0]);
    archives[MAX_TARS - 1].tmp_folder = make_temp_dir();

    // After checking archive extensions, calls expand_tar() for each input archive to respective temps.
    printf("\n%s: Expanding all archives to temporary archives...\n", argv[0]);
    expand_all(argv, input_file_count);

    // Prints Archive data structure array ('archives')
    printf("%s: Now printing data structure...\n", argv[0]);
    print_data_structure();

    // Searches for unique files, placing each new instance found into an array.
    // Records any duplicate files that are found, and their paths.
    printf("\n%s: Analysing data structure for unique files...\n", argv[0]);
    scan_archives();

    // Prints Item data structure array ('items')
    printf("\n%s: Printing items array...\n", argv[0]);
    print_items_array();

    // Determines which files are the most appropriate files to use in the final output archive.
    printf("%s: Resolving duplicates files between input archives...\n", argv[0]);
    sort_files();

    // Debug for output array 
    printf("\n%s: Printing output array...\n", argv[0]);
    print_outputs();

    // Creates output tmp directory, then populates tmp with most appropriate files and directory structure.
    printf("\n%s: Populating output temporary directory with appropriate files...\n", argv[0]);
    init_output_directories();
    populate_output();

    // Archives the tmp output folder after population with inputs
    printf("\n%s: Archiving output directory...\n", argv[0]);
    archive_output(archives[MAX_TARS - 1].tmp_folder);

    // Removes temporary folders made in the process of the program running.
    printf("\n%s: Output archived. Cleaning temporary directories...\n", argv[0]);
    clean_temps();

    printf("\n%s: Complete, new archive created \"%s\".\n", argv[0], argv[argc - 1]);
    exit(EXIT_SUCCESS);
}
