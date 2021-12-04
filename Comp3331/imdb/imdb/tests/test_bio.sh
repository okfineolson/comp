cd ..
./bio | diff - tests/bio_missing_args.out
./bio xyzzy | diff - tests/bio_no_name_matching.out
./bio 'Kyle MacLachlan' | diff - tests/bio_Kyle_MacLachlan.out
./bio 'jacques tati' | diff - tests/bio_jacques_tati.out
./bio 'spike lee' | diff - tests/bio_spike_lee.out