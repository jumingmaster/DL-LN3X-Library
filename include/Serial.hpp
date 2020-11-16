#pragma once

#include <iostream>
#include <stdlib.h> /*标准函数库定义*/
#include <unistd.h> /*Unix 标准函数定义*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>   /*文件控制定义*/
#include <termios.h> /*PPSIX 终端控制定义*/
#include <errno.h>   /*错误号定义*/
#include <string.h>
#include <string>
#include <sys/select.h>
#include <pthread.h>



class Serial
{
        friend __always_inline void closeSerial(Serial &serial);
        friend __always_inline void closeSerial(Serial *serial);
        friend void *readSerialThread(void *arg);

public:
        enum rwMode
        {
                READ,
                WRITE,
                READ_WITE
        };
        enum parityEnum
        {
                EVEN,
                ODD,
                NONE
        };

        Serial(
            std::string port, int flag, uint8_t dataBits,
            uint8_t parity, uint32_t baudrate, uint8_t stopBit);

        ~Serial() = default;

        /*      
        *       Public interface
        *       change carefully
        */
       void Open (void);
        /* flagSet
       *  @func       file flag set
       *  @pram       open flag  
       */
        void flagSet(int rwFlag);

        /* dataBitSet
        *  @func        set the usart data bit
        *  @pram        CS7, CS8
        */
        void dataBitSet(uint8_t dataBit);

        /*
        *       PA_NONE,  EVEN,  ODD
        */
        void paritySet(uint8_t parity);

        void baudrateSet(uint32_t budrate);

        void stopBitsSet(uint8_t stopBit);

        size_t getSendBytes(void) { return sendBytes; }

        size_t getRecvBytes(void) { return recvBytes; }

        void showRecvBytes (void) 
        {
                pthread_mutex_lock (&lock);
                std::cout << "RX: " << getRecvBytes () << std::endl;
                pthread_mutex_unlock (&lock);
        } 

        void showWriteBytes (void)
        {
                pthread_mutex_lock (&lock);
                std::cout << "TX: " << getSendBytes () << std::endl;
                pthread_mutex_unlock (&lock);
        }
        /*
        *       Please change under function by yourself 
        *       if you need or if extend
        */
        virtual void readSerialHex(void *buf, ssize_t *recvSize);

        virtual void readSerial(unsigned char *buf, ssize_t *readBytes);

        virtual void writeSerial(void *buf);

        virtual void writeSerial(unsigned char *buf, size_t len);

private:
        std::string portName;
        int fd;
        int flag;
        struct termios tio;
        void *data;
        static size_t sendBytes;
        static size_t recvBytes;
        static pthread_mutex_t lock;
        pthread_t readTid;
        //void * yourDataType;
};

__always_inline void closeSerial(Serial &serial)
{
        using namespace std;

        if (close(serial.fd) == -1)
        {
                perror("When close serial");
                exit(EXIT_FAILURE);
        }

        if (pthread_cancel(serial.readTid) == -1)
        {
                perror("ReadThread canel failed");
                exit(EXIT_FAILURE);
        }

        cout << serial.portName << "closed." << endl;
}

__always_inline void closeSerial(Serial *serial)
{
        using namespace std;

        if (close(serial->fd) == -1)
        {
                perror("When close serial");
                exit(EXIT_FAILURE);
        }

        cout << serial->portName << "closed." << endl;
}