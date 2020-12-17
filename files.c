#include "mergetars.h"

FILES *files = NULL;
int nfiles = 0;

void populate_output(char *outputarchive)
{
    add_item(outputarchive);
    char *outputdir = items[nitems-1].tmpdir;

    init_output_directories(outputdir);

    for (int i = 0; i < nfiles; i++)
    {
        char sourcepath[MAXPATHLEN];
        sprintf(sourcepath, "%s/%s", items[files[i].itemn].tmpdir, files[i].filename);

        char destpath[MAXPATHLEN];
        sprintf(destpath, "%s/%s", outputdir, files[i].filename);

        int source_descriptor, dest_descriptor, error;
        unsigned char buffer[BUFSIZ];

        struct stat st;
        stat(sourcepath, &st);

        // preserve original modtime
        struct timeval times[2];
        times[0].tv_sec = st.st_mtime;
        times[0].tv_usec = 0;
        times[1].tv_sec = st.st_mtime;
        times[1].tv_usec = 0;

        source_descriptor = open(sourcepath, O_RDONLY);
        dest_descriptor = open(destpath, O_CREAT | O_WRONLY, 0600);

        // preserve original permissions
        chmod(destpath, st.st_mode);

        while (1)
        {
            error = read(source_descriptor, buffer, BUFSIZ);
            if (error == -1)
            {
                printf("\tError reading file.\n");
                cleanup(EXIT_FAILURE);
            }

            if (error == 0)
                break;

            error = write(dest_descriptor, buffer, error);
            if (error == -1)
            {
                printf("\tError writing to file.\n");
                cleanup(EXIT_FAILURE);
            }
        }
        close(source_descriptor);
        close(dest_descriptor);
        utimes(destpath, times);
    }

    if(compress_files(outputdir, outputarchive) != 0)
    {
        cleanup(EXIT_FAILURE);
    }

}

void add_file(int itemn, char *filename, struct stat *statbuf)
{
    FILES *file;

    // check if filename already exists
    for (int i = 0; i < nfiles; i++)
    {
        file = &files[i];

        if (strcmp(file->filename, filename) == 0)
        {
            sort(statbuf, file, itemn);
            return;
        }
    }
    // else, it's a novel filename; add to struct
    files = realloc(files, (nfiles + 1) * sizeof(files[0]));
    file = &files[nfiles];
    file->filename = strdup(filename);

    file->itemn = itemn;
    file->file_size = statbuf->st_size;
    file->file_modtime = statbuf->st_mtime;
    file->file_mode = statbuf->st_mode;

    nfiles++;
}

void find_files(char *dirname, int itemn)
{
    char path[MAXPATHLEN];
    struct dirent *dp;

    sprintf(path, "%s/%s", items[itemn].tmpdir, dirname);
    DIR *dir = opendir(path);
    if (dir == NULL)
    {
        perror(path);
        cleanup(EXIT_FAILURE);
    }

    while ((dp = readdir(dir)) != NULL)
    {
        struct stat statbuf;

        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
            continue;

        sprintf(path, "%s/%s/%s", items[itemn].tmpdir, dirname, dp->d_name);
        if (stat(path, &statbuf) != 0)
        {
            perror(path);
        }

        else if (S_ISREG(statbuf.st_mode))
        {
            sprintf(path, "%s/%s", dirname, dp->d_name);
            add_file(itemn, path + 1, &statbuf);
        }

        else if (S_ISDIR(statbuf.st_mode))
        {
            sprintf(path, "%s/%s", dirname, dp->d_name);
            add_directory(path);
            find_files(path, itemn);
        }
    }

    closedir(dir);
}

void merge(void)
{
    for(int i = 0; i < nitems; i++)
    {
        find_files("", i);
    }
}
