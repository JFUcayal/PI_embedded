#include "logger.h"

using namespace std;

Logger::Logger(){

}

Logger::~Logger(){

}

void Logger::open_logger(){

    log_file.open("Log.txt");

}

void Logger::close_logger(){
    
    log_file.close();
}

void Logger::log_write_info(string info_msg){

    log_file << "[INFO]\t  -> ";
    log_file << info_msg << "\n";
    log_file.flush();
}

void Logger::log_write_error(string error_msg){


    log_file << "[ERROR]\t  -> ";
    log_file << error_msg << "\n";
    log_file.flush();
}

void Logger::log_write_warning(string warning_msg){

    log_file << "[WARNING]\t  -> ";
    log_file << warning_msg << "\n";
    log_file.flush();

}