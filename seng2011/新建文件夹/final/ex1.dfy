predicate sorted(s: string) 
decreases s
{
    if (|s| < 2){
        true
    }else{
        if(s[0]<=s[1]){
            sorted(s[1..])
        }else{
            false
        }
    }
}