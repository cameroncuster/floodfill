SOURCE = src/floodfill.cpp \
		 src/imageFileIO.cpp \
		 src/memory.cpp

OBJS = $(SOURCE:.cpp=.o)

#GNU C/C++ Compiler
GCC = g++

# GNU C/C++ Linker
LINK = g++

# Compiler flags
CFLAGS = -Wall -O3 -I inc
CXXFLAGS = $(CFLAGS)

.PHONY: clean

# Targets include all, clean, debug, tar

all : floodfill

floodfill: $(OBJS)
	$(LINK) -o $@ $^

clean:
	rm -rf src/*.o src/*.d

tar:
	clean
	tar zcvf floodfill.tgz $(SOURCE) inc/*.h Makefile

help:
	@echo " make all   - builds the main target"
	@echo " make       - same as make all"
	@echo " make clean - remove .o .d core main"
	@echo " make debug - make all with -g and -DDEBUG"
	@echo " make tar   - make a tarball of .cpp and .h files"
	@echo " make help  - this message"

-include $(SOURCE:.cpp=.d)

%.d: %.cpp
	@set -e; rm -rf $@;$(GCC) -MM $< $(CXXFLAGS) > $@
