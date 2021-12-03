method ShuffleSort(a: array<char>) returns (b: array<char>)
ensures sortedarray(b, 0, b.Length)
ensures a.Length == b.Length
//ensures multiset not work
{
    
    b:= new char[a.Length];
    var num:= 0;
    while(num<b.Length)
    invariant num <= b.Length
    invariant forall k: nat :: k < num ==> b[k] == a[k]
    {
      b[num]:=a[num];
      num:=num+1;
    }
    var i := 1;
    if (b.Length == 0){
      return b;
    }
    while(i < b.Length)
    invariant 0<i<=b.Length
    invariant sortedarray(b, 0, i)
    {
        var j := i-1;
        var suff := b[i];
        b[i]:=b[j];
        while (j >= 0 && suff < b[j] ) 
        invariant j<i<b.Length
        invariant sortedarray(b, 0, i+1)
        invariant forall k::j<k<i ==>b[k]>suff
        {
            b[j + 1] := b[j];
            j:=j-1;
        }
        b[j+1]:=suff;
        i:=i+1;
    }
}
predicate sortedarray(c:array<char>, m:nat, n:nat)
requires 0<=m<=n<=c.Length
reads c
{ 
    if c.Length > 1 then forall j,k:: m<=j<k<n ==> c[j] <= c[k] else true
}
