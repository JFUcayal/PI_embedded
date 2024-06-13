#include "system.h"

using namespace std;



static void damage_list();

/// @brief System Constructor
System::System(){
    startup();
}

/// @brief System Destructor
System::~System(){
    
}

/// @brief System shutdown
void System::shutdown(){

    main_logger.close_logger();
    dmg_arquive.close();
}

/// @brief System Init
void System::startup(){

    //Load i2c modules
    system("modprobe i2c-dev");
    system("modprobe i2c-bcm2835");

    //Get photo_index for image ID registration
    photo_index = 1;
    //photo_index = get_photo_index();

    cout << "Hello system!" << endl;

    //Main & damage_list log opening
    main_logger.open_logger();   
    dmg_arquive.open("dmg_list.txt", ios::out | ios::app | ios::ate);

    //Startup message sent to main logger
    string startup_msg = ("System Startup @");
    startup_msg = append_timestamp(startup_msg);
    main_logger.log_write_info(startup_msg);

    //Connect to server 
    connect_server();
} 

/// @brief Append timestamp to a message string
/// @param msg string with the original message
/// @return Concatenated message -> Original message + Timestamp
string System::append_timestamp(string msg){

    string concatenated_msg = msg;

    //get time
    current_time = get_time();
    tm *tm_info = localtime(&current_time);

    //give the correct format
    string time_msg;
    time_msg = get_time_format(tm_info);

    //Append message info with timestamp
    concatenated_msg += " " + time_msg;
    
    return concatenated_msg;
}

/// @brief Formats the current time to "Day-Month-Year Hour-Minute-Second" 
/// @param time_info -> Current time fetched in get_time() function
/// @return Time formated string
string System::get_time_format(tm *time_info){

    char sampleTimeStr[20]; 
    strftime(sampleTimeStr, sizeof(sampleTimeStr), "%d-%m-%Y %H:%M:%S", time_info); 

   string time_string(sampleTimeStr);

    return time_string;
}

/// @brief Retrieves current time
/// @return Current time in time_t type
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

/// @brief Capture image using opencv functions
/// @return True -> Captured Image | False -> Camera didn't open
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

/// @brief Detect Damage in a image -> AI model output
/// @return True -> Damage detected | False -> Damage not detected
bool System::damage_detected(){

    bool dmg_detected = true;

    if(dmg_detected){

        add_damage_list();

        return true;
    }

    return false;
}

/// @brief Add timestamp of damage occured to dmg_list.txt
void System::add_damage_list(){

    string log_dmg_msg;

    log_dmg_msg = append_timestamp(log_dmg_msg);

    dmg_arquive << log_dmg_msg << endl;
    dmg_arquive.flush();

    log_dmg_msg = "Damage Detected @";
    log_dmg_msg = append_timestamp(log_dmg_msg);
    main_logger.log_write_warning(log_dmg_msg);
}

/// @brief Get LDR value from ADC
/// @return ADC value read 
uint16_t System::get_light_value(){

    uint16_t light_val;

    light_val = get_adc_value(0);

    return light_val;
}

/// @brief Establishes a connection with the node.js server via WebSockets
void System::connect_server(){

    client.connect(WEB_SOCKET_IP_ADDR).wait();

    web::json::value json_msg;

    json_msg[U("action")] = web::json::value::string(U("connection"));
    json_msg[U("type")]   = web::json::value::string(U("rasp"));

    utility::string_t dataJSON = json_msg.serialize();

    //Message to server
    websocket_outgoing_message out_msg;

    //Add the json content to the message
    out_msg.set_utf8_message(dataJSON);
    
    //Send to server
    client.send(out_msg).wait();

    sleep(3);
}