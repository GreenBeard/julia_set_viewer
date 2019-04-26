#CC=x86_64-w64-mingw32-gcc
#CXX=x86_64-w64-mingw32-g++

#WX_CONFIG=/opt/mingw_libs/wxwidgets/lib/wx/config/inplace-x86_64-w64-mingw32-msw-unicode-3.1
WX_CONFIG=wx-config
WX_FLAGS=$(shell $(WX_CONFIG) --cxxflags) -Dcomplex=_Complex
WX_LIBS=$(shell $(WX_CONFIG) --libs)

#finishing-touches: bin/viewer
#	./terrible_windows_setup.sh

bin/viewer: build/main.o build/julia.o
	$(CXX) build/main.o build/julia.o -o bin/viewer $(WX_LIBS) -lm -fopenmp

build/main.o: src/main.cpp
	$(CXX) -O2 -c -g src/main.cpp -o build/main.o -lm $(WX_FLAGS)

build/julia.o: src/julia.c
	$(CC) -O2 -c -g src/julia.c -o build/julia.o -lm -fopenmp

clean:
	$(RM) build/* bin/*

