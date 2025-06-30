MACROS=-DVERSION="\"v1.0.0-Pre\""
FLAGS=
OUT=bfi++
IN=src/main.cpp


all: compile


BASE-RELEASE=pkg

release: OUT=$(BASE-RELEASE)/bfipp-linux-x86_64
release: compile


debug: FLAGS+=-ggdb
debug: MACROS+=-DDEBUG
debug: compile

compile: FLAGS+=-Wall -Wextra
compile: $(IN)
	g++ -std=c++23 $(IN) -o $(OUT) $(FLAGS) $(MACROS)
