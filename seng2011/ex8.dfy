//Dafny program verifier finished with 2 verified, 0 errors
//Program compiled successfully
method GetEven(a: array<nat>) // DO NOT CHANGE
requires true
ensures forall i::(0<=i<a.Length ==> a[i]%2 ==0)
 modifies a 
{
  var k:=0;
  while k < a.Length
  invariant a.Length >= k >= 0 && forall i::(0<=i<k==> a[i]%2 == 0)
  {if(a[k] % 2 != 0)
  {a[k] := a[k] + 1;}
  k := k + 1;}
}
