set -x

CFLAGS=$(pkg-config --cflags glfw3)
CXX=g++

$CXX -std=c++23 -I../lib/imgui -I../lib/imgui/backends -g -Wall -Wformat $CFLAGS -c -o imgui.o ../lib/imgui/imgui.cpp
$CXX -std=c++23 -I../lib/imgui -I../lib/imgui/backends -g -Wall -Wformat $CFLAGS -c -o imgui_demo.o ../lib/imgui/imgui_demo.cpp
$CXX -std=c++23 -I../lib/imgui -I../lib/imgui/backends -g -Wall -Wformat $CFLAGS -c -o imgui_draw.o ../lib/imgui/imgui_draw.cpp
$CXX -std=c++23 -I../lib/imgui -I../lib/imgui/backends -g -Wall -Wformat $CFLAGS -c -o imgui_tables.o ../lib/imgui/imgui_tables.cpp
$CXX -std=c++23 -I../lib/imgui -I../lib/imgui/backends -g -Wall -Wformat $CFLAGS -c -o imgui_widgets.o ../lib/imgui/imgui_widgets.cpp
$CXX -std=c++23 -I../lib/imgui -I../lib/imgui/backends -g -Wall -Wformat $CFLAGS -c -o imgui_impl_glfw.o ../lib/imgui/backends/imgui_impl_glfw.cpp
$CXX -std=c++23 -I../lib/imgui -I../lib/imgui/backends -g -Wall -Wformat $CFLAGS -c -o imgui_impl_opengl3.o ../lib/imgui/backends/imgui_impl_opengl3.cpp
