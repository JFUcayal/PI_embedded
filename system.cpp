#include "system.h"
#include <ctime>
#include <iostream>


using namespace std;


System::System(){
    startup();
}

System::~System(){
    
}

void System::shutdown(){

    main_logger.close_logger("Log.txt");

}

void System::startup(){

    cout << "Hello system!" << endl;

    main_logger.open_logger("Log.txt");   

    const char* startup_msg = ("System Startup @ ");
    
    startup_msg = append_timestamp(startup_msg);

    main_logger.log_write_info(startup_msg);
} 

char* System::append_timestamp(const char* msg){

    char* concatenated_msg;

    //get time
    current_time = get_time();
    tm *tm_info = localtime(&current_time);

    //give the correct format
    const char* time_msg;
    time_msg = get_time_format(tm_info);

    concatenated_msg = (char*) malloc(strlen(msg) + strlen(time_msg));

    strcpy(concatenated_msg, msg);
    strcat(concatenated_msg, time_msg);

    return concatenated_msg;
}

char* System::get_time_format(tm *time_info){

    char sampleTimeStr[20]; 
    strftime(sampleTimeStr, sizeof(sampleTimeStr), "%d-%m-%Y %H:%M:%S", time_info); 
    
    cout << sampleTimeStr << endl;

    char* time_string = (char*)malloc((strlen(sampleTimeStr) + 1) * sizeof(char));

    if (time_string == NULL) 
    {
        cerr << "Error allocating memory in time string!" << std::endl;
        return NULL;
    }

    strncpy(time_string ,sampleTimeStr, sizeof(sampleTimeStr));

    return time_string;
}

time_t System::get_time(){

    time_t curr_time;
    time(&curr_time);

    return curr_time;
}

bool System::capture_image(/*timestamp*/){
    
    const char* folder_path = "./images/";
    folder_path = append_timestamp(folder_path); 

    char* image_path = (char*) malloc(strlen(folder_path) + strlen(".jpg"));

    strcpy(image_path, folder_path);
    strcat(image_path,".jpg");

    if(!camera1.open_camera())
    {
        main_logger.log_write_error("Camera Didn't Opened!");
        return false;
    }

    if(camera1.capture_image(photo_frame))
    {
        
        if(camera1.save_image(image_path, photo_frame))
        {
            main_logger.log_write_info("Image Captured & Saved!"); /*Add Path*/    
        }
        else
        {
            main_logger.log_write_error("Error Saving Image!");
        }
    }
    else
    {
        main_logger.log_write_error("Error Capturing Image!");
    }


    camera1.close_camera();
    return true;
}