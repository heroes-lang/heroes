#include <stdlib.h>
#include <stdio.h>
int main(int argc, char**argv){
    int which = argc>1 ? atoi(argv[1]) : 0;
    char *b = malloc(64);
    char stackbuf[64];
    fprintf(stderr,"before %d\n", which);
    if (which==0) free(b+16);        /* interior */
    if (which==1) { free(b); free(b); } /* double free */
    if (which==2) free(stackbuf);    /* stack pointer */
    if (which==3) free((void*)0x1234); /* garbage */
    if (which==4) { char *big = malloc(100000); free(big+16); } /* large: not nano */
    fprintf(stderr,"after\n");
    return 0;
}
