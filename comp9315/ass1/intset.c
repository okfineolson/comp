/*
*
******************************************************************************
This file contains routines that can be bound to a Postgres backend and
called by the backend in the process of processing queries.  The calling
format for these routines is dictated by Postgres architecture.
******************************************************************************/
#include "stdio.h"
#include "postgres.h"
#include "string.h"
#include "fmgr.h"
#include "libpq/pqformat.h"		/* needed for send/recv functions */
# include "stdlib.h"

typedef struct Number
{
	char		*value;
	struct Number *next;	
}			Number;
typedef struct intSet 
{
	Number    *head;
	Number    *tail;
}			intSet ;
intSet *intset_init(void);
void addlist(intSet *plist,char *value);
int checkvalid(intSet *plist);
int checkisnumber(char *str);
int checkequal(char *a,char *b);
char *fixnum(char * num);
char *fixzero(char *num);
int checkexist(intSet *plist,char *value);
int A_contains_B(intSet *a,intSet *b);
int set_contain_num(int num , intSet * plist);
void addinrange(intSet *plist,Number *pnode);
PG_MODULE_MAGIC;
PG_FUNCTION_INFO_V1(intset_in);

Datum
intset_in(PG_FUNCTION_ARGS)
{
	intSet *plist = intset_init();
	char	   *str = PG_GETARG_CSTRING(0);
	char *ptr;
	char x[51200];
	char out[51200];
	int i, j ,k;
	int countright = 0;
	i = 0;
	j = strlen(str) - 1;
	//remove all the space in the frount and the end
	while(str[i] == ' ')
		++i;
	while(str[j] == ' '){
		--j;
	}
	k = i;
	strncpy(out, str + i , j - i + 1);
	out[j - i + 1] = '\0';
	//invalid check for not start with { and end with}
	if(str[i] != '{' || str[j] != '}'){
		ereport(ERROR,(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
				errmsg("invalid syntax intSet 1")));
	}
	//invalid check for have empty value etc {1,,2} {,2,3} {1,2,} 
	//invalid check for have space between two digits{1 2}
	if(strlen(str)>=3 && strcmp(out,"{}")!=0){
		while (k != j)
		{
			if(str[k] == ',' && str[k+1] == ','){
				ereport(ERROR,(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
					errmsg("invalid syntax intSet 2")));
			}
			if(str[k] == ',' && str[k+1] == '}'){
				ereport(ERROR,(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
					errmsg("invalid syntax intSet 3")));
			}
			if(str[k] == '{' && str[k+1] == ','){
				ereport(ERROR,(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
					errmsg("invalid syntax intSet 4")));
			}
			if(str[k] != ' '&& str[k] !=',' && str[k]!='{' && str[k+1] == ' ' &&str[k+2] != ' ' && str[k+2] != ','&&str[k+2]!='}'){
				ereport(ERROR,(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
					errmsg("invalid syntax intSet 5")));
			}
			if(str[k] == '}'){
				countright++;
			}
			k++;
		}
		if(countright > 0){
			ereport(ERROR,(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
					errmsg("invalid syntax intSet ")));
		}
	}
	if(strcmp(out,"{}") == 0){
		PG_RETURN_POINTER(plist);  
	}else{
		if (sscanf(out, " {%[^}] ", x) != 1 )
			ereport(ERROR,
				(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
				errmsg("invalid input syntax for type")));
	}
	ptr = strtok(x, ",");
	while (ptr != NULL ) {
		if(checkisnumber(fixnum(ptr)) != 0){
			ereport(ERROR,(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
				errmsg("invalid syntax intSet")));
		}
		if(strlen(fixnum(ptr)) != 0){
			addlist(plist,ptr);
		}
		ptr = strtok(NULL, ",");
	}

	PG_RETURN_POINTER(plist);
}
PG_FUNCTION_INFO_V1(intset_out);

