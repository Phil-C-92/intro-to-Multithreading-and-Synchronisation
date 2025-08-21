all: mmcopier

clean:
	rm -f mmcopier *.o 

mmcopier: mmcopier.o
	g++ -Wall -Werror -std=c++17 -g -O -o $@ $^ -lpthread

%.o: %.cpp
	g++ -Wall -Werror -std=c++17 -g -O -c $^