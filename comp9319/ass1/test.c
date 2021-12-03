#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
int main(int argc, char* argv[]){
    int width = 12;
    int fileplace = 1;
    int numindex = 0;
    double size = 0;
    if(argc != 2){
        width = atoi(argv[2]);
        fileplace = 3;
    }
    char *buff = malloc(sizeof(char)*50);
    FILE *fp = fopen(argv[fileplace],"r");
    if(fp == NULL){
        printf("can not find file");
        return -1;
    }
    int buffer[20];
    fread(buffer, width, 1, fp);
    printf("%s\n", buffer);
}