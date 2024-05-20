#ifndef SYSTEM_H_
#define SYSTEM_H_

#include "camera.h"
#include "logger.h"

class System{

    private:
        Logger main_logger;
        Camera camera1;

    public:
        System();
        ~System();
        void startup();
        void shutdown();
        //time_t get_time(); //get current time 
        

        //Threads
        static void* t_CameraBoot();
        static void* t_DetectDmg();
        static void* t_CapturePhoto();
        static void* t_Warning();
        //static void* tDatabase(); -> comm w/ database or send info to company
        
        bool init_threads();
        void join_threads();
};

#endif