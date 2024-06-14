#ifndef SYSTEM_H_
#define SYSTEM_H_

#include "camera.h"
#include "logger.h"
#include "string.h"
#include <string>
#include <ctime>
#include "iostream"
#include <fstream>
#include "adc.h"
#include <cpprest/ws_client.h>
#include <cpprest/json.h>
#include <iomanip>

//#include <filesystem>
//#include <sys/types.h>
//#include <dirent.h>

using namespace web;
using namespace web::websockets::client;

#define WEB_SOCKET_IP_ADDR "ws://192.168.1.75:3000"
#define DEVICE_PATH "/dev/gpio_irq"


class System{

    private:
        Logger main_logger;
        Camera camera1;
        cv::Mat photo_frame;
        time_t current_time;
        uint32_t photo_index;
        ofstream dmg_arquive;
        uint16_t light_value;
        websocket_client client;
        static System* pSys;

    public:
        System();
        ~System();
        void startup();
        void shutdown();
        time_t get_time(); 
        string get_time_format(tm*);
        string append_timestamp(string);
        //uint32_t get_photo_index();
        bool capture_image();
        bool damage_detected();
        void add_damage_list();
        uint16_t get_light_value();
        void connect_server();
        void send_timestamp(string);
        static void signal_handler(int);

        //Threads
        //static void* t_CapturePhoto();
        //static void* t_SendToServer();
        
        //bool init_threads();
        //void join_threads();
};

#endif