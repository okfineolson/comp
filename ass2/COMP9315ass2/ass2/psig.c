// psig.c ... functions on page signatures (psig's)
// part of signature indexed files
// Written by John Shepherd, March 2019

#include "defs.h"
#include "reln.h"
#include "query.h"
#include "psig.h"
#include "hash.h"
Bits pcodeword(char *attr_value,int size, int m, int k);

Bits pcodeword(char *attr_value, int size,int m, int k)
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
return cword;  // m-bits with k 1-bits and m-k 0-bits
}
Bits makePageSig(Reln r, Tuple t)
{
	assert(r != NULL && t != NULL);
	//TODO
	Bits newbits = newBits(psigBits(r));
	char **values = tupleVals(r,t);
	for(int i = 0; i < nAttrs(r);i++){
		if (strcmp(values[i], "?") != 0) {
			if(sigType(r) == 'c'){
				int m = psigBits(r) / nAttrs(r);
				int n = psigBits(r) % nAttrs(r);
				int k = m/2/maxTupsPP(r);
				Bits cw;
				if(i != 0){
					cw = pcodeword(values[i],psigBits(r),m,k);
					shiftBits(cw,m*i+n); 
				}else{
					cw = pcodeword(values[i],psigBits(r),m+n,(m+n)/2/maxTupsPP(r));
				}
				orBits(newbits,cw);
			}else{
				orBits(newbits,pcodeword(values[i],psigBits(r),psigBits(r),codeBits(r)));
			}
		}
	}
	return newbits;
	
}

void findPagesUsingPageSigs(Query q)
{
	assert(q != NULL);

	Bits querySig = makePageSig(q->rel, q->qstring);
	Bits psig = newBits(psigBits(q->rel));
	for (int i = 0; i < nPsigPages(q->rel); i++){
		Page queryPage = getPage(psigFile(q->rel), i);
		for (int j = 0; j < pageNitems(queryPage); j++){
			getBits(queryPage, j, psig);
			if (isSubset(querySig, psig)){
				PageID pid = q->nsigs;
				setBit(q->pages, pid);
			}
			q->nsigs++;
			unsetAllBits(psig);
		}
		q->nsigpages++;
	}
}

