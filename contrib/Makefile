#
# Makefile
#
# Author       : Finn Rayment <finn@rayment.fr>
# Date created : 30/04/2021
#

include vars.mk

HERE:=$(shell pwd)

all: fetch patch build install

fetch:
	mkdir -p build/
	cd build
	git clone --depth 1 "https://github.com/nothings/stb" build/stb || true
	git clone --depth 1 "https://github.com/mackron/dr_libs" build/dr_libs \
		|| true
	cd $(HERE)

patch:
	sh patch.sh

build:

install:
	mkdir -p include/sticky
	mkdir -p include/sticky/stb && cp build/stb/*.h include/sticky/stb/
	mkdir -p include/sticky/dr_libs && cp build/dr_libs/*.h include/sticky/dr_libs/

clean:
	rm -rf include

purge:
	rm -rf build

.PHONY: all fetch patch build install clean purge

