##
# @author Sargis S Yonan
# @date 29 September 2018
# @brief used for compiling and linking Mancala

exec_name = mancala

# using g++ for c++
cpp = g++

# for compiling c code
cc = gcc

# turn on debugging symbols
debugger = -g 

# going to compile using the C++ 2011 Standard
cpp_options = -std=c++11

# cant live with/without them...
cc_options = -Wall -Wextra

# a list of my compiled objects -- not wildcarding anything here
# to avoid any surprises
objects = main.o game.o server.o client.o game_server.o

all: build

run: build $(exec_name)
	./$(exec_name)

build: $(objects)
	$(cpp) $(cc_options) $(objects) -o $(exec_name)

main.o: main.cc game.o game_server.o
	$(cpp) $(debugger) $(cpp_options) $(cc_options) -c -I board -I game -I server main.cc

game.o: game/game.cc game/game.h board/board.h board/hole.h
	$(cpp) $(debugger) $(cpp_options) $(cc_options) -c -I board -I game game/game.cc

game_server.o: server.o client.o server/game_server.cc server/game_server.h
	$(cpp) $(debugger) $(cpp_options) $(cc_options) -c -I board -I server server/game_server.cc

server.o: server/server.h server/server.c
	$(cc) $(debugger) $(cc_options) $(cc_options) -c -I server server/server.c

client.o: server/client.h server/client.c
	$(cc) $(debugger) $(cc_options) -c -I server server/client.c

gdb: build $(exec_name)
	sudo gdb ./$(exec_name)

valgrind: build $(exec_name)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./$(exec_name) $(test_filename)

clean:
	rm -rf $(objects) $(exec_name)*