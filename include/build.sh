set -x

CFLAGS=$(pkg-config --cflags glfw3)
WARNINGS="-Wall -Wextra -Wformat"
FLAGS="$WARNINGS $CFLAGS"

CXX=g++
OUT_FILE=.

if [ -n "$WINDOWS" ]; then
	CXX="x86_64-w64-mingw32-g++"
	OUT_FILE=win
	mkdir $OUT_FILE
fi

$CXX -std=c++23 -I../lib/imgui -I../lib/imgui/backends $FLAGS -c -o $OUT_FILE/imgui.o ../lib/imgui/imgui.cpp
$CXX -std=c++23 -I../lib/imgui -I../lib/imgui/backends $FLAGS -c -o $OUT_FILE/imgui_draw.o ../lib/imgui/imgui_draw.cpp
$CXX -std=c++23 -I../lib/imgui -I../lib/imgui/backends $FLAGS -c -o $OUT_FILE/imgui_tables.o ../lib/imgui/imgui_tables.cpp
$CXX -std=c++23 -I../lib/imgui -I../lib/imgui/backends $FLAGS -c -o $OUT_FILE/imgui_widgets.o ../lib/imgui/imgui_widgets.cpp
$CXX -std=c++23 -I../lib/imgui -I../lib/imgui/backends $FLAGS -c -o $OUT_FILE/imgui_impl_glfw.o ../lib/imgui/backends/imgui_impl_glfw.cpp
$CXX -std=c++23 -I../lib/imgui -I../lib/imgui/backends $FLAGS -c -o $OUT_FILE/imgui_impl_opengl3.o ../lib/imgui/backends/imgui_impl_opengl3.cpp

$CXX -std=c++23 -I../lib/imgui -I../lib/imgui/backends $FLAGS -c -o $OUT_FILE/imgui_stdlib.o ../lib/imgui/misc/cpp/imgui_stdlib.cpp