Datum
intset_out(PG_FUNCTION_ARGS)
{
	
	intSet    *plist = (intSet *) PG_GETARG_POINTER(0);
	Number *p = plist->head; 
	char list[51200];
	char *final = malloc(51200*sizeof(char));
	list[0] = '\0';
	strcat(list,"{");
	while (p!=NULL)
	{
		strcat(list,p->value);
		if(p->next !=NULL){
			strcat(list,",");
		}
		p = p->next;
	}
	strcat(list,"}");
	strcpy(final,list);
	
	PG_RETURN_CSTRING(final);
}
//help functions
intSet *intset_init()
{
	intSet* pList =  malloc(sizeof(intSet));
	pList->head = NULL;
	pList->tail = NULL;
	return pList;
}
void addlist(intSet *pList,char *value){
	Number * a = pList->head;
	Number * pNode = malloc(sizeof(Number));
	char *v = malloc(10*sizeof(char));
	value = fixnum(value);
	strcpy(v,value);
	pNode->value = v;
	pNode->next = NULL;
	if(a==NULL){
		pList->head = pNode;
		pList->tail = pNode;
	}else{
		if(checkexist(pList,value) == 0){
			return;
		}/*
		while(a->next!=NULL){
			a = a->next;
		}*/
		addinrange(pList,pNode);
	}
	//add node to the end
}
void addinrange(intSet *plist,Number *pnode){
	Number *node = plist->head;
	Number *t = plist->tail;
	if(atoi(node->value) > atoi(pnode->value)){
		pnode->next = node;
		plist->head = pnode;
		return;
	}
	if(atoi(t->value) < atoi(pnode->value)){
		plist->tail->next = pnode;
		plist->tail = pnode;
		return;
	}
	while(node->next!=NULL){
		if(atoi(node->value)<atoi(pnode->value) && atoi(pnode->value) < atoi(node->next->value)){
			pnode->next = node->next;
			node->next = pnode;
			return;
		}
		node = node->next;
	}
	return;
}
int checkexist(intSet *plist,char *value){
	Number *a = plist->head;
	while(a!=NULL){
		if(strcmp(a->value,value) == 0){
			return 0;
		}
		a = a->next;
	}
	return 1;
}
int checkisnumber(char *value){
	char *str=value;
	if(strlen(value) == 0){
		return 0;
	}
	while(*str != '\0'){
		if (*str >= '0' && *str <= '9')  {  
		}else{
			return 1;
		}
		str++;
	}
	return 0;
}
int checkequal(char *a,char *b){
	if(strcmp(fixnum(a),fixnum(b))){
		return 0;
	}else{
		return 1;
	}
}
char* fixnum(char * num){
	char *p=num;
	int i=0;
	//int nofirstnum = 0;
	while(*p)
	{
		if(*p!=' '){
			num[i++]=*p;
		}
		p++;
	}
	num[i]='\0';
	
	if(strcmp(num,"0") == 0){
		return num;
	}
	return fixzero(num);
}
char *fixzero(char *num){
	char *p=num;
	int i=0;
	int check= 0;
	//int nofirstnum = 0;
	while(*p)
	{	
		if(*p!='0'&&check==0){
			num[i++]=*p;
			check = 1;
			p++;
			continue;
		}
		if(check == 1){
			num[i++]=*p;
		}
		p++;
	}
	num[i]='\0';
	return num;
}
int A_contains_B(intSet *a,intSet *b){
	Number *anode = a->head;
	Number *bnode = b->head;
	int check = 1;
	while(bnode!=NULL){
		while(anode!=NULL){
			if(atoi(anode->value) == atoi(bnode->value)){
				check = 0;
				anode = anode->next;
				break;
			}
			anode = anode->next;
		}
		if(check != 0){
			return 1;
		}
		check = 1;
		bnode = bnode->next;
	}
	return 0;
}
int set_contain_num(int num , intSet * plist){
	Number *p = plist->head; 
	int check = 1;
	while (p != NULL) {
		if (num == atoi(p->value)){
			check = 0;
			break;
		}
		p = p->next;
	}
	return check;
}
//oprators
PG_FUNCTION_INFO_V1(intset_contain);
Datum
intset_contain(PG_FUNCTION_ARGS)
{
	intSet    *plist = (intSet *) PG_GETARG_POINTER(1);
	int num = PG_GETARG_INT32(0);//DatumGetInt32
	
	PG_RETURN_BOOL(set_contain_num(num,plist) == 0);
}
PG_FUNCTION_INFO_V1(intset_len);
Datum
intset_len(PG_FUNCTION_ARGS)
{
	intSet *plist = (intSet *) PG_GETARG_POINTER(0);
	int count = 0;
	Number *node = plist->head;
	while(node != NULL){
		if(node->value !=NULL){
			count++;
		}
		node = node->next;
	}
	PG_RETURN_INT32(count);
}
PG_FUNCTION_INFO_V1(intset_contain_set_left);
Datum
intset_contain_set_left(PG_FUNCTION_ARGS)
{
	intSet    *a = (intSet *) PG_GETARG_POINTER(0);
	intSet    *b = (intSet *) PG_GETARG_POINTER(1);
	int check = A_contains_B(a,b);
	PG_RETURN_BOOL(check == 0);
}
PG_FUNCTION_INFO_V1(intset_contain_set_right);
Datum
intset_contain_set_right(PG_FUNCTION_ARGS)
{
	intSet    *a = (intSet *) PG_GETARG_POINTER(1);
	intSet    *b = (intSet *) PG_GETARG_POINTER(0);
	int check = A_contains_B(a,b);
	PG_RETURN_BOOL(check == 0);
}
PG_FUNCTION_INFO_V1(intset_equal);
Datum
intset_equal(PG_FUNCTION_ARGS)
{
	intSet    *a = (intSet *) PG_GETARG_POINTER(0);
	intSet    *b = (intSet *) PG_GETARG_POINTER(1);
	int checkA = A_contains_B(a,b);
	int checkB = A_contains_B(b,a);
	PG_RETURN_BOOL(checkA == 0 && checkB == 0);
}
PG_FUNCTION_INFO_V1(intset_not_equal);
Datum
intset_not_equal(PG_FUNCTION_ARGS)
{
	intSet    *a = (intSet *) PG_GETARG_POINTER(1);
	intSet    *b = (intSet *) PG_GETARG_POINTER(0);
	int checkA = A_contains_B(a,b);
	int checkB = A_contains_B(b,a);
	PG_RETURN_BOOL(checkA != 0 || checkB != 0);
}
PG_FUNCTION_INFO_V1(intset_and);
Datum
intset_and(PG_FUNCTION_ARGS)
{
	intSet    *a = (intSet *) PG_GETARG_POINTER(0);
	intSet    *b = (intSet *) PG_GETARG_POINTER(1);
	intSet *plist = intset_init();
	Number *anode = a->head;
	while (anode!=NULL)
	{
		if(set_contain_num(atoi(anode->value),b) == 0){
			addlist(plist,anode->value);
		}
		anode = anode->next;
	}
	PG_RETURN_POINTER(plist);
}
PG_FUNCTION_INFO_V1(intset_union);
Datum
intset_union(PG_FUNCTION_ARGS){
	intSet    *a = (intSet *) PG_GETARG_POINTER(0);
	intSet    *b = (intSet *) PG_GETARG_POINTER(1);
	Number *anode = a->head;
	Number *bnode = b->head;
	intSet* unionset = intset_init();
	while (anode!=NULL)
	{
		if(set_contain_num(atoi(anode->value),unionset) == 1){
			addlist(unionset,anode->value);

		}		
		anode = anode->next;
	}
	while (bnode!=NULL)
	{
		if( set_contain_num(atoi(bnode->value),unionset) == 1){
			addlist(unionset,bnode->value);
		}
		bnode = bnode->next;
	}
	PG_RETURN_POINTER(unionset);
}
PG_FUNCTION_INFO_V1(intset_disjunction);
Datum
intset_disjunction(PG_FUNCTION_ARGS){
	intSet    *a = (intSet *) PG_GETARG_POINTER(1);
	intSet    *b = (intSet *) PG_GETARG_POINTER(0);
	intSet *plist = intset_init();
	Number *anode = a->head;
	Number *bnode = b->head;
	while(anode!=NULL){
		if(set_contain_num(atoi(anode->value),b) == 1){
			addlist(plist,anode->value);
		}
		anode = anode->next;
	}
	while(bnode!=NULL){
		if(set_contain_num(atoi(bnode->value),a) == 1){
			addlist(plist,bnode->value);
		}
		bnode = bnode->next;
	}
	PG_RETURN_POINTER(plist);
}
PG_FUNCTION_INFO_V1(intset_minus);
Datum
intset_minus(PG_FUNCTION_ARGS){
	intSet    *a = (intSet *) PG_GETARG_POINTER(0);
	intSet    *b = (intSet *) PG_GETARG_POINTER(1);
	intSet *plist = intset_init();
	Number *anode = a->head;
	while(anode!=NULL){
		if(set_contain_num(atoi(anode->value),b) == 1){
			addlist(plist,anode->value);
		}
		anode = anode->next;
	}

	PG_RETURN_POINTER(plist);
}