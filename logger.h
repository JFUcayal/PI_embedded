#ifndef LOGGER_H_
#define LOGGER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <cstring>


class Logger {

    private:
        FILE *file_ptr;
        //const char* file_name = "Log.txt";
        const char* log_msg;

    public:
        Logger();
        ~Logger();
        void open_logger(const char*);
        void close_logger(const char*);
        void log_write_info(const char*);
        void log_write_error(const char*);
        void log_write_warning(const char*);
        
};

#endif