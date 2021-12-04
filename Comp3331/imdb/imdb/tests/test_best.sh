cd ..
./best | diff - tests/best_default.out
./best 20 | diff - tests/best_20.out
./best xyz | diff - tests/best_wrong_params.out