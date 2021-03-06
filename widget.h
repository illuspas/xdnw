#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QFileDialog>
#include <QProgressDialog>
#include <QFile>
#include <QMessageBox>
#include <QInputDialog>
#include "dnw.h"
#include <QtXml/QDomDocument>
#include "modelsetup.h"


namespace Ui {
    class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    Dnw *dnw;
    char *write_buf;
    int len;
    struct usb_dev_handle *hdev;
    u_int32_t down_addr;
private:
    Ui::Widget *ui;
QDomDocument *doc;
ModelSetup mo;

public slots:
    void sl_OpenFile();
    void sl_StartWrite();
    void sl_OpenState(int);
    void sl_about();
    void sl_setDownAddr();
private slots:
    void on_cb_model_currentIndexChanged(const QString &arg1);
    void on_pbn_setup_clicked();
    void SetModel();
};

#endif // WIDGET_H
