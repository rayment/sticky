#
# Makefile
#
# Author       : Finn Rayment <finn@rayment.fr>
# Date created : 29/03/2021
#

VERSION:=1.0.0
PREFIX:=/usr/local
DEBUG:=0
# extra debug tracing info if DEBUG=1
# 0 = tracing off
# 1 = memory tracing
# 2 = memory tracing + function calls + errors
DEBUG_TRACE:=0

# optional library-specific functionality

# requires assimp
ENABLE_ASSIMP:=1
# requires OpenMP
ENABLE_OPENMP:=1

# ------
# set ARCH:=32 for 32-bit builds
export ARCH:=64

export LIBNAME:=sticky
export VERSION

DISTFILES:=COPYING DOCUMENTATION INSTALL README Doxyfile Makefile \
           sticky.sln sticky.vcxproj sticky.vcxproj.filters sticky.vcxproj.user
DISTDIRS:=contrib docs src include test tools

CC:=cc
LD:=$(CC)

CCFLAGS:=--std=c99
CXXFLAGS:=-Wall -Wextra -Werror --pedantic-errors \
          -fPIC -m$(ARCH) -O3 -DVERSION=\"${VERSION}\" \
          -fvisibility=hidden
ifeq ($(DEBUG),1)
CXXFLAGS+=-g -DDEBUG=1
ifneq ($(DEBUG_TRACE),0)
CXXFLAGS+= -DDEBUG_TRACE=$(DEBUG_TRACE)
endif
else
CXXFLAGS+=-DNDEBUG
endif

ifeq ($(ARCH),64)
CXXFLAGS+=-DSTICKY_64BIT=1
else
ifeq($(ARCH),32)
CXXFLAGS+=-DSTICKY_32BIT=1
endif

MAKE_CXXFLAGS:=-DSTICKY_ISMAKELIB=1
TEST_CXXFLAGS:=

LIBRARIES:=glew sdl2 freetype2
LDFLAGS:=
TEST_LDFLAGS:=

SOURCES:=$(wildcard src/*.c) $(wildcard src/*/*.c)
HEADERS:=$(shell find include/ -name '*.h') \
         $(shell find contrib/include/ -name '*.h')

# OS-dependant variables
ifeq ($(OS),Windows_NT)
	SOURCES:=$(filter-out $(wildcard src/*/**_posix.c),$(SOURCES))
	HEADERS:=$(filter-out $(wildcard include/sticky/*/**_posix.h),$(HEADERS))
	STATIC_LIB:=$(LIBNAME)-$(VERSION).lib
	LINK_LIB=$(LIBNAME).dll
	DYNAMIC_LIB:=$(LIBNAME)-$(VERSION).dll
