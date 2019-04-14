#CC=x86_64-w64-mingw32-gcc
#CXX=x86_64-w64-mingw32-g++

#WX_CONFIG=/opt/mingw_libs/wxwidgets/lib/wx/config/inplace-x86_64-w64-mingw32-msw-unicode-3.1
WX_CONFIG=wx-config
WX_FLAGS=$(shell $(WX_CONFIG) --cxxflags)
WX_LIBS=$(shell $(WX_CONFIG) --libs)

#finishing-touches: viewer
#	./terrible_windows_setup.sh

viewer: main.o julia.o
	$(CXX) main.o julia.o -o viewer $(WX_LIBS) -lm -fopenmp

main.o: main.cpp
	$(CXX) -O2 -c -g main.cpp $(WX_FLAGS)

julia.o: julia.c
	$(CC) -O2 -c -g julia.c -lm -fopenmp

clean:
	$(RM) julia.o main.o viewer

