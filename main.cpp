#include <iostream>
#include "system.h"
#include "logger.h"
#include <unistd.h> 

using namespace std;

int main(){

    System main_system;

    while(1){

        main_system.capture_image();
        //sleep(2);
        main_system.damage_detected();
        sleep(2);

    }

    return 0;
}