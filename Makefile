src:
	g++ -g -o opencv main.cpp system.cpp logger.cpp camera.cpp `pkg-config --cflags --libs opencv4`

log:
	rm Log.txt

clean:
	rm opencv

prog:
	./opencv

run: clean all prog

all: src 