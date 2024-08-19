compile: ./src/*.cpp
	g++ -std=c++20 -I./include -lncurses ./src/*.cpp -o ./bin/program

run: ./bin/program
	./bin/program
