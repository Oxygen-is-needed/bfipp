CFLAGS=$(pkg-config --cflags glfw3)
LIBS="-lGL $(pkg-config --static --libs glfw3)"
CXX=g++
WARNINGS='-Wall -Wformat -Wextra'

MAIN_MACROS=-DVERSION="\"v1.2.0-pre\""
MAIN_DIR="src"
MAIN_IN="$MAIN_DIR/main.cpp"
MAIN_OUT="$MAIN_DIR/main.o"

MAIN_UPDATE=0
for FILE in $MAIN_DIR/{*.hpp,*.cpp}; do
	if [ "$FILE" -nt "$MAIN_OUT" ]; then
		MAIN_UPDATE=1
		break
	fi
done


set -x

if [ $MAIN_UPDATE -eq 1 ]; then # If main update is needed
	$CXX -std=c++23 \
		-I lib/imgui \
		-I lib/imgui/backends \
		$WARNINGS \
		$CFLAGS $MAIN_MACROS\
		-c -o $MAIN_OUT $MAIN_IN
fi

$CXX -o pkg/bfi++ src/main.o \
	include/imgui.o \
	include/imgui_draw.o \
	include/imgui_tables.o \
	include/imgui_widgets.o \
	include/imgui_impl_glfw.o \
	include/imgui_impl_opengl3.o \
	-I imgui -I imgui/backends \
	-std=c++23 $WARNINGS $LIBS
