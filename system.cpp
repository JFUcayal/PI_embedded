#include "system.h"

using namespace std;

static void damage_list();

System* System::pSys = nullptr;

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
    //system("modprobe i2c-dev");
    //system("modprobe i2c-bcm2835");

    cout << "Hello system!" << endl;

    //WARNING LED INIT
    // WiringPi init
    if (wiringPiSetup() == -1) {
        cout << "Error initializing LED" << endl;
        return;
    }

    pinMode(LED_PIN, OUTPUT);

    warning_state = false;
    warning_led(warning_state);


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
    concatenated_msg += "_" + time_msg;
    
    return concatenated_msg;
}

/// @brief Formats the current time to "Day-Month-Year Hour-Minute-Second" 
/// @param time_info -> Current time fetched in get_time() function
/// @return Time formated string
string System::get_time_format(tm *time_info){

    char sampleTimeStr[20]; 
    strftime(sampleTimeStr, sizeof(sampleTimeStr), "%d-%m-%Y_%H:%M:%S", time_info); 

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

/// @brief Capture image using opencv functions
/// @return True -> Captured Image | False -> Camera didn't open
string System::capture_image(){

    string index_char;
    string log_msg;
    string return_name;

    if(!camera1.open_camera())
    {
        log_msg = "Camera Didn't Opened @";
        log_msg = append_timestamp(log_msg);

        main_logger.log_write_error(log_msg);
        return "ERROR";
    }

    if(camera1.capture_image(photo_frame))
    {
        return_name = append_timestamp(return_name);
        
        // ./images/_xx-xx-xxxx xx:xx:xx.jpg
        string image_path = "./images/" + return_name + ".jpg";

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
    return return_name;
}

/// @brief Detect Damage in a image -> AI model output
/// @return True -> Damage detected | False -> Damage not detected
bool System::damage_detect(bool dmg_detected){

    if(dmg_detected){
        //Add timestamp to dmg_list.txt + Send to server
        add_damage_list();

        warning_state = true;
        warning_led(warning_state);

        cout << "---> DAMAGE DETECTED " << endl;

        return true;
    }

    //damage not detected + Send to server

    warning_state = false;
    warning_led(warning_state);

    cout << "---> DAMAGE NOT DETECTED " << endl;
    send_ride_ok();

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

    //Send to the server
    send_timestamp(log_dmg_msg);
}

/// @brief Register ride without any damage detected & Send data to server
void System::send_ride_ok(){

    string log_no_dmg_msg;

    log_no_dmg_msg = append_timestamp(log_no_dmg_msg);

    log_no_dmg_msg = "No Damage Detected @";
    log_no_dmg_msg = append_timestamp(log_no_dmg_msg);
    main_logger.log_write_info(log_no_dmg_msg);

    //Send to the server
    send_timestamp(log_no_dmg_msg);
}

/// @brief Establishes a connection with the node.js server via WebSockets
void System::connect_server(){

    client.connect(WEB_SOCKET_IP_ADDR).wait();

    string connect_msg = "Connected to the server @";
    connect_msg = append_timestamp(connect_msg);
    main_logger.log_write_info(connect_msg);

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
}

void System::send_timestamp(string timestamp_msg){

    //Send data to server
    try {
        // JSON msg to send
        web::json::value json_msg;

        json_msg[U("action")] = web::json::value::string(U("update_data"));

        json_msg[U("time")] = web::json::value::string(timestamp_msg);

        utility::string_t dataJSON = json_msg.serialize();

        // Send msg to server
        websocket_outgoing_message out_msg;

        //Add the json content to the message
        out_msg.set_utf8_message(dataJSON);

        //Send to server
        client.send(out_msg).wait();

        string time_msg = "Message sent to server @";
        time_msg = append_timestamp(time_msg);
        main_logger.log_write_info(time_msg);


        } catch (const web::websockets::client::websocket_exception& e) {

            string error_msg = "Error sending message to server @";
            error_msg = append_timestamp(error_msg);
            main_logger.log_write_error(error_msg);
            
            cerr << "Error sending message: " << e.what() << endl;

        } catch (const std::exception& e) {

            string error_msg = "Unknown error @";
            error_msg = append_timestamp(error_msg);
            main_logger.log_write_error(error_msg);

            cerr << "Erro desconhecido: " << e.what() << endl;

        }   
}

void System::warning_led(bool state){

    if (state == true){
        //Turn ON warning LED
        digitalWrite(LED_PIN, HIGH);
    } else {
        //Turn OFF warning LED 
        digitalWrite(LED_PIN, LOW);
    }
}