all: mmcopier mscopier

clean:
	rm -f mmcopier *.o 
	rm -f mscopier *.o

mmcopier: mmcopier.o
	g++ -Wall -Werror -std=c++11 -g -O -o $@ $^ -lpthread

mscopier: mscopier.o
	g++ -Wall -Werror -std=c++11 -g -O -o $@ $^ -lpthread

%.o: %.cpp
	g++ -Wall -Werror -std=c++11 -g -O -c $^


