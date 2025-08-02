CFLAGS=$(pkg-config --cflags glfw3)
OPENGL_LINKS="-lGL $(pkg-config --static --libs glfw3)"
LIBS="$OPENGL_LINKS"

CXX=g++
WARNINGS='-Wall -Wformat -Wextra'
FLAGS=

MAIN_MACROS=-DVERSION="\"v1.2.0-pre\""
MAIN_DIR="src"
MAIN_IN="$MAIN_DIR/main.cpp"
MAIN_OUT="$MAIN_DIR/main.o"

MAIN_UPDATE=1
if [ -n "$UPDATE" ]; then
	MAIN_UPDATE=0
	for FILE in $MAIN_DIR/{*.hpp,*.cpp}; do
		if [ "$FILE" -nt "$MAIN_OUT" ]; then
			MAIN_UPDATE=1
			break
		fi
	done
fi


OUT_FILE=bfi++
OUT_DIR=pkg
INCLUDE_DIR=include

set -x

if [ -n "$WINDOWS" ]; then
	OUT_FILE=bfi++.exe
	CXX="x86_64-w64-mingw32-g++"
	OPENGL_LINKS="-lglfw3 -lopengl32 -lgdi32"
	LIBS="$OPENGL_LINKS -lstdc++exp -static"
	MAIN_UPDATE=1
	INCLUDE_DIR=include/win
fi

if [ -n "RELEASE" ]; then
	FLAGS+="-O3"
fi


if [ $MAIN_UPDATE -eq 1 ]; then # If main update is needed
	$CXX -std=c++23 \
		-I lib/imgui \
		-I lib/imgui/backends \
		$WARNINGS \
		$CFLAGS $MAIN_MACROS \
		-c -o $MAIN_OUT $MAIN_IN
	if [ $? -ne 0 ]; then
		exit
	fi
fi

OUT=$OUT_DIR/$OUT_FILE
$CXX -o $OUT $MAIN_OUT \
	$INCLUDE_DIR/imgui.o \
	$INCLUDE_DIR/imgui_draw.o \
	$INCLUDE_DIR/imgui_tables.o \
	$INCLUDE_DIR/imgui_widgets.o \
	$INCLUDE_DIR/imgui_impl_glfw.o \
	$INCLUDE_DIR/imgui_impl_opengl3.o \
	-I imgui -I imgui/backends \
	-std=c++23 $WARNINGS $LIBS
