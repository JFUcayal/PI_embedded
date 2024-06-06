#include "logger.h"

using namespace std;

Logger::Logger(){

}

Logger::~Logger(){

}

void Logger::open_logger(const char* file_name){

    file_ptr = fopen(file_name, "w");

    if(file_ptr == NULL)
    {
        printf("Error opening logger file!\n");
    }
}

void Logger::close_logger(const char* file_name){
    
    fclose(file_ptr);
}

void Logger::log_write_info(const char* info_msg){

    if(file_ptr == NULL)
    {
        printf("Error in logger file!\n");
    }

    fprintf(file_ptr, "[INFO]\t  -> ");
    fprintf(file_ptr, "%s",info_msg);
    fprintf(file_ptr, "\n");

}

void Logger::log_write_error(const char* error_msg){

    if(file_ptr == NULL)
    {
        printf("Error in logger file!\n");
    }

    fprintf(file_ptr, "[ERROR]   -> ");
    fprintf(file_ptr, "%s",error_msg);
    fprintf(file_ptr, "\n");
}

void Logger::log_write_warning(const char* warning_msg){

    if(file_ptr == NULL)
    {
        printf("Error in logger file!\n");
    }

    fprintf(file_ptr, "[WARNING] -> ");
    fprintf(file_ptr, "\t%s",warning_msg);
    fprintf(file_ptr, "\n");

}