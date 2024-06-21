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
#include <wiringPi.h>

using namespace web;
using namespace web::websockets::client;

// #define WEB_SOCKET_IP_ADDR "ws://192.168.1.75:3000"
#define WEB_SOCKET_IP_ADDR "ws://10.42.0.1:3000"
#define DEVICE_PATH "/dev/gpio_irq"
#define LED_PIN 7 //GPIO4

class System{

    private:
        Logger main_logger;
        Camera camera1;
        cv::Mat photo_frame;
        time_t current_time;
        ofstream dmg_arquive;
        websocket_client client;
        static System* pSys;
        bool warning_state;

    public:
        System();
        ~System();
        void startup();
        void shutdown();
        time_t get_time(); 
        string get_time_format(tm*);
        string append_timestamp(string);
        string capture_image();
        bool damage_detect(bool);
        void connect_server();
        void send_timestamp(string);
        void add_damage_list();
        void send_ride_ok();
        void warning_led(bool);

};

#endif