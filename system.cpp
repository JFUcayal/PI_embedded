#include "system.h"


using namespace std;

static void damage_list();

System::System(){
    startup();
}

System::~System(){
    
}

void System::shutdown(){

    main_logger.close_logger();
    dmg_arquive.close();
}

void System::startup(){

    photo_index = 1;

    cout << "Hello system!" << endl;

    main_logger.open_logger();   

    dmg_arquive.open("dmg_list.txt", ios::out | ios::app | ios::ate);

    string startup_msg = ("System Startup @");
    startup_msg = append_timestamp(startup_msg);

    main_logger.log_write_info(startup_msg);

    //photo_index = get_photo_index();
} 

string System::append_timestamp(string msg){

    //get time
    current_time = get_time();
    tm *tm_info = localtime(&current_time);

    //give the correct format
    string time_msg;
    time_msg = get_time_format(tm_info);

    string concatenated_msg = msg;

    concatenated_msg += " " + time_msg;
    
    return concatenated_msg;
}

string System::get_time_format(tm *time_info){

    char sampleTimeStr[20]; 
    strftime(sampleTimeStr, sizeof(sampleTimeStr), "%d-%m-%Y %H:%M:%S", time_info); 

   string time_string(sampleTimeStr);

    return time_string;
}

time_t System::get_time(){

    time_t curr_time;
    time(&curr_time);

    return curr_time;
}

/*
uint32_t System::get_photo_index(){

    static uint32_t counter = 0;



    return counter;
}
*/

bool System::capture_image(){

    string index_char;
    string folder_path = "./images/";
    string log_msg;

    if(!camera1.open_camera())
    {
        log_msg = "Camera Didn't Opened @";
        log_msg = append_timestamp(log_msg);

        main_logger.log_write_error(log_msg);
        return false;
    }

    if(camera1.capture_image(photo_frame))
    {
        index_char = to_string(photo_index);

        folder_path += index_char;

        folder_path = append_timestamp(folder_path); 

        //append .jpg
        string image_path = folder_path + ".jpg";

        photo_index++;

        if(camera1.save_image(image_path, photo_frame))
        {
            log_msg = "Image Captured & Saved @";
            log_msg = append_timestamp(log_msg);

            main_logger.log_write_info(log_msg);   
        }
        else
        {
            log_msg = "Error Saving Image @";
            log_msg = append_timestamp(log_msg);
            main_logger.log_write_error(log_msg);
        }
    }
    else
    {
        log_msg = "Error Capturing Image @";
        log_msg = append_timestamp(log_msg);

        main_logger.log_write_error(log_msg);
    }

    camera1.close_camera();
    return true;
}


bool System::damage_detected(){

    bool dmg_detected = true;

    if(dmg_detected){

        add_damage_list();

        return true;
    }

    return false;
}

void System::add_damage_list(){

    string log_dmg_msg;

    log_dmg_msg = append_timestamp(log_dmg_msg);

    dmg_arquive << log_dmg_msg << endl;
    dmg_arquive.flush();
}