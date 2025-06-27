MACROS=
FLAGS=
OUT=bfi++
IN=src/main.cpp


all: compile

debug: FLAGS+=-ggdb
debug: MACROS+=-DDEBUG
debug: compile

compile: $(IN)
	g++ -std=c++23 $(IN) -o $(OUT) $(FLAGS) $(MACROS)
