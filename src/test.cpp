#include "Serial.hpp"
#include "DL-LN3X.h"

using namespace std;

dl_ln3x_t init_local_dev (Serial &usart, dl_ln3x &device);

int main(int argc, char **argv)
{
        uint8_t data[] = {0xfe, 0x05, 0x90, 0x21, 0x00, 0x00, 0x01, 0xff};
        uint8_t blink[] = {0xfe, 0x05, 0x91, 0x21, 0x00, 0x00, 0x02, 0xff};

        Serial serial (argv[1], Serial::READ_WITE, 8, Serial::NONE, 115200, 1);
        dl_ln3x local_dev;

        
        while (1)
        {
                
                serial.writeSerial ((unsigned char*)GET_INFO_CMD[ADDR_CMD], sizeof (GET_INFO_CMD[ADDR_CMD]));
                
                for (int i = 0; i < 200000; i++)
                        for (int j = 0; j < 10000; j++);
                        
                serial.writeSerial ((unsigned char*)GET_INFO_CMD[NID_CMD], sizeof (GET_INFO_CMD[NID_CMD]));

                for (int i = 0; i < 200000; i++)
                        for (int j = 0; j < 10000; j++);

                serial.writeSerial ((unsigned char*)GET_INFO_CMD[CHL_CMD], sizeof (GET_INFO_CMD[CHL_CMD]));

                for (int i = 0; i < 200000; i++)
                        for (int j = 0; j < 10000; j++);

                serial.writeSerial ((unsigned char*)GET_INFO_CMD[BUAD_CMD], sizeof (GET_INFO_CMD[BUAD_CMD]));   
                 
                for (int i = 0; i < 200000; i++)
                        for (int j = 0; j < 10000; j++);
                   
        }

        closeSerial (serial);
        return 0;
}

/*
dl_ln3x_t init_local_dev (Serial &usart, dl_ln3x &device)
{
        uint8_t buf[32];
}
*/