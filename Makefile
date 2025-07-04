CC=g++
CC_WINDOWS=x86_64-w64-mingw32-g++
MACROS=-DVERSION="\"v1.0.0-Pre\""
FLAGS=
OUT=bfi++
IN=src/main.cpp


all: compile


BASE-RELEASE=pkg

release: OUT=$(BASE-RELEASE)/bfipp-linux-x86_64
release: base-release linux windows
base-release:
	mkdir -p $(BASE-RELEASE)


debug: FLAGS+=-ggdb
debug: MACROS+=-DDEBUG
debug: compile


# Arguments to compile with
OPTS=-std=c++23 $(IN) -o $(OUT) $(FLAGS) $(MACROS)

linux:	OUT=$(BASE-RELEASE)/bfipp-linux-x86_64
linux:
	$(CC) $(OPTS)

windows: FLAGS+=-static -static-libstdc++ -static-libgcc
windows: OUT=$(BASE-RELEASE)/bfipp-windows-x86_64
windows:
	$(CC_WINDOWS) $(OPTS)


FLAGS+=-Wall -Wextra
compile: $(IN)
	$(CC) $(OPTS)

distclean: clean
	rm -fr ./$(BASE_RELEASE)/* -vf
clean:
	rm ./$(OUT).* -vf

