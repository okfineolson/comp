method Symmetric(a: array<int>) returns (flag: bool)
//print work but assert not
ensures flag == true || flag == false
ensures a.Length == 1 && a.Length == 0 ==> flag == true
//ensures flag == true ==> a.Length == 1 || a.Length == 0 || forall i :: 0<=i<=a.Length/2<a.Length==> a[i] == a[a.Length-i-1]
//ensures flag == false ==> a.Length != 1 && a.Length != 0 && !forall i :: 0<=i<=a.Length/2<a.Length==> a[i] == a[a.Length-i-1]
{
    if(a.Length == 1 || a.Length == 0){
      flag:= true;
    }else{
      var num := 0;
      var size := a.Length/2;
      flag := true;
      while(num <= size)
      decreases size-num
      {
        if(a[num] != a[a.Length-1-num]){
          flag:=false;
        }
        num:=num+1;
      } 
    }
}
