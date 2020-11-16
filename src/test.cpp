#include "Serial.hpp"

using namespace std;

int main(int argc, char **argv)
{
        unsigned char data[] = {0xfe, 0x05, 0x90, 0x90, 0x65, 0x62, 0x01, 0xff};
        unsigned char blink[] = {0xfe, 0x05, 0x91, 0x91, 0x65, 0x62, 0x32, 0xff};

        Serial serial (argv[1], Serial::READ_WITE, 8, Serial::NONE, 115200, 1);
        


        while (1)
        {
               /* 
                serial.writeSerial (data, sizeof (data));
                
                for (int i = 0; i < 200000; i++)
                        for (int j = 0; j < 10000; j++);
                        
                serial.writeSerial (blink, sizeof (blink));

                for (int i = 0; i < 200000; i++)
                        for (int j = 0; j < 10000; j++);   
                 
                 */       
        }
        closeSerial (serial);
        return 0;
}
