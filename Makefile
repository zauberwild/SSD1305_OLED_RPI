# Gavin lyons 06-2021
# Makefile to install library for SSD1305_OLED library.
# URL: https://github.com/gavinlyonsrepo/SSD1305_OLED_RPI
# Library is installed to /usr/lib and include files are placed at /usr/include.
# Uninstall and clean options provided
# Options
# 1. make = install library
# 2. clean = deletes .o files generated by step 1 from build directory
# 3. make uninstall  = uninstalls library

# Where you want it installed
PREFIX=/usr
# where to place the libray
LIBDIR=$(PREFIX)/lib
# library name
LIB=libSSD1305_OLED_RPI
# shared library name
LIBNAME=$(LIB).so.1.0

MD=mkdir
SRC=src
OBJ=obj
SRCS = $(wildcard $(SRC)/*.cpp)
OBJS = $(patsubst $(SRC)%.cpp,  $(OBJ)/%.o, $(SRCS))

CXX=g++
CCFLAGS= -Ofast -mfpu=vfp -mfloat-abi=hard -march=armv6zk -mtune=arm1176jzf-s -Iinclude/
LDFLAGS= -lbcm2835

# make all
# reinstall the library after each recompilation
all: pre-build SSD1305_OLED_RPI install

pre-build:
	@echo
	@echo "*****************"
	@echo "[START!]"
	@echo
	$(MD) -vp $(OBJ)

# Make the library
SSD1305_OLED_RPI: $(OBJS)
	$(CXX) -shared -Wl,-soname,$(LIB).so.1 $(CCFLAGS) $(LDFLAGS)  -o ${LIBNAME} $^

# Library parts
$(OBJ)/%.o: $(SRC)/%.cpp
	$(CXX) -Wall -fPIC -c $(CCFLAGS) $< -o $@

# Install the library to LIBPATH
install:
	@echo
	@echo "*****************"
	@echo "[INSTALL LIBRARY]"
	@if ( test ! -d $(PREFIX)/lib ) ; then mkdir -vp $(PREFIX)/lib ; fi
	@install -vm 0755 ${LIBNAME} ${LIBDIR}
	@ln -svf ${LIBDIR}/${LIBNAME} ${LIBDIR}/${LIB}.so.1
	@ln -svf ${LIBDIR}/${LIBNAME} ${LIBDIR}/${LIB}.so
	@ldconfig
	@rm -rvf ${LIB}.*
	@echo "*****************"
	@echo
	@echo "[INSTALL HEADERS]"
	@if ( test ! -d $(PREFIX)/include ) ; then mkdir -p $(PREFIX)/include ; fi
	@cp -vf  include/SSD1305_OLED.h $(PREFIX)/include
	@cp -vf  include/SSD1305_OLED_graphics.h $(PREFIX)/include
	@cp -vf  include/SSD1305_OLED_Print.h $(PREFIX)/include
	@cp -vf  include/SSD1305_OLED_font.h $(PREFIX)/include
	@echo "[DONE!]"

# Uninstall the library
uninstall:
	@echo "******************"
	@echo "[UNINSTALL LIBRARY]"
	@rm -vf ${LIBDIR}/${LIB}.*

	@echo "[UNINSTALL HEADERS]"
	@rm -rvf  $(PREFIX)/include/SSD1305_OLED.h
	@rm -rvf  $(PREFIX)/include/SSD1305_OLED_graphics.h
	@rm -rvf  $(PREFIX)/include/SSD1305_OLED_Print.h
	@rm -rvf  $(PREFIX)/include/SSD1305_OLED_font.h
	@echo "[DONE!]"

# clear build files
clean:
	@echo "******************"
	@echo "[CLEAN OBJECT FILES]"
	rm -rvf $(OBJ)/*.o ${LIB}.*
	@echo "[DONE!]"
