GXXVER   := $(shell command -v mingw32-g++ 2> /dev/null)

ifdef GXXVER
CXX      = mingw32-gcc.exe
else
CXX      = x86_64-w64-mingw32-gcc.exe
endif

CXXFLAGS = -g -static -mwindows
MAIN     = main
_MAIN    = ${MAIN:%=%.c}
TRGT     = sol

all:
ifeq ($(OS),Windows_NT)
	$(CXX) $(CXXFLAGS) -o $(TRGT) $(_MAIN)
else
	echo 'not available on other platforms except windows'
endif
clean:
ifeq ($(shell uname -o),Cygwin)
	rm $(_OBJS) $(TRGT)
else
	del $(_OBJS) $(TRGT)
endif
