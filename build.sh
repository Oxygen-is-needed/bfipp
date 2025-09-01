CFLAGS=$(pkg-config --cflags glfw3)
OPENGL_LINKS="-lGL $(pkg-config --static --libs glfw3)"
LIBS=""
if [ -n "$STATIC" ]; then
	LIBS+=" -static"
fi
LIBS+=" $OPENGL_LINKS"
VERSION="v1.3.0"

if [ -z "$CXX" ]; then
	CXX=g++
fi
WARNINGS='-Wall -Wformat -Wextra'
FLAGS=

MAIN_MACROS="-DVERSION=\"$VERSION\" -DCXX=\"$CXX\""
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


OUT_NAME="bfi++"
OUT_FILE="$OUT_NAME-$VERSION"
OUT_DIR=pkg
INCLUDE_DIR=include

if [ -n "$STATIC" ]; then
	OUT_FILE+="-static"
fi

set -x

if [ -n "$WINDOWS" ]; then
	CXX="x86_64-w64-mingw32-g++"
	OPENGL_LINKS="-lglfw3 -lopengl32 -lgdi32"
	LIBS=""
	if [ -n "$STATIC" ]; then
		LIBS+=" -static"
	fi
	LIBS+=" $OPENGL_LINKS -lstdc++exp"
	MAIN_UPDATE=1
	INCLUDE_DIR=include/win
	OUT_FILE+="-windows"
else
	OUT_FILE+="-linux"
fi
OUT_FILE+="-x86_64"
if [ -n "$WINDOWS" ]; then
	OUT_FILE+=".exe"
fi

if [ -n "$RELEASE" ]; then
	clang-format --dry-run -Werror src/{*.hpp,*.h,*.cpp}
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

mkdir $OUT_DIR 2>/dev/null
OUT=$OUT_DIR/$OUT_FILE
$CXX -o $OUT $MAIN_OUT \
	$INCLUDE_DIR/imgui.o \
	$INCLUDE_DIR/imgui_draw.o \
	$INCLUDE_DIR/imgui_tables.o \
	$INCLUDE_DIR/imgui_widgets.o \
	$INCLUDE_DIR/imgui_impl_glfw.o \
	$INCLUDE_DIR/imgui_impl_opengl3.o \
	$INCLUDE_DIR/imgui_stdlib.o \
	-I imgui -I imgui/backends \
	-std=c++23 $WARNINGS $LIBS
