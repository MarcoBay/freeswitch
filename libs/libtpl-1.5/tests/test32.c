#include "tpl.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char*argv[]) {
    tpl_bin bin;
    tpl_node *tn;
    int i;
    char *file = "/tmp/test32.tpl";
    char str[10];

    strcpy(str,"good egg");
    bin.addr = str;
    bin.sz = 4;  /* just going to pack 'good' (no NUL) */

    tn = tpl_map("A(B)", &bin);
    for(i=0; i < 5; i++) {
        tpl_pack(tn,1);
        bin.addr = (char*)(bin.addr) + 1;
    }
    memset(str,0,10);   /* just to test that buf was copied */
    tpl_dump(tn,TPL_FILE,file);
    tpl_free(tn);

    bin.addr = NULL;
    bin.sz = 0;

    tn = tpl_map("A(B)", &bin);
    tpl_load(tn,TPL_FILE,file);
    while (tpl_unpack(tn,1) > 0) {
        /* print the buffer char-by-char ; its not a nul-termd string */
        printf("buffer length: %u\n", bin.sz);
        for(i=0; i < bin.sz; i++) printf("%c", ((char*)bin.addr)[i]);
        printf("\n");

        if (bin.sz > 0) 
            free(bin.addr);  /* malloc'd for us by tpl_unpack, we must free */
    }
    tpl_free(tn);
    return(0);

}

