cd ..
./rels | diff - tests/rels_wrong_format.out
./rels xyzzy | diff - tests/rels_no_matching.out
./rels ocean | diff - tests/rels_ocean.out
./rels "Ocean's Eleven" | diff - tests/rels_ocean_eleven.out
./rels 'Lemonade Joe' | diff - tests/rels_Lemonade_Joe.out
./rels 'Ne Zha' | diff - tests/rels_Ne_Zha.out