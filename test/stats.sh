#!/bin/sh

#
# stats.sh
# Code statistics script.
#
# Author       : Finn Rayment <finn@rayment.fr>
# Date created : 05/10/2021
#

folders="src include"
tags="TODO FIXME"

# scan for tags in files and print them
for i in $tags;
do
	search=`cd .. && grep --color=always -B1 -A3 -e "$i" -n -r $folders`
	numsearch=`printf "%s" "$search" | grep "$i" | grep -c "^"`

	echo "-----------"
	echo "There are $numsearch $i tags in the project."
	if [ "$numsearch" != 0 ];
	then
		echo "$search\n"
	fi
done

lines_hdr=`cd .. && find include/ -name '*.h' | xargs cat | wc -l | xargs`
lines_src=`cd .. && find src/ -name '*.c' | xargs cat | wc -l | xargs`
echo "There are $lines_hdr lines of header code."
echo "There are $lines_src lines of source code."

