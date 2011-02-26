#include "dnw.h"

Dnw::Dnw(QObject *parent) :
    QObject(parent)
{
    hdev = NULL;
}

Dnw::~Dnw()
{
    if (hdev)
    {
        usb_close(hdev);
        usb_release_interface(hdev,0);
    }
}
u_int16_t Dnw::checkSum(unsigned char *data, u_int32_t len)
{
    u_int16_t dnwCS = 0;
    unsigned int j;

    for (j = 0; j < len; j ++) {
        dnwCS += data[j];
    }

    return dnwCS;
}

struct usb_dev_handle * Dnw::open_port()
{
    if (hdev != NULL)
    {
        usb_release_interface(hdev,0);
        usb_close(hdev);
    }
    usb_init();
    usb_find_busses();
    usb_find_devices();
    for(bus = usb_busses; bus; bus = bus->next)
    {
        struct usb_device *dev;
        for(dev = bus->devices; dev; dev = dev->next)
        {
            if( IDVENDOR == dev->descriptor.idVendor
            &&  IDPRODUCT == dev->descriptor.idProduct)
            {
                emit si_OpenState(DEVFOUND);
                hdev = usb_open(dev);
                if(!hdev)
                {
                    emit si_OpenState(OPENERROR);
                    return NULL;
                }
                else
                {
                    if(0!=usb_claim_interface(hdev, 0))
                    {
                        emit si_OpenState(CLAIMERR);
                        usb_close(hdev);
                        hdev = NULL;
                        return NULL;
                    }
                }
                emit si_OpenState(ALLREADY);
                return hdev;

            }
        }
    }
    emit si_OpenState(DEVNOFOUND);
    return NULL;
}

