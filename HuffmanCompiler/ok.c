#include <stdio.h>


int main(){

    char x[] ="AABCABBACD";
    int a[] = {1,2,3,-1};
    FILE *in = NULL;
    in = fopen("witam.bin","wb");
    if(in == NULL){
        return 2;
    }
    fwrite(x,sizeof(x),1,in);
    
    fclose(in);

    return 0;
}