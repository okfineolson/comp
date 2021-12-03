#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
double entropy(double *list,int total);
double huffman(double *list,int total,int numindex);
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
    double countlist[128] = {0};
    while(1){
        int a = fgetc(fp);
        if(a == 10){
            break;
        }
        if(countlist[a] == 0){
            numindex++;
        }
        countlist[a]++;
        size++;
    }
    fclose(fp);
    double entrop = entropy(countlist,size);
    printf("%.2f\n",entrop);
    double huff = huffman(countlist,size,numindex);
    printf("%.2f\n",huff);
    return 0;
}
double entropy(double *list,int total){
    double sum = 0;
    for(int i = 0 ;i< 128;i++){
        if(list[i] == 0){
            continue;
        }
        
        double a = list[i]/total;
        sum+= -a*log(a)/log(2);

    }
    return sum;
}
double huffman(double *list1,int total,int numindex){
    double list[128];
    for(int i = 0;i<128;i++){
        list[i] = list1[i]; 
    }
    double countbits[128] = {0};
    int firstsmall_index = 0;
    int secondsmalll_index = 0;
    for(int i = 1;i<numindex;i++){
        double firstsmall = total;
        double secoundsmall = total;
        for(int i = 0;i<128;i++){
            if(list[i] == 0){
                continue;
            }
            if(list[i]<firstsmall){
                secoundsmall = firstsmall;
                secondsmalll_index = firstsmall_index;
                firstsmall = list[i];
                firstsmall_index = i;
            }else if(list[i]<secoundsmall){
                if(firstsmall == list[i] && countbits[i] != 0){
                    continue;
                }
                secoundsmall = list[i];
                secondsmalll_index = i; 
            }
        }
        //printf("i %f %f\n",firstsmall,secoundsmall);
        countbits[firstsmall_index]++;
        countbits[secondsmalll_index]++;
        list[secondsmalll_index] =firstsmall+secoundsmall;
        list[firstsmall_index] =firstsmall+secoundsmall;
        //printf("1st ind:%d  2nd index:%d\n",firstsmall_index,secondsmalll_index);
        for(int i = 0;i<128;i++){
            if(list[i] == firstsmall || list[i] == secoundsmall){
                if(i != firstsmall_index && i != secondsmalll_index && countbits[i]!= 0){
                    list[i] = firstsmall+secoundsmall;
                    //printf("other index %d\n",i);
                    countbits[i]++;
                }        
            }
        }
    }
    double sum = 0;
    for(int i = 0;i<128;i++){
        if(countbits[i] == 0){
            continue;
        }
        //printf("%d %f\n",i,countbits[i]);
        sum +=countbits[i]*list1[i];
    }
    return sum/total;
}