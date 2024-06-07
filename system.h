#ifndef SYSTEM_H_
#define SYSTEM_H_

#include "camera.h"
#include "logger.h"
#include "string.h"
#include <string>
#include <ctime>
//#include <filesystem>
//#include <sys/types.h>
//#include <dirent.h>
#include "iostream"
#include <fstream>

class System{

    private:
        Logger main_logger;
        Camera camera1;
        cv::Mat photo_frame;
        time_t current_time;
        uint32_t photo_index;
        ofstream dmg_arquive;

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


        //Threads
        //static void* t_CameraBoot();
        //static void* t_DetectDmg();
        //static void* t_CapturePhoto();
        //static void* t_Warning();
        //static void* tDatabase(); -> comm w/ database or send info to company
        
        //bool init_threads();
        //void join_threads();
};

#endif