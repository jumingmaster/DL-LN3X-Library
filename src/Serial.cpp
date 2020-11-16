#include "Serial.hpp"

pthread_mutex_t Serial::lock = PTHREAD_MUTEX_INITIALIZER;
size_t Serial::sendBytes = 0;
size_t Serial::recvBytes = 0;

static void showSendMsg(void *buf, ssize_t len)
{
        const unsigned char *data = (unsigned char *)buf;

        printf("send: ");
        for (int i = 0; i < len; i++)
        {
                if (data[i] < 0x10)
                        printf("0");
                printf("%X ", data[i]);
        }
        printf("\n");
}

static void showRecvMsg(void *buf, ssize_t len)
{
        const unsigned char *data = (unsigned char *)buf;

        printf("recv: ");
        for (int i = 0; i < len; i++)
        {
                if (data[i] < 0x10)
                        printf("0");
                printf("%X ", data[i]);
        }
        printf("\n");
}

void *readSerialThread(void *arg)
{
        Serial *serial = (Serial *)arg;
        ssize_t recvSize;
        int nSelect;
        fd_set readFds;
        struct timeval timeout = {0};
        void *buf = malloc (128);

        while (1)
        {
                memset (buf, 0, 128);       
                FD_ZERO(&readFds);
                FD_SET(serial->fd, &readFds);
                timeout.tv_sec = 0;
                timeout.tv_usec = 50;

                nSelect = select(serial->fd + 1, &readFds, &readFds, NULL, &timeout);

                if (nSelect == -1)
                {
                        perror("select failed");
                        return NULL;
                }
                else if (nSelect && FD_ISSET(serial->fd, &readFds))
                {
                        std::cout << "selected." << std::endl;
                        serial->readSerialHex (buf, &recvSize);
                        
                        
                }
                else
                {
                        break;
                }
                showRecvMsg (buf, recvSize);
                serial->showRecvBytes ();
        }
        return NULL;

}



void Serial::flagSet(int rwFlag)
{
        using namespace std;

        int require = O_NOCTTY;

        switch (rwFlag)
        {
        case Serial::READ:
                flag = require | O_RDONLY;
                cout << "Read / Write: Read" << endl;
                break;

        case Serial::WRITE:
                flag = require | O_WRONLY;
                cout << "Read / Write: Write" << endl;
                break;

        case Serial::READ_WITE:
                flag = require | O_RDWR;
                cout << "Read / Write: Read & Write" << endl;
                break;

        default:
                flag = require | O_RDWR;
                cout << "Read / Write: Read & Write" << endl;
                break;
        }
}

void Serial::dataBitSet(uint8_t dataBit)
{
        using namespace std;
        
        tio.c_cflag &= ~CSIZE;

        switch (dataBit)
        {
        case 7:
                tio.c_cflag |= CS7;
                cout << "DataBit: 7" << endl;
                break;
        case 8:
                tio.c_cflag |= CS8;
                cout << "DataBit: 8" << endl;
                break;
        default:
                tio.c_cflag |= CS8;
                cout << "DataBit: 8" << endl;
                break;
        }
}

void Serial::paritySet(uint8_t parity)
{
        using namespace std;

        switch (parity)
        {
        case EVEN:
                tio.c_iflag |= (INPCK | ISTRIP);
                tio.c_cflag |= PARENB;
                tio.c_cflag &= ~PARODD;
                cout << "Parity: EVEN" << endl;
                break;
        case ODD:
                tio.c_cflag |= PARENB;
                tio.c_cflag |= PARODD;
                tio.c_iflag |= (INPCK | ISTRIP);
                cout << "Parity: ODD" << endl;
                break;
        case NONE:
                tio.c_cflag &= ~PARENB;
                tio.c_cflag &= ~CRTSCTS;
                cout << "Parity: NONE" << endl;
                break;
        default:
                tio.c_cflag &= ~PARENB;
                tio.c_cflag &= ~CRTSCTS;
                cout << "Parity: NONE" << endl;
                break;
        }
}

