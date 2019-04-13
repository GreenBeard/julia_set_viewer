
viewer: main.o julia.o
	g++ main.o julia.o $(shell wx-config --libs) -lm -fopenmp

main.o: main.cpp
	g++ -c -g main.cpp $(shell wx-config --cxxflags)

julia.o: julia.c
	gcc -c -g julia.c -lm -fopenmp

clean:
	rm a.out

