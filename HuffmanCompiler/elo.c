#include<stdio.h>
#include <stdlib.h>
#include "huff.h"
int main () {
   char c = 0b00000000;
   c |=0b00000001;
   c<<=3;
   printf("%d",c);

}