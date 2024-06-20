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
#include <wiringPi.h>
#include <sys/time.h>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <dirent.h>
#include <cstdio>

#include "system.h"
#include "logger.h"

using namespace std;

#define PIN 11 //GPIO17

#define DEBOUNCE_TIME 1000000 // 1 second Delay in debounce

struct timeval last_interrupt_time;

int last_state = LOW;

//function prototypes
static int go_to_directory(const char *);
static bool read_file(const char *);
void handle();

System main_system;

int main(){

    if (wiringPiSetupPhys() == -1) {
        cerr << "Error initialing WiringPi" << endl;
        return 1;
    }

    pinMode(PIN, INPUT);

    gettimeofday(&last_interrupt_time, NULL);
    last_state = digitalRead(PIN);

    //Rising edge interrup
    if (wiringPiISR(PIN, INT_EDGE_RISING, &handle) < 0) {
        cerr << "Error configuring interrupt" << endl;
        return 1;
    }

        cout << "Programme started! Waiting for interruptions..." << endl;

    while (true) {
        this_thread::sleep_for(chrono::seconds(1));

    }
    return 0;
}



void run_system_command_and_process() {

    string sys_cmd_string;
    
    string img_name = main_system.capture_image();

    bool dmg_detected = false;
    
    if(img_name != "ERROR"){
        
        string img_path = "./images/" + img_name + ".jpg";

        sys_cmd_string = "yolo predict source=" + img_path + " conf=0.3 show=True save_txt=True save_conf=True model=best.onnx";

        const char * command = sys_cmd_string.c_str();
        system(command);

        if (!go_to_directory("runs")) return;
        if (!go_to_directory("detect")) return;
        if (!go_to_directory("predict")) return;
        if (!go_to_directory("labels")) return;

        string img_content = img_name + ".txt";
        const char * img = img_content.c_str();

        //Detect damage in bounding boxes .txt file  
        dmg_detected = read_file(img);
        main_system.damage_detect(dmg_detected);

        chdir("../../../.."); 
        system("rm -r runs");
    } 
}


void handle() {
    struct timeval now;
    gettimeofday(&now, NULL);

    unsigned long diff = (now.tv_sec - last_interrupt_time.tv_sec) * DEBOUNCE_TIME + (now.tv_usec - last_interrupt_time.tv_usec);

    int current_state = digitalRead(PIN);

    if (diff > DEBOUNCE_TIME && current_state != last_state) {

        std::cout << "Interrupt detected!" << std::endl;

        std::thread(run_system_command_and_process).detach();

        last_interrupt_time = now;
        last_state = current_state;
    }
}

// Função para verificar se o diretório existe e entrar nele
int go_to_directory(const char *directory_name) {

    cout << "Trying to get into the directory: " << directory_name << endl;

    DIR *dir = opendir(directory_name);

    if (dir == NULL) {
        perror("Could not open the directory");
        return 0;
    }
    closedir(dir);

    if (chdir(directory_name) != 0) {
        perror("Could not enter the directory");
        return 0;
    }

    return 1;
}

bool read_file(const char *file_name) {

    FILE *arquivo = fopen(file_name, "r");

    if (arquivo == NULL) {
        perror("Could not open file");
        return false;
    }

    char linha[256];
    while (fgets(linha, sizeof(linha), arquivo)) {
        cout << linha;
    }

    fclose(arquivo);

    return true;
}