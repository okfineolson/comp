method Getmini(a:array<int>) returns(mini:nat)
requires a.Length > 0
ensures mini < a.Length
//ensures forall i::0 <= i < mini<a.Length ==> a[i] > a[mini]
//ensures forall j::0 <= j < a.Length ==> a[j] >= a[mini]
{
    var size := 0;
    var min := 0;
    while (size<a.Length)
    invariant size <=a.Length
    invariant min <a.Length
    {
        if (a[min] > a[size]){
            min:=size;
        }
        size:=size+1;
    }
    mini := min;
}
