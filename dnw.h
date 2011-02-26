#ifndef DNW_H
#define DNW_H

#include <QObject>
#include <QtDBus/QDBusConnection>
#include <stdio.h>
#include <usb.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define IDVENDOR    0x5345
#define IDPRODUCT   0x1234

#define DEVFOUND    0
#define DEVNOFOUND  1
#define OPENERROR   2
#define ALLREADY    3
#define CLAIMERR    4

class Dnw : public QObject
{
    Q_OBJECT
public:
    explicit Dnw(QObject *parent = 0);
    ~Dnw();
    struct usb_dev_handle * open_port();
    u_int16_t checkSum(unsigned char *data, u_int32_t len);
private:
    struct usb_bus *bus;
    struct usb_dev_handle *hdev;

signals:
    void si_OpenState(int);

public slots:

};

#endif // DNW_H
