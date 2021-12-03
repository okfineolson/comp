#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
double entropy(double *list,int total);
double huffman(double *list,int total,int numindex);
double lzw(char *list,int total,int width);
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
    fp = fopen(argv[fileplace],"r");
    char l[128];
    fgets(l,size+1,fp);
    //printf("%s",l);
    double LZW = lzw(l,size,width);
    fclose(fp);
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
double huffman(double *oldlist,int total,int numindex){
    double list[128][128]={0};
    for(int i = 0;i<128;i++){
        if(oldlist[i] != 0){
            list[i][i] = 1;
        }
    }
    double list1[128];
    for(int i = 0;i<128;i++){
        list1[i] = oldlist[i];
    }
    double countbits[128] = {0};
    int firstsmall_index = 0;
    int secondsmalll_index = 0;
    for(int i = 1;i<numindex;i++){
        double firstsmall = total;
        double secoundsmall = total;
        for(int i = 0;i<128;i++){
            if(list1[i] == 0){
                continue;
            }
            if(list1[i]<firstsmall){
                secoundsmall = firstsmall;
                secondsmalll_index = firstsmall_index;
                firstsmall = list1[i];
                firstsmall_index = i;
            }else if(list1[i]<secoundsmall){
                secoundsmall = list1[i];
                secondsmalll_index = i; 
            }
        }
        for(int i = 0; i<128;i++){
            if(list[firstsmall_index][i] != 0 ){
                countbits[i]++;
            }
            if(list[secondsmalll_index][i] != 0 ){
                countbits[i]++;
            }
        }
        list[firstsmall_index][secondsmalll_index] = 1;
        for(int i = 0; i< 128;i++){
            if(list[secondsmalll_index][i]!=0){
                list[firstsmall_index][i] = 1;
            }
        }
        list1[firstsmall_index] = firstsmall+secoundsmall;
        list1[secondsmalll_index] = 0;
    }
    double sum = 0;
    for(int i = 0;i<128;i++){
        if(countbits[i] == 0){
            continue;
        }
        //printf("%d %f\n",i,countbits[i]);
        sum +=countbits[i]*oldlist[i];
    }
    return sum/total;
}
double lzw(char *list,int total,int width){
    char* dic[128];
    int check = 0;
    int count = 0; 
    int k = 0;
    for(int i = 0;i<128;i++){
        dic[i] = malloc(sizeof(char)*128);
    }
    char p[128] ;
    for(int i = 0;i<total;i++){
        char c[1];
        c[0] = list[i];
        printf("%s\n",c);
        
        strcat(p,c);
        printf("%s\n",p);
        for(int j = 0;j<k;j++){
            if(strcmp(dic[j],p)){
                check = 1;
                break;
            }
        
        }
        //printf("%d",check);
        if(check == 0){
            strcpy(dic[k],p);
            k++;
            count++;
            strcpy(p,c);
        }
        printf("%s\n%s",dic[k-1],c);
        break;
    }
    return width*count/total;
}