#include <iostream>
#include <unistd.h> 
#include <pthread.h>
#include <sched.h>
#include <mqueue.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>
#include <sys/wait.h>

#include "system.h"
#include "logger.h"

using namespace std;

int main(){

    System main_system;

    while(1){

        //system("libcamera-still -o ./images/test0.jpg");
        //sleep(5);

        main_system.capture_image();
        sleep(2);
        main_system.damage_detected();
        sleep(2);
        main_system.get_light_value();
    }

    return 0;
}