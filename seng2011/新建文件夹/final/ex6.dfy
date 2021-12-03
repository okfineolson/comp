datatype Tree = Leaf(n:int) | Fork(n:int, l:Tree, r:Tree) | Bend(n:int, b:Tree)
function total(t: Tree): int
{ match t

    case Leaf(n) => n
    case Fork(n,l,r) => n+total(l)+total(r)
    case Bend(n,b) => n+total(b)

}
function bag(t: Tree): set<int>
{match t
    case Leaf(n) => {n}
    case Fork(n,l,r) => {n}+bag(l)+bag(r)
    case Bend(n,b) => {n} + bag(b)
}
predicate growing(t: Tree)
{match t
    case Leaf(n) => true
    case Fork(n,l,r) => n > l.n && n > r.n && growing(l) && growing(r)
    case Bend(n,b) => n > b.n && growing(b) 
}