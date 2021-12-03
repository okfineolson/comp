#include<stdio.h>
#include<string.h>
#include<stdlib.h>
void creat_F (int start, int num,int* char_frequency_F,char *F,int i);
void decode(char* F,int* char_frequency_F,char* L,int* char_frequency_L,char*restriction,char*target);
int binary_search( char *a, int*b,int n, int key,int value );
int findbefore(char* F,int* char_frequency_F,char* L,int* char_frequency_L,int* decode_buffer ,int *identifier_1,int* identifier_2,int strlen);
void main(int argc,char *argv[]){
    int fgetc_result;
    int F_list[128] = {0};
    //FILE *fp = fopen(argv[2], "r");
    FILE *fp = fopen("gcc.bwt", "r");

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
    fp = fopen("gcc.bwt", "r");
    //printf("%s",p);
    int Lcount = 0;
    while ((fgetc_result = fgetc(fp)) != EOF) {
        F_list[fgetc_result]++;
        char_frequency_L[Lcount] = F_list[fgetc_result];
        Lcount++;
    }
    fclose(fp);
    char *F=(char *)malloc(flen);
    int count = 0;
    for(int i = 0;i<128;i++){
        if(F_list[i] !=0 ){
            creat_F(count,F_list[i],char_frequency_F,F,i);
            count+=F_list[i];
        }
    }
    char* restriction = "-m";
    char* target = "ana";
    decode(F,char_frequency_F, L,char_frequency_L,restriction,target);
    /*
    for(int i = 0;i<flen;i++){
        int d = F[i];
        int f = L[i];
        if(d == 10) d = 42;
        if(f == 10) f = 42;
        printf("F:    %c/%d          L:   %c/%d\n",d,char_frequency_F[i],f,char_frequency_L[i]);
    }*/
}
void creat_F (int start, int num,int* char_frequency_F,char *F,int char_ascii){
    int frequency = 1;
    for(int i = 0;i<num;i++){
        char_frequency_F[start+i] = frequency;
        F[start+i] = char_ascii;
        frequency++;
    }
}
void decode(char* F,int* char_frequency_F,char* L,int* char_frequency_L,char*restriction,char*target){
    char *each_line = (char *)malloc(strlen(F));
    int decode_buffer[2] = {1,F[0]};
    //记录次数
    int identifier_1[128] = {0};
    //记录位置
    int identifier_2[128] = {0};
    for(int i = strlen(F)-1;i>=0;i--){
        each_line[i] = decode_buffer[1];
        decode_buffer[1] = findbefore(F,char_frequency_F,L,char_frequency_L,decode_buffer,identifier_1,identifier_2,strlen(F));
    }
    printf("%s",each_line);
}
int findbefore(char* F,int* char_frequency_F,char* L,int* char_frequency_L,int* decode_buffer ,int *identifier_1,int* identifier_2,int strlen){
    int index = decode_buffer[1];
    int start = identifier_2[index];
    if(start == 0){
        int i = binary_search(F,char_frequency_F,strlen,index,decode_buffer[0]);
        identifier_1[index] = decode_buffer[0];
        identifier_2[index] = i;
        decode_buffer[0] = char_frequency_L[i];
        decode_buffer[1] = L[i];
        //printf("%d",i);
        return decode_buffer[1];
        /*
        for(int i = start; i < strlen ; i++){
            if(char_frequency_F[i] == decode_buffer[0] && F[i] == decode_buffer[1]){
                identifier_1[index] = decode_buffer[0];
                identifier_2[index] = i;
                decode_buffer[0] = char_frequency_L[i];
                decode_buffer[1] = L[i];
                return decode_buffer[1];
            }
            
        }*/
    }else{
        if(decode_buffer[0] >= identifier_1[index]){
            for(int i = start; i < strlen ; i++){
                if(char_frequency_F[i] == decode_buffer[0] && F[i] == decode_buffer[1]){
                    identifier_1[index] = decode_buffer[0];
                    identifier_2[index] = i;
                    decode_buffer[0] = char_frequency_L[i];
                    decode_buffer[1] = L[i];
                    return decode_buffer[1];
                }
            }
        }else{
            for(int i = start; i >= 0 ; i--){
                if(char_frequency_F[i] == decode_buffer[0] && F[i] == decode_buffer[1]){
                    identifier_1[index] = decode_buffer[0];
                    identifier_2[index] = i;
                    decode_buffer[0] = char_frequency_L[i];
                    decode_buffer[1] = L[i];
                    return decode_buffer[1];
                }
            }
        }
    }
}
int binary_search( char *a, int*b,int n, int key,int value ){
    int mid, front=0, back=n-1;
    while (front<=back){
        mid = front+(back-front)/2;
        if (a[mid] == key && b[mid] == value)
            return mid;
        if (a[mid] == key && b[mid] != value){
            if(b[mid] > value){
                back = mid-1;
                /*
                for(int i = mid-1;i>=0;i--){
                    if(a[i] == key && b[i] == value){
                        return i;
                    }
                }*/
            }else{
                front = mid+1;
                /*
                for(int i = mid+1;i < n;i++){
                    if(a[i] == key && b[i] == value){
                        return i;
                    }
                }*/
            }
            continue;
        }
        if (a[mid]<key){
            front = mid+1;
        }else {
            back = mid-1;
        }
    }
    return -1;
}

