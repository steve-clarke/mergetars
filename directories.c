#include "mergetars.h"

char **directories = NULL;
int ndirectories = 0;

void add_directory(char *path)
{
    directories = realloc(directories, (ndirectories+1) * sizeof(char *));
    directories[ndirectories] = strdup(path);
    ndirectories++;
}

void init_output_directories(char *outputtmp)
{
    for(int i = 0; i < ndirectories; i++)
    {
        char newdir[MAXPATHLEN];
        sprintf(newdir, "%s/%s", outputtmp, directories[i]);

        struct stat path_status;
        if (stat(newdir, &path_status) == -1)
        {                      // If the path doesn't exist...
            mkdir(newdir, 0700); // ...make it.
        }
    }
}
