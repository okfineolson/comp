lemma {:induction false} Divby6(n: nat)
ensures (n*n*n-n)%6 == 0
{
    
    assert (n*n*n-n) == (n-1)*n*(n+1);
    assert (n-1)*n*(n+1)%2 == 0;
    assert (n-1)*n*(n+1)%3 ==0;
    assert (n*n*n-n)%6 == 0;
    
}