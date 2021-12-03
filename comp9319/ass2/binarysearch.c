#include<stdio.h>
#include<string.h>
#include<stdlib.h>
void creat_F (int start, int num,int* char_frequency_F,char *F,int i);
void decode(char* F,int* char_frequency_F,char* L,int* char_frequency_L,char*restriction,char*target,int *F_start,int* F_end,int listnum);
int binary_search(int*b, int key,int front,int back );
int findbefore(int* char_frequency_F,char* L,int* char_frequency_L,int* decode_buffer ,int *F_start,int* F_end);
int KMP(char* s,char*t);
void Getnext(int* next,char* t);
void main(int argc,char *argv[]){
    int fgetc_result;
    int F_list[128] = {0};

    FILE *fp;
    if(argc == 5){
        fp = fopen(argv[2], "r");
    }else{
        fp = fopen(argv[1], "r");
    }
    fseek(fp,0L,SEEK_END);
    int flen=(int)ftell(fp);
    if(flen<=0){
        return;
    }
    int *char_frequency_L=(int *)malloc(sizeof(int)*flen);
    int *char_frequency_F=(int *)malloc(sizeof(int)*flen);
    for(int i = 0;i<flen;i++){
        char_frequency_F[i] = 0;
        char_frequency_L[i] = 0;
    }
    char *L=(char *)malloc(flen);
    fseek(fp,0L,SEEK_SET);
    fread(L,flen,1,fp);
    fclose(fp);
    if(argc == 5){
        fp = fopen(argv[2], "r");
    }else{
        fp = fopen(argv[1], "r");
    }
    //printf("%s",p);
    int Lcount = 0;
    while ((fgetc_result = fgetc(fp)) != EOF) {
        F_list[fgetc_result]++;
        char_frequency_L[Lcount] = F_list[fgetc_result];
        Lcount++;
    }
    fclose(fp);
    char *F=(char *)malloc(flen);
    int F_start[128] = {0};
    int F_end[128] = {0};
    int count = 0;
 
    for(int i = 0;i<128;i++){
        if(F_list[i] !=0 ){
            F_start[i] = count;
            creat_F(count,F_list[i],char_frequency_F,F,i);
            count+=F_list[i];
            F_end[i] = count; 
        }
    }
    //printf("%d",F_list[10]);
    if(argc == 5)
        decode(F,char_frequency_F, L,char_frequency_L,argv[1],argv[4],F_start,F_end,F_list[10]);
    else
        decode(F,char_frequency_F, L,char_frequency_L,"none",argv[3],F_start,F_end,F_list[10]);
}
void creat_F (int start, int num,int* char_frequency_F,char *F,int char_ascii){
    int frequency = 1;
    for(int i = 0;i<num;i++){
        char_frequency_F[start+i] = frequency;
        F[start+i] = char_ascii;
        frequency++;
    }
}
void decode(char* F,int* char_frequency_F,char* L,int* char_frequency_L,char*restriction,char*target,int *F_start,int* F_end,int listnum){
    char *each_line = (char *)malloc(strlen(F));
    int decode_buffer[2] = {1,F[0]};
    int target_count = 0;
    int m = 0;
    int n = 0;
    int *ncheck=(int *)malloc(sizeof(int)*listnum+1);
    int *ncount=(int *)malloc(sizeof(int)*listnum+1);
    for(int i = 0;i<listnum;i++){
        ncheck[i] = 0;
        ncount[i] = 0;
    }
    int maximum = 0;
    for(int i = strlen(F)-1;i>=0;i--){
        each_line[i] = decode_buffer[1];
        decode_buffer[1] = findbefore(char_frequency_F,L,char_frequency_L,decode_buffer,F_start,F_end);
        if(decode_buffer[1] == 10){
            ncount[decode_buffer[0]] = i;
            //printf("%d\n",decode_buffer[0]);
        }
    }
    //printf("%s",each_line);

    
    if(strcmp(restriction,"-o") == 0){
        FILE *fp = fopen(target, "w");
        fputs( each_line, fp );
        fclose(fp);
        return;
    }
    int target_last = target[strlen(target)-1];
    decode_buffer[1] = target_last;
    int j = 1;
    int check = 0;

    for(int i = F_start[target_last]; i < F_end[target_last];i++){
        //printf("%d\n",i);
        decode_buffer[0] = j;
        decode_buffer[1] = target_last;
        int k = 1;
        while(findbefore(char_frequency_F,L,char_frequency_L,decode_buffer,F_start,F_end) != 10){
            if(target[strlen(target)-k-1] == decode_buffer[1] && check == 0) k++;
            else if(check == 0){
                break;
            }
            if(k == strlen(target) ){
                //printf("yes");
                check = 1;
                m++;
                k = 1;
            }
        }
        if(check){
            ncheck[decode_buffer[0]] = 1;
            //printf("%d\n",decode_buffer[0]);
            check = 0;
        }
        j++;
    }

    for(int i = 0;i<listnum+1;i++){
        if(ncheck[i]){
            //printf("%d\n",i);
            if(strcmp(restriction,"none") == 0){
                for(int j = ncount[i];each_line[j]!=10;j++){
                    printf("%c",each_line[j]);
                }
                printf("\n");
            }
            n++;
        }
    }
    
    if(strcmp(restriction,"-m") == 0)printf("%d\n",m);
    if(strcmp(restriction,"-n") == 0)printf("%d\n",n);
    //printf("%s",each_line);
}
int findbefore(int* char_frequency_F,char* L,int* char_frequency_L,int* decode_buffer ,int *F_start,int* F_end){
    int index = decode_buffer[1];
  
    int i = binary_search(char_frequency_F,decode_buffer[0],F_start[index],F_end[index]);
    decode_buffer[0] = char_frequency_L[i];
    decode_buffer[1] = L[i];
    //printf("%d",i);
    return decode_buffer[1];
}
int binary_search(int*b, int key,int front,int back ){
    int mid;
    while (front<=back){
        mid = front+(back-front)/2;
        if (b[mid] == key){
            return mid;
        }else{
            if(b[mid]<key){
                front = mid+1;
            }else {
                back = mid-1;
            }
        }
    }
    return -1;
}


