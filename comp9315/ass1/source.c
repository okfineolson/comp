CREATE FUNCTION intset_contain(int, intset) 
   RETURNS bool
   AS '_OBJWD_/intset' 
   LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION intset_len(intset) 
   RETURNS int
   AS '_OBJWD_/intset' 
   LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION intset_contain_set_left(intset, intset) 
   RETURNS bool
   AS '_OBJWD_/intset' 
   LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION intset_contain_set_right(intset, intset) 
   RETURNS bool
   AS '_OBJWD_/intset' 
   LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION intset_equal(intset, intset) 
   RETURNS bool
   AS '_OBJWD_/intset' 
   LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION intset_not_equal(intset, intset) 
   RETURNS bool
   AS '_OBJWD_/intset' 
   LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION intset_and(intset, intset) 
   RETURNS intset
   AS '_OBJWD_/intset' 
   LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION intset_or(intset, intset) 
   RETURNS intset
   AS '_OBJWD_/intset' 
   LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION intset_disjunction(intset, intset) 
   RETURNS intset
   AS '_OBJWD_/intset' 
   LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION intset_minus(intset, intset) 
   RETURNS intset
   AS '_OBJWD_/intset' 
   LANGUAGE C IMMUTABLE STRICT;


CREATE OPERATOR ? (
   leftarg = int, rightarg = intset, procedure = intset_contain,
   restrict = scalarltsel, join = scalarltjoinsel
);
CREATE OPERATOR # (
   rightarg = intset, procedure = intset_len
);
CREATE OPERATOR >@ (
   leftarg = intset, rightarg = intset, procedure = intset_contain_set_left,
   commutator = @< , 
);
CREATE OPERATOR @< (
   leftarg = intset, rightarg = intset, procedure = intset_contain_set_right,
   commutator = >@ , 
);
CREATE OPERATOR = (
   leftarg = intset, rightarg = intset, procedure = intset_equal,
   commutator = = ,
   -- leave out negator since we didn't create <> operator
   negator = <> ,
);
CREATE OPERATOR <> (
   leftarg = intset, rightarg = intset, procedure = intset_not_equal,
   commutator = <> ,
   negator = = ,
);
CREATE OPERATOR && (
   leftarg = intset, rightarg = intset, procedure = intset_and,
   commutator = && 
);
CREATE OPERATOR || (
   leftarg = intset, rightarg = intset, procedure = intset_or,
   commutator = ||
);

CREATE OPERATOR !! (
   leftarg = intset, rightarg = intset, procedure = intset_disjunction,
   commutator = !!
);

CREATE OPERATOR - (
   leftarg = intset, rightarg = intset, procedure = intset_minus
);

