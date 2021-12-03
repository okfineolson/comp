//Dafny program verifier finished with 2 verified, 0 errors
//Program compiled successfully

method SumOfSquares(n:int) returns (s:int)
requires n>=0
ensures s == n*(n+1)*(2*n+1)/6
{
  var k:=0;
  s:= 0;
  while k<=n 
  invariant 0<=k<=n+1 && s == (k-1)*(k)*(2*(k-1)+1)/6
  decreases n - k
  {s:= s + k*k ; k := k + 1;}
}