void Serial::baudrateSet(uint32_t budrate)
{
        using namespace std;

        int rc;

        switch (budrate)
        {
        case 2400:
                rc = cfsetispeed(&tio, B2400);
                rc += cfsetospeed(&tio, B2400);
                break;
        case 4800:
                rc = cfsetispeed(&tio, B4800);
                rc += cfsetospeed(&tio, B4800);
                break;
        case 9600:
                rc = cfsetispeed(&tio, B9600);
                rc += cfsetospeed(&tio, B9600);
                break;
        case 115200:
                rc = cfsetispeed(&tio, B115200);
                rc += cfsetospeed(&tio, B115200);
                break;
        case 460800:
                rc = cfsetispeed(&tio, B460800);
                rc += cfsetospeed(&tio, B460800);
                break;
        default:
                rc = cfsetispeed(&tio, B115200);
                rc += cfsetospeed(&tio, B115200);
                break;
        }

        if (rc > 0)
        {
                perror("buard set failed");
                exit(EXIT_FAILURE);
        }
        else
        {
                cout << "Buadrate: " << budrate << endl;
        }
}

void Serial::stopBitsSet(uint8_t stopBit)
{
        using namespace std;
        switch (stopBit)
        {
        case 1:
                tio.c_cflag &= ~CSTOPB;
                cout << "StopBits: 1" << endl;
                break;
        case 2:
                tio.c_cflag |= CSTOPB;
                cout << "StopBits: 2" << endl;
                break;

        default:
                tio.c_cflag |= CSTOPB;
                cout << "StopBits: 2" << endl;
                break;
        }
}

Serial::Serial(
    std::string port, int rwFlag, uint8_t dataBits,
    uint8_t parity, uint32_t baudrate, uint8_t stopBit)
{
        using namespace std;

        flagSet(rwFlag);

        fd = open(port.c_str(), flag);

        if (fd == -1)
        {
                perror("Can't open serial port");
                exit(EXIT_FAILURE);
        }

        cout << port << " connected." << endl;

        /**/
        tcgetattr (fd, &tio);
        baudrateSet(baudrate);
        dataBitSet(dataBits);
        paritySet(parity);
        stopBitsSet(stopBit);
        tio.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
        tio.c_oflag &= ~OPOST;
        tio.c_iflag &= ~(IXON | IXOFF | IXANY);
        tio.c_iflag &= ~(ICRNL|IGNCR);
        tio.c_cc[VTIME] = 0;
        tio.c_cc[VMIN] = 1;

        tcflush(fd, TCIFLUSH);

        if ((tcsetattr(fd, TCSANOW, &tio)) != 0)
        {
                perror("com set error");
                exit(EXIT_FAILURE);
        }

        cout << "Com set done." << endl;
        if (pthread_create (&readTid, NULL, readSerialThread, (void *)this) == -1)
        {
                perror ("ReadThread create failed");
                exit (EXIT_FAILURE);
        }
             
}

void Serial::readSerialHex(void *buf, ssize_t *recvSize)
{
        
        *recvSize = 0;
        while(1)
        {
                *recvSize += read (fd, ((unsigned char *)buf) + *recvSize, 1);
                if (*recvSize == -1)
                {
                        perror ("read failed");
                        break;
                }
                else if (((unsigned char *)buf)[*recvSize - 1] == 0xFF)
                        break;
                
        }
        
        pthread_mutex_lock (&lock);
        recvBytes += *recvSize;
        pthread_mutex_unlock (&lock);

}

void Serial::readSerial(unsigned char *buf, ssize_t *readBytes)
{
        *readBytes = read (fd, buf, sizeof(buf));
}

void Serial::writeSerial(void *buf)
{
        ssize_t sendSize;
        ssize_t len = strlen((char *)buf);

        sendSize = write(fd, buf, len);
        if (sendSize == -1)
        {
                perror("send to usart failed");
                return;
        }
        sendBytes += sendSize;
        showSendMsg(buf, len);

        tcflush(fd, TCIOFLUSH);
}

void Serial::writeSerial(unsigned char *buf, size_t len)
{
        ssize_t sendSize;

        sendSize = write(fd, buf, len);
        if (sendSize == -1)
        {
                perror("send to usart failed");
                return;
        }
        sendBytes += sendSize;
        showSendMsg(buf, len);

        tcflush(fd, TCIOFLUSH);
}
