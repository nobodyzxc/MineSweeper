GXXVER   := $(shell command -v mingw32-g++ 2> /dev/null)

ifdef GXXVER
CXX      = mingw32-g++.exe
else
CXX      = x86_64-w64-mingw32-g++.exe
endif

CXXFLAGS = -g -static -mwindows
SRCS     = msapi analyscr ctrlmus point
_SRCS    = ${SRCS:%=%.cc}
_OBJS    = ${SRCS:%=%.o}
MAIN     = main
_MAIN    = ${MAIN:%=%.cc}
TRGT     = execute

all:
ifeq ($(OS),Windows_NT)
	$(CXX) $(CXXFLAGS) -c $(_SRCS)
	$(CXX) $(CXXFLAGS) -o $(TRGT) $(_MAIN) $(_OBJS)
else
	echo 'not available on other platforms except windows'
endif
clean:
ifeq ($(shell uname -o),Cygwin)
	rm $(_OBJS) $(TRGT)
else
	del $(_OBJS) $(TRGT)
endif
