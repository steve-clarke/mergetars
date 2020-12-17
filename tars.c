#include "mergetars.h"

//  accepts a tarfile and expands its contents to /tmp directory
int expand_tarfile(char *tarfile)
{
    ITEMS  *itm    = add_item(tarfile);

    char *args[] = {
        "tar",
        "-C",
        itm->tmpdir,
        "-xpf",
        itm->tarfile,
        (char *)NULL
    };

    int status  = fork_exec(TAR, args);

    return status;
}

int compress_files(char *inputfiles, char *outputfile)
{

    char *args[] = {
        "tar",
        "-C",
        inputfiles,
        "-cpf",
        outputfile,
        ".",
        (char *)NULL
    };

    int status  = fork_exec(TAR, args);

    return status;
}

