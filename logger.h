#ifndef LOGGER_H_
#define LOGGER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <cstring>


class Logger {

    private:
        FILE *file_ptr;
        const char* file_name = "Log.txt";

    public:
        Logger();
        ~Logger();
        void log_write_info();
        void log_write_error();
        void log_write_warning();
        
};

#endif