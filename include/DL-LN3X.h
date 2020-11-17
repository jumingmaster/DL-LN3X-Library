#ifndef __DL - LN3X__H__
#define __DL -LN3X__H__
#include <sys/types.h>

#define MIN_ADDR 0x0001
#define MAX_ADDR 0xFFFE
#define MIN_CHANNEL 0x0B
#define MAX_CHANNEL 0x1A
#define MIN_NET_ID MIN_ADDR
#define MAX_NET_ID MAX_ADDR
#define MAX_PACKET_SIZE 0x3F
#define MIN_COM_PORT    0x80
#define MAX_COM_PORT    0xFF
#define MIN_IN_PORT     0x00
#define MAX_IN_PORT     0x7F
#define LN3X_DATA_BIT 8
#define LN3X_BUADRATE 115200
#define LN3X_STOP_BIT 1
#define TRUE    1
#define FALSE   0
typedef int dl_ln3x_t;
typedef unsigned char dlboolean;

enum dl_ln3x_err
{
        SUCCESS,
        MSG_FORMAT_ERR,
        CHANNEL_ERR,
        ADDR_ERR,
        COM_PORT_ERR,
        IN_PORT_ERR
};

/*devices 's communication packet*/
typedef struct dl_ln3x_packet
{
        u_int16_t header;
        u_int8_t src_port;
        u_int8_t dest_port;
        u_int16_t dev_addr;
        void *var_data;
        u_int8_t tail;
} dl_ln3x_packet;

/*devices info*/
typedef struct dl_ln3x
{
        u_int16_t dev_addr;
        u_int8_t channel;
        u_int16_t net_ID;
        u_int8_t com_port;
} dl_ln3x;

typedef struct dl_ln3x_message
{
        u_int8_t lenth;
        u_int8_t *message;
} dl_ln3x_message;

/*  @func       check_xxx
*
*       The under function is used for check
*       the param vaule, check_xxxx.
*       the "xxx" is the param value,
*       if true return success, else
*       return the error value.
*/
static inline dl_ln3x_t check_channel (u_int8_t channel)
{
        if (channel < MIN_CHANNEL || channel > MAX_CHANNEL)
                return CHANNEL_ERR;
        else
                return SUCCESS;
}

static inline dl_ln3x_t check_addr (u_int16_t addr)
{
        if (addr < MIN_ADDR || addr > MAX_ADDR)
                return ADDR_ERR;
        else
                return SUCCESS;
}

static inline dl_ln3x_t check_com_port (u_int8_t port)
{
        if (port < MIN_COM_PORT || port > MAX_COM_PORT)
                return COM_PORT_ERR;
        else
                return SUCCESS;
}

static inline dl_ln3x_t check_in_port (u_int8_t port)
{
        if (port < MIN_IN_PORT || port > MAX_IN_PORT)
                return IN_PORT_ERR;
        else
                return SUCCESS;
}


/* 
* @func         unpack the DL_LN3X packet
* @description: 
                unpack the communication packet,
                extract into dl_ln3x_message.

* @param        packet: dl_ln3x_packet,  
* @param        message: store the extracted info to the message
* @return       if success return 0, error return -1, you can
*               call the dl_ln3x_err for check error info
*/
dl_ln3x_t unpacked(dl_ln3x_packet *packet, dl_ln3x_message *message);

/* 
*  @func        pack_message
*
*  @description: 
                packed the message into the specific
*               packet for send.
*  
*  @param       packet: store the packed packet
*  @param       message: the message you need to send
*  @param       dest_device: the target device
*  @param       src_port: which port that the src device's port you will use
*  @return      on success return 0, error return -1, you can
*               call the dl_ln3x_err for check error info
*/
dl_ln3x_t pack_message(
    dl_ln3x_packet *packet,
    dl_ln3x_message *message,
    dl_ln3x *dest_device,
    u_int8_t src_port);

/*
*  @func        set_device_info
*
*  @description:
*               set the device information, 
                for later communicate
*  @param       device: The device will be set                 
*  @param       channel: The device's channel
*  @param       addr:  The device's address, only one
                in the same channel
*  @param       net_ID: Device's netID
*  @param       port: commucation port, above 80
*  @return      on success return 0, error return -1, you can
*               call the dl_ln3x_err for check error info
*/
dl_ln3x_t set_device_info(
    dl_ln3x *device,
    u_int8_t channel,
    u_int16_t addr,
    u_int16_t net_ID,
    u_int8_t port);


/*
*  @func        set_com_port
*
*  @description:
*               set the com port
*  @param:      device: the target device
*  @param:      which port
*  @return      on success return 0, error return -1, you can
*               call the dl_ln3x_err for check error info
*/
dl_ln3x_t set_com_port(dl_ln3x *device, u_int8_t port);



#endif //!__DL-LN3X__H__