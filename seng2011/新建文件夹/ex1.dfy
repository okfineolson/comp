//Dafny 2.3.0.10506
//Dafny program verifier finished with 2 verifiedw
//
method StringSwap(s: string, i:nat, j:nat) returns (t: string)
requires if |s| != 0 then 0 <= i < |s| && 0 <= j < |s| else true
ensures |t| != 0 ==> 0 <= i < |t|==|s| && 0 <= j < |t|==|s| && t[i] == s[j] && t[j] == s[i]
ensures |t| != 0 ==> forall k :: 0 <= k < |t| && k!=i && k!=j ==> t[k] == s[k]
ensures multiset(s) == multiset(t)
{
    if |s| == 0 {
        t := s;
    }
    else{
        t := s[i := s[j]][j := s[i]];
    }
}
