cd ..
./minfo xyzzy | diff - tests/minfo_no_matching.out
./minfo Avatar | diff - tests/minfo_Avatar.out
./minfo "The Ring" | diff - tests/minfo_The_Ring.out
./minfo '^The Ring$' 2002 | diff - tests/minfo_The_Ring_2002.out
./minfo 'return of the king' | diff - tests/minfo_return_of_the_king.out
./minfo 'strangelove' 1964 | diff - tests/minfo_strangelove_1964.out
./minfo 'stars' xyzzy | diff - tests/minfo_wrong_year_format.out
./minfo 'return of the king' 1234 | diff - tests/minfo_return_of_the_king_1234.out
./minfo ring 2002 | diff - tests/minfo_ring_2002.out