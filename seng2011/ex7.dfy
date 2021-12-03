//Dafny program verifier finished with 1 verified, 0 errors
//Program compiled successfully
method BigFoot(step: nat) // DO NOT CHANGE
requires step != 0
{
  var indx := 0; // DO NOT CHANGE
  while indx<=42 // DO NOT CHANGE
  decreases 42 - indx
  invariant 0<= indx <= 42 + step && indx%step == 0
  { indx := indx+step;} // DO NOT CHANGE
  assert 42 <= indx <= 42+step;
}
