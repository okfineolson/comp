// tsig.c ... functions on Tuple Signatures (tsig's)
// part of signature indexed files
// Written by John Shepherd, March 2019

#include <unistd.h>
#include <string.h>
#include "defs.h"
#include "tsig.h"
#include "reln.h"
#include "hash.h"
#include "bits.h"
Bits codeword(char *attr_value,int size, int m, int k);
// make a tuple signature
Bits codeword(char *attr_value,int size, int m, int k)
{
   int  nbits = 0;   // count of set bits
   Bits cword = newBits(size);   // assuming m <= 32 bits
   srandom(hash_any(attr_value,strlen(attr_value)));
   while (nbits < k) {
      int i = random() % m;
      if (!bitIsSet(cword,i)) {
         setBit(cword,i);
         nbits++;
      }
   }
   return cword;// m-bits with k 1-bits and m-k 0-bits
}

Bits makeTupleSig(Reln r, Tuple t)
{
	assert(r != NULL && t != NULL);
	//TODO
	Bits newbits = newBits(tsigBits(r));
	char **values = tupleVals(r,t);
	for(int i = 0; i < nAttrs(r);i++){
		if (strcmp(values[i], "?") != 0) {
			if(sigType(r) == 'c'){
				int m = tsigBits(r) / nAttrs(r);
				int n = tsigBits(r) % nAttrs(r);
				Bits cw ;
				if(i != 0){
					cw = codeword(values[i],tsigBits(r),m,m/2);
					shiftBits(cw,m*i+n); 
				}else{
					cw = codeword(values[i],tsigBits(r),m+n,(m+n)/2);
				}
				orBits(newbits,cw);
			}else{
				Bits cw = codeword(values[i],tsigBits(r),tsigBits(r),codeBits(r));
				orBits(newbits,cw);
			}
		}
		
	}
	return newbits;
}

// find "matching" pages using tuple signatures

void 	findPagesUsingTupSigs(Query q)
{
	assert(q != NULL);
	//TODO
	Bits Querysig = makeTupleSig(q->rel,q->qstring);
	Bits sig = newBits(tsigBits(q->rel));
	for(int i = 0; i< nTsigPages(q->rel);i++){
		Page page = getPage(tsigFile(q->rel),i);
		for(int j = 0 ; j<pageNitems(page) ; j++){
			getBits(page , j , sig);
			if (isSubset(Querysig, sig)) {
				PageID pid = q->nsigs / maxTupsPP(q->rel);
				setBit(q->pages,pid);
			}		
			q->nsigs++;
			unsetAllBits(sig);
		}	
		
		q->nsigpages++;
	}
	// The printf below is primarily for debugging
	// Remove it before submitting this function
	//printf("Matched Pages:"); showBits(q->pages); putchar('\n');
}
