#!/bin/sh

#
# test.sh
# Sticky test suite.
#
# Author       : Finn Rayment <finn@rayment.fr>
# Date created : 05/10/2021
#

fail=0
total=0
passed=0

assert_pass() {
	echo "--- $1"
	total=$((total+1))
	./$1
	if [ ! $? -eq 0 ];
	then
		fail=1
		#printf "\e[1;31mtest failed! (code $?).\e[0m\n"
	else
		passed=$((passed+1))
		#printf "\e[1;32mtest passed\e[0m\n"
	fi
}

assert_pass collections/linkedlist
assert_pass concurrency/mutex
assert_pass concurrency/thread
assert_pass math/mat3
assert_pass math/mat4
assert_pass math/quat
assert_pass math/vec2
assert_pass math/vec3
assert_pass math/vec4
assert_pass math/transform
assert_pass util/random

echo "--- $passed/$total tests passed ---"
if [ ! $passed -eq $total ];
then
	exit 1
else
	exit 0
fi

