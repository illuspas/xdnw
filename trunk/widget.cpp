#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    dnw = new Dnw;
    down_addr = 0xC0000000;
    ui->label_downaddr_v->setText("0x" + QString::number(down_addr,16));
    connect(dnw, SIGNAL(si_OpenState(int)), this, SLOT(sl_OpenState(int)));
    connect(ui->pbn_open,SIGNAL(clicked()),this,SLOT(sl_OpenFile()));
    connect(ui->pbn_write,SIGNAL(clicked()),this,SLOT(sl_StartWrite()));
    connect(ui->pbn_set,SIGNAL(clicked()),this,SLOT(sl_setDownAddr()));
    connect(ui->pbn_about, SIGNAL(clicked()), this, SLOT(sl_about()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::sl_OpenFile()
{
    QString FileName;
    QString FilePath;
    int     Filesize;
    u_int16_t csum;
    FilePath = QFileDialog::getOpenFileName(this,tr("Select File"),"./", tr("All Files (*)"));
    if(FilePath != 0)
    {
        QStringList path = FilePath.split("/");
        FileName = path[path.size()-1];
        ui->label_filename_v->setText(FileName);
        QFile writeFile(FilePath);
        Filesize = writeFile.size();
        len = Filesize + 10;

        ui->label_filesize_v->setText("0x" + QString::number(Filesize,16) + " Bytes");

        writeFile.open(QIODevice::ReadOnly);
        write_buf = (char *)malloc(Filesize+10);

        *((u_int32_t*)write_buf) = down_addr;		// download address
        *((u_int32_t*)write_buf+1) = len;           // download size;
        writeFile.read(write_buf+8,Filesize);
        csum = dnw->checkSum((unsigned char*)write_buf+8,Filesize);
        write_buf[Filesize+8] = csum & 0xff;
        write_buf[Filesize+9] = (csum >> 8) & 0xff;

        writeFile.close();
        hdev = dnw->open_port();
    }
    else
    {
        ui->label_filename_v->setText("");
        ui->label_filesize_v->setText("");
        ui->pbn_write->setEnabled(false);
    }
}

void Widget::sl_StartWrite()
{
    int remain = len;
    int towrite;
    QProgressDialog progress("Writing file...", 0, 0, len, this);
    progress.setWindowModality(Qt::WindowModal);
    while(remain)
    {
        towrite = remain>512 ? 512 : remain;
        int writed = usb_bulk_write(hdev,0x02, write_buf+(len-remain), towrite, 3000);
        if(towrite != writed)
        {
            perror("usb_bulk_write failed");
            break;
        }
        remain-=towrite;
        progress.setValue(len-remain);
    }
    free(write_buf);
    ui->pbn_write->setEnabled(false);
    ui->pbn_set->setEnabled(false);
    ui->label_message_v->setText(tr("Transfer complete !"));
}

void Widget::sl_OpenState(int state)
{
    ui->pbn_write->setEnabled(false);
    ui->pbn_set->setEnabled(false);
    switch (state)
    {
    case DEVFOUND:
        ui->label_message_v->setText(tr("Target usb device found !"));
        break;
    case DEVNOFOUND:
        ui->label_message_v->setText(tr("Target usb device not found !"));
        break;
    case OPENERROR:
        ui->label_message_v->setText(tr("Cannot open device !"));
        break;
    case ALLREADY:
        ui->label_message_v->setText(tr("All ready to write !"));
        ui->pbn_write->setEnabled(true);
        ui->pbn_set->setEnabled(true);
        break;
    case CLAIMERR:
        ui->label_message_v->setText(tr("Cannot claim interface !"));
        break;
    }

}

void Widget::sl_about()
{
    QMessageBox::information(this, tr("About Xdnw"),
                       tr("<h2>Xdnw 0.4</h2>"
                          "<p>dnw for Linux"
                          "<br />You must have super user privileges to run it."
                          "<br />Copyright &copy; 2009-2011 illuspas@gmail.com"
                          "<br /><a href='http://code.google.com/p/xdnw/'>http://code.google.com/p/xdnw/</a></p>"
                          ));

}

void Widget::sl_setDownAddr()
{
    bool ok;
    QString addr = QInputDialog::getText(this, tr("Setting"),
        tr("Download Address"), QLineEdit::Normal,"0x"+QString::number(down_addr,16), &ok);
    if (ok)
    {
        down_addr = addr.toUInt(&ok,16);
        *((u_int32_t*)write_buf) = down_addr;
        ui->label_downaddr_v->setText("0x"+QString::number(down_addr,16));

    }

}
