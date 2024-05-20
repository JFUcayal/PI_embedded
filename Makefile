src:
	g++ -o opencv main.cpp system.cpp logger.cpp camera.cpp

log:
	rm Log.txt

clean:
	rm opencv

prog:
	./opencv

run: clean all prog

all: src 