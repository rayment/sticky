#
# Makefile
#
# Author       : Finn Rayment <finn@rayment.fr>
# Date created : 30/04/2021
#

include ../vars.mk

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
	mkdir -p include
	mkdir -p include/stb && cp build/stb/*.h include/stb/
	mkdir -p include/dr_libs && cp build/dr_libs/*.h include/dr_libs/

clean:
	rm -rf include

purge:
	rm -rf build

.PHONY: all fetch patch build install clean purge

