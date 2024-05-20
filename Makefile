src:
	g++ -o opencv main.cpp system.cpp logger.cpp

clean:
	rm opencv

prog:
	./opencv

run: clean all prog

all: src