#ifndef LOGGER_H_
#define LOGGER_H_

#include <string>
#include <iostream>
#include <fstream>

using namespace std;

class Logger {

    private:
        ofstream log_file;
        string log_msg;

    public:
        Logger();
        ~Logger();
        void open_logger();
        void close_logger();
        void log_write_info(string);
        void log_write_error(string);
        void log_write_warning(string);
        
};

#endif