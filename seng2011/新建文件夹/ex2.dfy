//Dafny 2.3.0.10506
//Dafny program verifier finished with 2 verifiedw

method String3Sort(a: string) returns (b: string)
requires |a| == 3
ensures forall i,j :: 0 <= i < j < |b| ==> b[i] <= b[j] && |b| == 3
ensures multiset(a) == multiset(b)
{
    b := a;
    if b[0]>b[1]{
        b := b[0 := b[1]][1 := b[0]];
    }
    if b[0]>b[2]{
        b := b[0 := b[2]][2 := b[0]];
    }
    if b[1]>b[2]{
        b := b[1 := b[2]][2 := b[1]];
    }
    
}