else
	UNAME_S:=$(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		LIBRARIES+=openal
		CXXFLAGS+=-Wpedantic -D_POSIX_C_SOURCE=200112L
		LDFLAGS+=-pthread
		ifeq ($(ENABLE_OPENMP),1)
			CXXFLAGS+=-fopenmp -DENABLE_OPENMP=1
		endif
		SOURCES:=$(filter-out $(wildcard src/*/**_win32.c),$(SOURCES))
		HEADERS:=$(filter-out \
		         $(wildcard include/sticky/*/**_win32.h),$(HEADERS))
		STATIC_LIB:=lib$(LIBNAME).a.$(VERSION)
		LINK_LIB=lib$(LIBNAME).so
		DYNAMIC_LIB:=$(LINK_LIB).$(VERSION)
	endif
	ifeq ($(UNAME_S),Darwin)
		CXXFLAGS+=-Wno-deprecated-declarations -Wno-typedef-redefinition \
		          -Wno-static-in-inline
		LDFLAGS+=-framework OpenGL -framework OpenAL -pthread
		ifeq ($(ENABLE_OPENMP),1)
			CXXFLAGS+=-Xpreprocessor -fopenmp -DENABLE_OPENMP=1
			LDFLAGS+=-lomp
		endif
		SOURCES:=$(filter-out $(wildcard src/*/**_win32.c),$(SOURCES))
		HEADERS:=$(filter-out $(wildcard src/*/**_win32.h),$(HEADERS))
		STATIC_LIB:=lib$(LIBNAME)-$(VERSION).a
		LINK_LIB=lib$(LIBNAME).dylib
		DYNAMIC_LIB:=lib$(LIBNAME)-$(VERSION).dylib
	endif
endif

ifeq ($(ENABLE_ASSIMP),1)
LIBRARIES+=assimp
CXXFLAGS+=-DENABLE_ASSIMP=1
endif

# remove memtrace utility if not debugging
ifneq ($(DEBUG),1)
SOURCES:=$(filter-out src/memory/memtrace.c,$(SOURCES))
HEADERS:=$(filter-out include/memory/memtrace.h,$(HEADERS))
endif

OBJECTS:=$(patsubst src/%,obj/%,\
         $(patsubst %.c,%.o,$(SOURCES)))
INCLUDE:=-Iinclude/ -Icontrib/include \
         $(shell pkg-config --cflags $(LIBRARIES)) \
         -DGL_GLEXT_PROTOTYPES

LDFLAGS+=$(shell pkg-config --libs $(LIBRARIES)) -lm
TEST_LDFLAGS+=-Lbuild/ -lsticky -Wl,--rpath=build

TEST_INCLUDE:=-Itest/
TEST_SOURCES:=$(wildcard test/*.c) $(wildcard test/*/*.c)
TEST_OBJECTS:=$(patsubst %.c,%.o,$(TEST_SOURCES))
TEST_BINARIES:=$(patsubst %.c,%,$(TEST_SOURCES))

# create a build string
BUILD_STRING:=lib$(LIBNAME)-$(VERSION)
ifeq ($(DEBUG),1)
BUILD_STRING+=+debug
endif
ifneq ($(DEBUG_TRACE),0)
BUILD_STRING+=+trace($(DEBUG_TRACE))
endif
ifeq ($(ENABLE_ASSIMP),1)
BUILD_STRING+=+assimp
endif
ifeq ($(ENABLE_OPENMP),1)
BUILD_STRING+=+openmp
endif

all: vardump $(OBJECTS)
	mkdir -p build
	ar -crv build/$(STATIC_LIB) $(OBJECTS)
	ranlib build/$(STATIC_LIB)
	$(CC) $(OBJECTS) $(LDFLAGS) -shared -o build/$(DYNAMIC_LIB)
	ln -fs $(DYNAMIC_LIB) build/$(LINK_LIB)
	echo "$(BUILD_STRING)" > build/buildinfo
	if [ "$(DEBUG)" = 0 ]; then \
		strip build/$(STATIC_LIB) build/$(DYNAMIC_LIB); \
	fi

vardump:
	@echo "CFLAGS   := $(CFLAGS)"
	@echo "CPPFLAGS := $(CPPFLAGS)"
	@echo "CXXFLAGS := $(CXXFLAGS)"
	@echo "INCLUDE  := $(INCLUDE)"
	@echo "LDFLAGS  := $(LDFLAGS)"

obj/%.o: src/%.c
	mkdir -p $(shell dirname $@)
	$(CC) $(CCFLAGS) $(CXXFLAGS) $(MAKE_CXXFLAGS) $(INCLUDE) -c $< -o $@

docs:
	doxygen Doxyfile

install:
	install -d $(PREFIX)/include/sticky
	cp -R include/* $(PREFIX)/include/
	find $(PREFIX)/include/sticky -type d -print0 | xargs -0 chmod 755
	find $(PREFIX)/include/sticky -type f -print0 | xargs -0 chmod 644
	install -d $(PREFIX)/lib
	cp -a -f build/$(LINK_LIB) $(PREFIX)/lib
	install build/$(DYNAMIC_LIB) $(PREFIX)/lib
	ldconfig || true

uninstall:
	rm -f $(PREFIX)/lib/$(LINK_LIB)
	rm -f $(PREFIX)/lib/$(DYNAMIC_LIB)
	rm -rf $(PREFIX)/include/sticky
	rm -rf $(PREFIX)/include/sticky.h

test: $(TEST_OBJECTS) $(TEST_BINARIES)

test/%.o: test/%.c
	$(CC) -g $(CCFLAGS) $(CXXFLAGS) $(TEST_CXXFLAGS) \
		$(TEST_INCLUDE) $(INCLUDE) -c $< -o $@

test/%: test/%.o
	$(LD) $< $(LDFLAGS) $(TEST_LDFLAGS) -o $@

dist:
	@echo "Calling \`make clean' functions to delete work prior to archival."
	@echo "You have 5 seconds to cancel the clean actions with Ctrl+C."
	@sleep 5
	cd contrib && $(MAKE) clean && $(MAKE) purge
	$(MAKE) clean_test
	$(MAKE) clean_docs
	rm -rf lib$(LIBNAME)-$(VERSION)
	mkdir -p lib$(LIBNAME)-$(VERSION)
	cp -R $(DISTFILES) $(DISTDIRS) lib$(LIBNAME)-$(VERSION)
	tar -cJf lib$(LIBNAME)-$(VERSION).tar.xz lib$(LIBNAME)-$(VERSION)
	rm -rf lib$(LIBNAME)-$(VERSION)

clean:
	rm -rf build/ obj/ $(BINARY) $(TEST_OBJECTS) $(TEST_BINARIES) || true

clean_test:
	rm -rf $(TEST_OBJECTS) $(TEST_BINARIES) || true

clean_docs:
	rm -rf docs/html || true

.PHONY: all vardump dist docs install uninstall test clean clean_test

