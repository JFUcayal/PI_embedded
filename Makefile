src:
	g++ -g -o opencv main.cpp system.cpp logger.cpp camera.cpp adc.cpp `pkg-config --cflags --libs opencv4` -pthread -lcrypto -lssl -lcpprest -lboost_system

cross:
	aarch64-linux-gnu-g++ -g -o opencv main.cpp system.cpp logger.cpp camera.cpp adc.cpp `pkg-config --cflags --libs opencv4` -lpthread -pthread -lcrypto -lssl -lcpprest -lboost_system -lwiringPi

log:
	rm Log.txt dmg_list.txt

clean:
	rm opencv 

delete_img:
	rm -rf ./images/*

prog:
	./opencv

run: clean all prog

mbed: cross prog

all: src 