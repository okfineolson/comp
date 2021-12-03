function expo(x:int, n:nat): int
{
    if(n==0)
    then 1
    else
    var t := expo(x, n-1);
    x * t
}
lemma {:induction false} Expon23(n: nat)
