#include "DL-LN3X.h"
#include <string.h>

dl_ln3x_t unpacked(dl_ln3x_packet *packet, dl_ln3x_message *message)
{
        
}

dl_ln3x_t pack_message(
    dl_ln3x_packet *packet,
    dl_ln3x_message *message,
    dlboolean in_port)
{
        dl_ln3x_t err;

        if (!message || !packet || message->dest_dev || message->src_dev)
                return INVALID_ARGUMENT;

        if ( (err = check_message_len (message)) != 0 )
                return err;

        if ( (err = check_device (message->dest_dev, in_port) ) != 0)
                return err;

        if ( (err = check_device (message->src_dev, FALSE)) != 0)
                return err;
        

        packet->header = (0xFE << 8) | (message->lenth + DEFAULT_PACKET_LEN) ;
        packet->src_port = message->src_dev->com_port;
        packet->dest_port = message->dest_dev->com_port;
        packet->dev_addr = message->dest_dev->dev_addr;
        
        if ( (err = check_var_data (message)) != 0)
                return err;
        memcpy (packet->var_data, message->message, message->lenth);
        packet->tail = 0xFF;

        return SUCCESS;
}

dl_ln3x_t set_device_info(
    dl_ln3x *device,
    u_int8_t channel,
    u_int16_t addr,
    u_int16_t net_ID,
    u_int8_t port,
    dlboolean in_port)
{
        dl_ln3x_t err;
        if (!device)
                return INVALID_ARGUMENT;
        if ((err = check_channel (channel)) != 0)
                return err;
        if ((err = check_addr (addr)) != 0)
                return err;
        if ((err = check_netID (net_ID)) != 0)
                return err;

        switch (in_port)
        {
        case TRUE:
                if ((err = set_in_port (device, port)) != 0)
                        return err;
                else
                        break;
        default:
        case FALSE:
                if ((err = set_com_port (device, port)) != 0)
                        return err;
                else
                        break;
        }

        device->channel = channel;
        device->dev_addr = addr;
        device->net_ID = net_ID;

        return SUCCESS;
}

dl_ln3x_t set_com_port(dl_ln3x *device, u_int8_t port)
{
        if (!device)
                return INVALID_ARGUMENT;
        if (!check_com_port (port))
                return COM_PORT_ERR;
        else
                return SUCCESS;
}

dl_ln3x_t set_in_port(dl_ln3x *device, u_int8_t port)
{
        if (!device)
                return INVALID_ARGUMENT;
        if (!check_in_port (port))
                return IN_PORT_ERR;
        else
                return SUCCESS;
}


dl_ln3x_t create_message (
        dl_ln3x_message * message, 
        u_int8_t * msg, 
        u_int8_t lenth, 
        dl_ln3x *src_dev, 
        dl_ln3x *dest_dev)
{
        if (lenth > MAX_VAR_DATA_SIZE)
                return MSG_LEN_ERR;
        if (!src_dev && !dest_dev)
        {
                message->src_dev = message->dest_dev = NULL;
                message->lenth = lenth;
                memcpy (message->message, msg, lenth);
        }        
        else if (!dest_dev && src_dev)
                return INVALID_ARGUMENT;
        else if (!src_dev && dest_dev)
                return INVALID_ARGUMENT;
        else
        {
                
        }
        
        return SUCCESS;
}