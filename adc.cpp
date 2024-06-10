#include <iostream>
#include "adc.h"

using namespace std;

//Single Conversion + Mux_default + Gain = 1.024V + Pwr-down single-shot + data_rate default + COMP default
//#define ADC_DEFAULT_CMD 0b1100 0101 1000 0011

//Single Conversion + Mux:100 -> AIN0 + Gain = 4.096V + Pwr-down single-shot + data_rate default + COMP default
#define ADC_DEFAULT_CMD 0b1100001110000011

const char* i2c_device_adc = "/dev/i2c-1";
int i2c_file_adc;          //file descriptor -> open i2c file in Read/Write mode

const int ADS1115_CONV_REG = 0x00;
const int ADS1115_CONFIG_REG = 0x01;
const int ADS1115_ADDR = 0x48;

void init(){

    i2c_file_adc = open(i2c_device_adc, O_RDWR);

       if(i2c_file_adc == -1){
        cerr << "#############################" << endl;
        cerr << "Error opening i2c file!" << endl;
        return;
    }

    if (ioctl(i2c_file_adc, I2C_SLAVE, ADS1115_ADDR) < 0){
        cerr << "#############################" << endl;
        cerr << ("Error in i2c device!");
        close(i2c_file_adc);
        return;
    } 
}

uint16_t get_adc_value(uint8_t channel){

    uint16_t adc_value, config_CH;
    uint8_t msb_config, lsb_config;
    

    //Input MUX[2:0] for mask -> bits[14:12] in config REG
    uint8_t channel_array[4]={0b100, 0b101, 0b110, 0b111};

    //select channel -> MUX[14:12]
    config_CH = ADC_DEFAULT_CMD | ((channel_array[channel]) << 12);

    //shift one byte to the right to get MSB value
    msb_config = static_cast<uint8_t>(config_CH >> 8);
    //Logic AND with 8 bits with value=1(0xFF) to get LSB value
    lsb_config = static_cast<uint8_t>(config_CH && 0xFF);


    //i²c init
    init();


    //write command for configuration of ADC
    uint8_t config_cmd[3]={ADS1115_CONFIG_REG, msb_config, lsb_config};

    if (write(i2c_file_adc, &config_cmd, 3) != 3) {       
        cerr << "#############################" << endl;
        cerr << "Error in setup!" << endl;
        close(i2c_file_adc);
        return 0;       
    }

    usleep(I2C_DELAY);

    //read ADC value -> CONVERSION REG -> get 16-bit value
    if (write(i2c_file_adc, &ADS1115_CONV_REG, 1) != 1) {       
        cerr << "#############################" << endl;
        cerr << "Error in setup!" << endl;
        close(i2c_file_adc);
        return 0;       
    }
    
    usleep(I2C_DELAY);

    uint8_t adc_buff[2];

    if (read(i2c_file_adc, adc_buff, 2) != 2){         //ERR -> 2 bytes not returned
        cerr << "#############################" << endl;
        cerr << "Error reading ADC data!" << endl;
        close(i2c_file_adc);
        return 0;
    }

    close(i2c_file_adc);

    adc_value = (adc_buff[0] << 8 | adc_buff[1]);

    if(adc_value < 100 || adc_value > 32767){
        adc_value = 1;
    }

    //percentage conversion
    adc_value = ((adc_value * 100)/26000);

    //cout << "ADC value -> " << adc_value << " % " << endl;

    return adc_value;
}