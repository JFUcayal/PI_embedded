#include "system.h"
#include <iostream>

using namespace std;

System::System(){
    startup();
}

System::~System(){

}

void System::startup(){
    Logger Logger_1;

     cout << "Hello system!" << endl;

    Logger_1.log_write_info();
    Logger_1.log_write_error();
    Logger_1.log_write_warning();
} 