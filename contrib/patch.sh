#!/bin/sh

# make patch with:
# diff -ruN <orig> <new>

for f in patches/*;
do
	[ -f "$f" ] || continue
	b=$(basename "$f")
	for p in patches/"$b"/*.diff;
	do
		[ -f "$p" ] || continue
		echo "patch --verbose -d build/""$b""/ < "$p""
		patch -d build/"$b"/ < "$p"
	done
done

