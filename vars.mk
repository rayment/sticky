#
# vars.mk
# Variables for project building.
#
# Author       : Finn Rayment <finn@rayment.fr>
# Date created : 08/06/2022
#

# -----------------------------------------------------
# GLOBAL VARIABLES
#

# prefix to use for install/uninstall - this is not used on Windows
PREFIX:=/usr/local
# project build number
VERSION:=1.0.0
# set ARCH:=32 for 32-bit builds - this is not used on Windows
ARCH:=64
# set DEBUG:=1 to turn on debugging
DEBUG=:=0
# extra debug tracing info if DEBUG:=1
# 0 = tracing off
# 1 = memory tracing
# 2 = memory tracing + function calls + errors
DEBUG_TRACE:=1

# code and library toggles - 0 = off, 1 = on
ENABLE_ASSIMP:=1
ENABLE_OPENMP:=1

# -----------------------------------------------------
# LINUX/UNIX VARIABLES
#

# -----------------------------------------------------
# MACOS VARIABLES
#

# -----------------------------------------------------
# WINDOWS VARIABLES
#

# -----------------------------------------------------
# OTHER BUILD VARIABLES
#

# library versions to use for contrib building
# DO NOT CHANGE IF YOU DO NOT KNOW WHAT YOU ARE DOING
SDL2_VER=2.0.22
OPENAL_VER=1.22.0
GLEW_VER=2.2.0
FREETYPE2_VER=2.12.1

