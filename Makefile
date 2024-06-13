src:
	g++ -g -o opencv main.cpp system.cpp logger.cpp camera.cpp adc.cpp `pkg-config --cflags --libs opencv4` -pthread -lcrypto -lssl -lcpprest -lboost_system


log:
	rm Log.txt dmg_list.txt

clean:
	rm opencv 

delete_img:
	rm -rf ./images/*

prog:
	./opencv

run: clean all prog

all: src 