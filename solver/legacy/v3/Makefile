CXX      = mingw32-g++.exe
CXXFLAGS = -g -static -mwindows
SRCS     = msapi analyscr ctrlmus
_SRCS    = ${SRCS:%=%.c}
_OBJS    = ${SRCS:%=%.o}
MAIN     = main
_MAIN    = ${MAIN:%=%.c}
TRGT     = sol

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
