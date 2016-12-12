@all:
	g++ main.cpp -o main -std=c++11 -Wwrite-strings
	g++ cubic.cpp -o cubic -std=c++11 -Wwrite-strings
	g++ gen.cpp -o gen -std=c++11 -Wwrite-strings
	g++ render.cpp -o render -std=c++11 -lglfw -lGLEW -lGL -lGLU -Wwrite-strings
