#include "modelsetup.h"
#include "ui_modelsetup.h"
#include <QFile>
#include <QtXml>
#include <QLineEdit>
#include <QCloseEvent>

ModelSetup::ModelSetup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ModelSetup)
{
    ui->setupUi(this);
}

ModelSetup::~ModelSetup()
{
    delete ui;
}

void ModelSetup::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void ModelSetup::showEvent(QShowEvent *)
{

    ui->tb_model->setRowCount(0);
   ui->tb_model->setSelectionBehavior(QAbstractItemView::SelectRows);

    QFile file("config.xml");
    QDomDocument doc;

      if (file.open(QIODevice::ReadWrite))
   {

          if (!doc.setContent(&file))  {
              file.close();
              return;
          }


      file.close();

      QDomElement root=doc.documentElement();
      if (root.tagName()=="groups")
      {

          QDomNode child=root.firstChild();
          while (!child.isNull())
          {
              if (child.toElement().tagName()=="item")
              {
                  addRow();
                  int i=ui->tb_model->rowCount();

ui->tb_model->item(i-1,0)->setText(child.toElement().attribute("name"));

((QLineEdit *)ui->tb_model->cellWidget(i-1,1))->setText( "0x" + QString::number(child.toElement().attribute("down_addr").toUInt(NULL,16),16));
((QLineEdit *)ui->tb_model->cellWidget(i-1,2))->setText( "0x" + QString::number(child.toElement().attribute("IDVENDOR").toUInt(NULL,16),16));
((QLineEdit *)ui->tb_model->cellWidget(i-1,3))->setText( "0x" + QString::number(child.toElement().attribute("IDPRODUCT").toUInt(NULL,16),16));


              }
              child=child.nextSibling();
          }
      }
      }
}


void ModelSetup::on_toolButton_clicked()
{
   addRow();
int i=ui->tb_model->rowCount();
ui->tb_model->editItem(ui->tb_model->item(i-1,0));

}

void ModelSetup::saveXml()
{
QFile file("config.xml");
QDomDocument doc;

  if (file.open(QIODevice::ReadWrite))
{

      if (!doc.setContent(&file))  {
          file.close();
          return;
      }

file.close();



     QDomElement root=doc.documentElement();
  if (root.tagName()=="groups")
  {
      //clear item
    while ( root.hasChildNodes())
    {
      root.removeChild(root.firstChild());
    }

      for (int i=0;i<ui->tb_model->rowCount();i++)
      {

          QDomElement item = doc.createElement("item");
          QDomAttr id = doc.createAttribute("name");
          id.setValue(ui->tb_model->item(i,0)->text());
          QDomAttr id1 = doc.createAttribute("down_addr");
          id1.setValue(((QLineEdit *) ui->tb_model->cellWidget(i,1))->text());
          QDomAttr id2 = doc.createAttribute("IDVENDOR");
          id2.setValue(((QLineEdit *) ui->tb_model->cellWidget(i,2))->text());
          QDomAttr id3 = doc.createAttribute("IDPRODUCT");
          id3.setValue(((QLineEdit *) ui->tb_model->cellWidget(i,3))->text());
          item.setAttributeNode(id);
          item.setAttributeNode(id1);
          item.setAttributeNode(id2);
          item.setAttributeNode(id3);

          root.appendChild(item);


      }
file.open(QIODevice::WriteOnly| QIODevice::Truncate);
      QTextStream out(&file);

     doc.save(out,4);
      file.close();
}

  }
}


void ModelSetup::closeEvent(QCloseEvent *event)
{
    saveXml();
    event->accept();
}

void ModelSetup::addRow()
{
    int i=ui->tb_model->rowCount()+1;
    ui->tb_model->setRowCount(i);
    QTableWidgetItem *item=new QTableWidgetItem;
    ui->tb_model->setItem(i-1,0,item);


    QLineEdit *editor = new QLineEdit();
                 editor->setValidator(new QRegExpValidator(QRegExp("^[0][x][0-9a-fA-F]+$ "),editor));
    ui->tb_model->setCellWidget(i-1,1,editor);

    QLineEdit *editor1 = new QLineEdit();
                 editor1->setValidator(new QRegExpValidator(QRegExp("^[0][x][0-9a-fA-F]+$ "),editor1));
    ui->tb_model->setCellWidget(i-1,2,editor1);

    QLineEdit *editor2 = new QLineEdit();
                 editor2->setValidator(new QRegExpValidator(QRegExp("^[0][x][0-9a-fA-F]+$ "),editor2));
    ui->tb_model->setCellWidget(i-1,3,editor2);

}

void ModelSetup::on_toolButton_2_clicked()
{
    QModelIndexList selected=ui->tb_model->selectionModel()->selectedIndexes();
    QMap<int, int> rowMap;
       foreach (QModelIndex index, selected)
       {
           //记录已选择行的<键，值>对
           rowMap.insert(index.row(), 0);
       }

       int rowToDel; //记录当前删除行
           //定义一个迭代器，并定位rowmap的反面
           QMapIterator<int, int> rowMapIterator(rowMap);
           rowMapIterator.toBack();
           //从后往前依次删除行
           while (rowMapIterator.hasPrevious())
           {
               rowMapIterator.previous();
               rowToDel = rowMapIterator.key();
               ui->tb_model-> removeRow(rowToDel);
           }
}
