#ifndef MODELSETUP_H
#define MODELSETUP_H

#include <QDialog>

namespace Ui {
class ModelSetup;
}

class ModelSetup : public QDialog
{
    Q_OBJECT
    
public:
    explicit ModelSetup(QWidget *parent = 0);
    ~ModelSetup();
    
protected:
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *);
    


private slots:
    void on_toolButton_clicked();

    void on_toolButton_2_clicked();

private:
    Ui::ModelSetup *ui;
    virtual void showEvent(QShowEvent *);
    void saveXml();
   void addRow();
};

#endif // MODELSETUP_H
