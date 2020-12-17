#include "mergetars.h"

ITEMS  *items         = NULL;
int     nitems         = 0;

// populate ITEM struct with given tarfile
ITEMS *add_item(char *input_tarfile)
{
    items          = realloc(items, (nitems+1)*sizeof(items[0]));

    ITEMS  *itm    = &items[nitems];

    itm->tarfile    = strdup(input_tarfile);
    itm->tmpdir     = make_temp_dir();

    ++nitems;

    return itm;
}
