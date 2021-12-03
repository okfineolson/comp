//Dafny 2.3.0.10506
//Dafny program verifier finished with 4 verified

method BadSort(a: string) returns (b: string)
requires forall i :: 0 <= i < |a|  ==> a[i] in {'a','b','d'} 
ensures multiset(b)==multiset(a);
ensures forall i :: 0 <= i < |b|  ==> b[i] in {'a','b','d'} 
ensures sortedbad(b)
{
    var next := 0;
    var acolor := 0;
    var dcolor := |a|; 
    b := a;
    while (next != dcolor)
    decreases dcolor-next
    invariant 0 <= acolor<=next<=dcolor <= |b| 
    invariant multiset(b)==multiset(a);
    invariant forall i:: 0 <=i<acolor ==> b[i]=='b'
    invariant forall i:: acolor<=i<next ==> b[i]=='a'
    invariant helper(b,dcolor)
    invariant forall i :: 0 <= i < |b|  ==> b[i] in {'a','b','d'}
    invariant |a| == |b| 
    {
        if (b[next] == 'b'){
            b := b[next := b[acolor]][acolor := b[next]];
            next := next+1;
            acolor:= acolor+1;
        }else if (b[next] == 'a'){
            next := next+1;
        }else {
            dcolor := dcolor-1;
            b := b[next := b[dcolor]][dcolor := b[next]];
            
        }
    } 
}
predicate helper(s:string,d:int)
requires 0<=d<=|s|
{
  forall i :: d<=i<|s| ==> s[i] == 'd'
}
predicate sortedbad(s:string)
requires forall i :: 0 <= i < |s|  ==> s[i] in {'a','b','d'} 
{
    forall i,j :: 0 <= i < |s|&&0 <= j < |s|&&((s[i]=='a'&&s[j]=='d')||(s[i]=='b'&&s[j]=='d')||(s[i]=='b'&&s[j]=='a')) ==> i < j
}


