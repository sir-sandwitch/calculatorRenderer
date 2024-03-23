# ----------------------------
# Makefile Options
# ----------------------------

NAME = RENDER
#ICON = icon.png
DESCRIPTION = "3D cube"
COMPRESSED = NO
HAS_PRINTF = NO

CFLAGS = -Wall -Wextra -Oz
CXXFLAGS = -Wall -Wextra -Oz

# ----------------------------

include $(shell cedev-config --makefile)