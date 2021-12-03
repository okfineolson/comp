

lemma {:induction false} Divby2(n: nat)
ensures (n*(n-1))%2 == 0
{   
  // base case n==0 Dafny can do
  if (n == 0){
      assert (n*(n-1))%2 == 0;
  }else{
        Divby2(n-1);//// hypothesis
        // the rest Dafny can do
    }
}
