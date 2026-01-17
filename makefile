CXX = g++
CXXFLAGS = -g -Wall -Werror -Wextra -lsfml-graphics -lsfml-window -lsfml-system
STRING = "C:\Users\roblo\Downloads\library\SFML-2.5.1\include"
obj = main.o welcome.o tile.o leaderboard.o board.o

main: $(obj)
	$(CXX)  $^ -o $@ -L"C:\Users\roblo\Downloads\library\SFML-2.5.1\lib" $(CXXFLAGS)
%.o: %.cpp window.hpp
	g++ -c $< -I$(STRING)
tictactoe: tictactoe.o
	$(CXX) $< -o $@ -L"C:\Users\roblo\Downloads\library\SFML-2.5.1\lib" $(CXXFLAGS)
.PHONY: clean tic r
clean: 
	rm *.o main tictactoe
tic: tictactoe
	./tictactoe
r: main
	./main