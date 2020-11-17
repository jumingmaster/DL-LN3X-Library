#include "DL-LN3X.h"

dl_ln3x_t unpacked(dl_ln3x_packet *packet, dl_ln3x_message *message)
{

}

dl_ln3x_t pack_message(
    dl_ln3x_packet *packet,
    dl_ln3x_message *message,
    dl_ln3x *dest_device,
    u_int8_t src_port)
{

}

dl_ln3x_t set_device_info(
    dl_ln3x *device,
    u_int8_t channel,
    u_int16_t addr,
    u_int16_t net_ID,
    u_int8_t port)
{
        int err;
        
        if ((err = check_channel (channel)) != 0)
                return err;
        if ((err = ))

        device->channel = channel;

}