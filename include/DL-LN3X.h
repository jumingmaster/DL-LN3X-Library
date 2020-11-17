#ifndef __DL - LN3X__H__
#define __DL -LN3X__H__
#include <sys/types.h>

#define DEFAULT_PACKET_LEN      4
#define MIN_ADDR                0x0001
#define MAX_ADDR                0xFFFE
#define MIN_CHANNEL             0x0B
#define MAX_CHANNEL             0x1A
#define MIN_NET_ID              MIN_ADDR
#define MAX_NET_ID              MAX_ADDR
#define MAX_PACKET_SIZE         0x3F
#define MAX_VAR_DATA_SIZE       (u_int8_t)(MAX_PACKET_SIZE - DEFAULT_PACKET_LEN)
#define MIN_COM_PORT            0x80
#define MAX_COM_PORT            0xFF
#define MIN_IN_PORT             0x00
#define MAX_IN_PORT             0x7F
#define LN3X_DATA_BIT           8
#define LN3X_BUADRATE           115200
#define LN3X_STOP_BIT           1
#define TRUE                    1
#define FALSE                   0
#define LOCAL_ADDR              0x0000
#define BLINK_PORT              0x20
#define INFO_PORT               0x21
#define ERR_PORT                0x22
#define LINK_QUALITY_PORT       0x23
#define TTL_PIN4_PORT           0x44
#define TTL_PIN5_PORT           0x45



typedef int dl_ln3x_t;
typedef unsigned char dlboolean;

enum dl_ln3x_err
{
        SUCCESS,
        MSG_FORMAT_ERR,
        CHANNEL_ERR,
        ADDR_ERR,
        COM_PORT_ERR,
        PORT_ERR,
        IN_PORT_ERR,
        NET_ID_ERR,
        INVALID_ARGUMENT,
        MSG_LEN_ERR,
        MSG_HAND_ERR
};

static const u_int8_t GET_INFO_CMD[10][8] = 
{
        {0xFE, 0x05, 0x90, 0x21, 0x00, 0x00, 0x01, 0xFF},
        {0xFE, 0x05, 0x90, 0x21, 0x00, 0x00, 0x02, 0xFF},
        {0xFE, 0x05, 0x90, 0x21, 0x00, 0x00, 0x03, 0xFF},
        {0xFE, 0x05, 0x90, 0x21, 0x00, 0x00, 0x04, 0xFF}
};

#define ADDR_CMD        0
#define NID_CMD         1
#define CHL_CMD         2
#define BUAD_CMD        3

/*devices 's communication packet*/
typedef struct dl_ln3x_packet
{
        u_int16_t header;
        u_int8_t src_port;
        u_int8_t dest_port;
        u_int16_t dev_addr;
        u_int8_t var_data[MAX_VAR_DATA_SIZE];
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

/*unpacked message*/
typedef struct dl_ln3x_message
{
        u_int8_t lenth;
        u_int8_t *message;
        dl_ln3x  *src_dev;
        dl_ln3x  *dest_dev;
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

static inline dl_ln3x_t check_netID (u_int16_t id)
{
        if (id < MIN_NET_ID || id > MAX_NET_ID)
                return NET_ID_ERR;
        else
                return SUCCESS;
}

static inline dl_ln3x_t check_message_len (dl_ln3x_message * message)
{
        if (!message)
                return INVALID_ARGUMENT;
        if (message->lenth > MAX_VAR_DATA_SIZE)
                return MSG_LEN_ERR;
        
        return SUCCESS;
}

static dl_ln3x_t check_device (dl_ln3x *device, dlboolean in_port)
{
        dl_ln3x_t err;

        if (!device)
                return INVALID_ARGUMENT;
        if ( (err = check_addr (device->dev_addr)) != 0)
                return err;
        if ( (err = check_netID (device->net_ID)) != 0)
                return err;
        if ( (err = check_channel (device->channel)) != 0)
                return err;
        switch (in_port)
        {
        case TRUE:
                if ( (err = check_in_port (device->com_port)) != 0)
                        return err;
                break;
        
        default:
        case FALSE:
                if ( (err = check_com_port (device->com_port)) != 0)
                        return err;
                break;
        }

        return SUCCESS;
}

static dl_ln3x_t check_var_data (dl_ln3x_message *message)
{
        dl_ln3x_t err;

        if (!message)
                return INVALID_ARGUMENT;

        if ( (err= check_message_len (message)) != 0)
                return err;

        int index;
        for (index = 0; index < message->lenth; index ++)
        {
                if (message->message[index] == 0xFF)
                        return MSG_FORMAT_ERR;
        }
        
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
*  @param       in_port: whether inside port

*  @return      on success return 0, error return -1, you can
*               call the dl_ln3x_err for check error info
*/
dl_ln3x_t pack_message(
    dl_ln3x_packet *packet,
    dl_ln3x_message *message,
    dlboolean in_port);

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
*  @param       in_port: true or false for set inside port
*  @return      on success return 0, error return -1, you can
*               call the dl_ln3x_err for check error info
*/
dl_ln3x_t set_device_info(
    dl_ln3x *device,
    u_int8_t channel,
    u_int16_t addr,
    u_int16_t net_ID,
    u_int8_t port,
    dlboolean in_port);


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

/*
*  @func        set_cin_port
*
*  @description:
*               set the inside port
*  @param:      device: the target device
*  @param:      which port
*  @return      on success return 0, error return -1, you can
*               call the dl_ln3x_err for check error info
*/
dl_ln3x_t set_in_port(dl_ln3x *device, u_int8_t port);

/*
*  @func        init_local_dev
*  
*  @description:
*               init the local device:
*               get the net id, addr, channel
*  @param:      device: store the device's info
*
*  @return:     on success return 0, error return -1, you can
*               call the dl_ln3x_err for check error info
*/
//dl_ln3x_t init_local_dev (dl_ln3x *device);


dl_ln3x_t create_message (
        dl_ln3x_message * message, 
        u_int8_t * msg, 
        u_int8_t lenth, 
        dl_ln3x *src_dev, 
        dl_ln3x *dest_dev);

#endif //!__DL-LN3X__H__