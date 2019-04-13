
viewer: main.o julia.o
	g++ main.o julia.o -o viewer $(shell wx-config --libs) -lm -fopenmp

main.o: main.cpp
	g++ -O2 -c -g main.cpp $(shell wx-config --cxxflags)

julia.o: julia.c
	gcc -O2 -c -g julia.c -lm -fopenmp

clean:
	$(RM) julia.o main.o viewer

