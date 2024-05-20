#include "logger.h"

using namespace std;

Logger::Logger(){

}

Logger::~Logger(){

}

void Logger::log_write_info(){
    
    file_ptr = fopen(file_name, "a");

    fprintf(file_ptr, "*** \t LOGGER INFO \t ***\n");



    fclose(file_ptr);
}

void Logger::log_write_error(){
    
    file_ptr = fopen(file_name, "a");

    fprintf(file_ptr, "*** \t LOGGER ERROR \t ***\n");



    fclose(file_ptr);
}

void Logger::log_write_warning(){

    file_ptr = fopen(file_name, "a");

    fprintf(file_ptr, "*** \t LOGGER WARNING \t ***\n");



    fclose(file_ptr);
}