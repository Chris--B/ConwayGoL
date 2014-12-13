#/bin/bash

dir=~/host
out=$dir/callgrind/$1.txt
dot=$dir/callgrind/$1.dot

valgrind --tool=callgrind --callgrind-out-file=$out ./Conway
~/gprof2dot.py --format=callgrind $out --output=$dot
dot -Tpng $dot -o $dir/$1.png
