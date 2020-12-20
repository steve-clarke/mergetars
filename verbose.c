#include "mergetars.h"

// User can enable verbose mode using the -v flag.

bool verbosemode = false;

void VERBOSE(bool newfuction, char *message, ...)
{
    if(verbosemode)
    {
        if(!newfuction) {   // !newfunction means a tab is added for neatness
            fprintf(stdout, "\t");
        }
        va_list	ap;
        va_start(ap, message);
        vfprintf(stdout, message, ap);
        fprintf(stdout,  "\n");
        va_end(ap);
    }
}
