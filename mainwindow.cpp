#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dbmanager.h"
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <Items_Definition.h>
#include <QStringListModel>
#include <QTableWidgetItem>
#include <QThread>

#include "xlsxdocument.h"
#include "xlsxworksheet.h"
#include "xlsxcellrange.h"

using namespace QXlsx;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->prGroupBox->addAction(ui->action_Edit_PR);
    ui->prGroupBox->setEnabled(false);
    currentPR = new PR_Number;
}

MainWindow::~MainWindow()
{
    delete ui;
    if(db)
    db->~DbManager();

}


void MainWindow::InitPR(PR_Number *PR)
{
    PR->Department.clear();
    PR->ID =0;
    PR->Item_Count =0;
    PR->PR_Number =0;
    PR->Purpose.clear();
    PR->Remarks.clear();
    PR->Status.clear();
    PR->Subjects.clear();
    PR->Submit_Date = QDate();
    PR->TAG.clear();
}

void MainWindow::InitPRItems(PR_Items *Items)
{
    Items->Description.clear();
    Items->ID =0;
    Items->Item_Code.clear();
    Items->Progress = 0;
    Items->PR_Number = 0;
    Items->Quantity = 0;
    Items->Remarks.clear();
    Items->Status.clear();
    Items->UOM.clear();
    Items->Classification.clear();
}

void MainWindow::on_actionAdd_From_Excel_File_triggered()
{
//    QThread *worker = new QThread(this);


    QString filePath = QFileDialog::getOpenFileName(0, "Open xlsx file", QString(), "*.xlsx");
    if (filePath.isEmpty())
        return ;
    Document xlsx(filePath);
    QList<int> Numb = db->GetPRNumber();
    PR_Number *PR = new PR_Number;
    PR_Items *Items = new PR_Items;
    int new_Items=0;
    int item_errors=0;

    QList<int> PR_List;
    qDebug()<<"Total Columns"<<xlsx.dimension().columnCount();
    qDebug()<<"Total Rows"<<xlsx.dimension().rowCount();

    //////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief PR_List
    ///
    ///
    /// /////////////////////////////////////////////////////////////////////////////////////////////

    InitPR(PR);
    QList<PR_Number> newPR;
    int lll = 0;
    for(int x=1; x<xlsx.dimension().rowCount()+1;x++)
    {
        PR->PR_Number = xlsx.read(x,1).toInt();
        if((!PR_List.contains(PR->PR_Number))&(!Numb.contains(PR->PR_Number))&(PR->PR_Number !=0))
        {
//            PR->Submit_Date = QDate();

            PR_List.append(PR->PR_Number);
            PR->Status = xlsx.read(x,2).toString();
            PR->Submit_Date =  xlsx.read(x,8).toDate();
            PR->Purpose= xlsx.read(x,9).toString();
            PR->Department = xlsx.read(x,10).toString();
            PR->Subjects = xlsx.read(x,11).toString();
            PR->Remarks= xlsx.read(x,12).toString();
            newPR.append(*PR);
            PR = new PR_Number;

        }
        lll ++;



    }
    qDebug()<<"Finished Looping:"<<lll;
    qDebug()<<"Total New Records:"<<newPR.size();
//    return;
    if(newPR.size()>0)
    {

        if(!db->addPRBatch(newPR))
        {
            qDebug()<<"PR Add SQL Error ";
        }
//        foreach (PR_Number pp , newPR)
//        {
//            if(!db->addPRBatch(&pp))
//            {
//                qDebug()<<"PR Add SQL Error "<<QString::number(pp.PR_Number);
//            }
//        }
    }



    return;
    /////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief PR_List
    ///
    ///
    /// /////////////////////////////////////////////////////////////////////////////////////////////




    InitPR(PR);
    InitPRItems(Items);


    for(int x=1; x<xlsx.dimension().rowCount()+1;x++)
//    for(int x=2060; x<3270;x++)
//    for(int x=xlsx.dimension().rowCount()-50; x<xlsx.dimension().rowCount()+1;x++)
    {
//        qDebug()<<"==============================";
//        qDebug()<<"New PR Loop X= "<<x;
        PR->PR_Number = xlsx.read(x,1).toInt();
        if((!PR_List.contains(PR->PR_Number))&(!Numb.contains(PR->PR_Number))&(PR->PR_Number !=0))
        {
            PR->Submit_Date = QDate();

            PR_List.append(PR->PR_Number);
            PR->Status = xlsx.read(x,2).toString();
            PR->Submit_Date =  xlsx.read(x,8).toDate();
            PR->Purpose= xlsx.read(x,9).toString();
            PR->Department = xlsx.read(x,10).toString();
            PR->Subjects = xlsx.read(x,11).toString();
            PR->Remarks= xlsx.read(x,12).toString();
            if(!db->addPR(PR))
            {
                qDebug()<<"IN Row"<<QString::number(x)<<"PR Add SQL Error "<<QString::number(PR->PR_Number);
            }

        }
        bool br =false;
        QList<QString> EItems = db->GetPRItems(PR->PR_Number);
        if((PR_List.contains(PR->PR_Number))|(Numb.contains(PR->PR_Number)))
        {
            int y=0;
            new_Items = 0;
            for (y =0 ;(( y<(xlsx.dimension().rowCount()-x))&(!br));y++)
            {

//                qDebug()<<"Items loop X= "<<x<<"  Y= "<<y;

                Items->PR_Number = xlsx.read(y+x,1).toInt();
                if(Items->PR_Number == PR->PR_Number)
                {
                    Items->Item_Code = xlsx.read(y+x,4).toString();
                    if(!((Items->Item_Code.isEmpty())|(EItems.contains(Items->Item_Code))))
                    {
                        Items->Description = xlsx.read(y+x,5).toString();
                        Items->UOM = xlsx.read(y+x,6).toString();
                        Items->Quantity = xlsx.read(y+x,7).toInt();
                        Items->Progress = xlsx.read(y+x,3).toInt();
                        Items->Status = xlsx.read(y+x,2).toString();
                        Items->Remarks = xlsx.read(y+x,12).toString();
                        if(db->addItem(Items))
                        {
                            new_Items++;
                        }
                        else
                        {

                            qDebug()<<"IN Row"<<QString::number(y+x)<<"Item Add SQL Error "<<QString::number(Items->PR_Number);
                            item_errors++;
                        }

                    }
                    else
                    {
                        if(Items->Item_Code.isEmpty())qDebug()<<"IN Row"<<QString::number(y+x)<<" Item "<<"Empty Item Code In PR:"<<QString::number(Items->PR_Number);
                        else qDebug()<<"IN Row"<<QString::number(y+x)<<" Item "<<Items->Item_Code<<"DuplicatedItem in same PR:"<<QString::number(Items->PR_Number);
                        qDebug()<<"======================";
                        item_errors++;
                    }
                }
                else {
                    y--;
//                    qDebug()<<"New PR Loop X= "<<x<<"  Y= "<<y;
                    br=true;
                }

            }

            db->updatePRItemCount(PR->PR_Number,y);
            x +=(y-1);
//            qDebug()<<"New PR Loop After calc X= "<<x<<"  Y= "<<y;
    }
}
    qDebug()<<"Total Added Records: "<<PR_List.count();
    qDebug()<<"Total Non confirming Items: "<<item_errors;

}

void MainWindow::on_actionConnect_To_Database_triggered()
{
    QString filePath = QFileDialog::getOpenFileName(0, "Open SQlite3 file", QString(), "*.db");
    if (filePath.isEmpty())
        return;

    if (db != Q_NULLPTR)
    {
        qDebug()<<"Deleting old DBManager";
        db->~DbManager();
    }
    db  = new DbManager(filePath);
    ui->actionAdd_From_Excel_File->setEnabled(true);
    ui->groupBox_2->setEnabled(true);
    ui->statusBar->showMessage("Databes connected");
    QList<int> EItems = db->GetPRNumber();
    QStringList *stringList = new QStringList();


    for(int i=EItems.size()-1;i>=0;i--)
    {
        stringList->append(QString::number(EItems[i]));
    }
    QStringListModel *listModel = new QStringListModel(*stringList, NULL);
    ui->listView->setModel(listModel);

}


void MainWindow::on_listView_clicked(const QModelIndex &index)
{
    QStringList slist;
    slist.append( index.data(Qt::DisplayRole ).toString());
    qDebug()<<"Clicked" << slist[0];
    updateView(slist[0].toInt());
}

void MainWindow::on_lineEdit_returnPressed()
{
    qDebug()<<ui->lineEdit->text();
    updateView(ui->lineEdit->text().toInt());
}

//void MainWindow::on_lineEdit_textChanged(const QString &arg1)
//{
//    qDebug()<<"text changed"<<ui->lineEdit->text()<<arg1;
//}

void MainWindow::on_lineEdit_textEdited(const QString &arg1)
{
    qDebug()<<"Text edited"<<ui->lineEdit->text()<<QString(arg1);
}

bool MainWindow::updateView(int prNum)
{
    qDebug()<<"Department: "<<ui->comboBox->currentText();
    ui->comboBox->addItem("Smart add");
    qDebug()<<ui->comboBox->findText("Maintenance");


//    PR_Number pp;
    InitPR(currentPR);
    db->getPR(prNum,currentPR);
//    PR->Submit_Date.toString(Qt::ISODate)
    ui->editPR->setText(QString::number(currentPR->PR_Number));
    ui->editCount->setText(QString::number(currentPR->Item_Count));
    ui->editDate->setDate(currentPR->Submit_Date);
    ui->editDepart->setText(currentPR->Department);
    ui->editRemrk->setText(currentPR->Remarks);
    ui->editStat->setText(currentPR->Status);
    ui->editSub->setText(currentPR->Subjects);
    ui->editPorp->setText(currentPR->Purpose);
    ui->editTags->setText(currentPR->TAG);
    qDebug()<<"ID ="<<currentPR->ID;
    qDebug()<<"PR# ="<<currentPR->PR_Number;
    QList<PR_Items> items = db->getItems(currentPR->PR_Number);
    ui->tableWidget->clearContents();
    int count = items.size();
    if(count)
    {
        ui->tableWidget->setRowCount(count);
        QTableWidgetItem *widgetItems;
        for (int i=0;i<count;i++)
        {
            widgetItems = new QTableWidgetItem();
            widgetItems->setText(items[i].Item_Code);
            ui->tableWidget->setItem(i,0,widgetItems);

            widgetItems = new QTableWidgetItem();
            widgetItems->setText(items[i].Description);
            ui->tableWidget->setItem(i,1,widgetItems);

            widgetItems = new QTableWidgetItem();
            widgetItems->setText(items[i].UOM);
            ui->tableWidget->setItem(i,2,widgetItems);

            widgetItems = new QTableWidgetItem();
            widgetItems->setText(QString::number(items[i].Quantity));
            ui->tableWidget->setItem(i,3,widgetItems);

            widgetItems = new QTableWidgetItem();
            widgetItems->setText(QString::number(items[i].Progress));
            ui->tableWidget->setItem(i,4,widgetItems);

            widgetItems = new QTableWidgetItem();
            widgetItems->setText(items[i].Status);
            ui->tableWidget->setItem(i,5,widgetItems)
                    ;
            widgetItems = new QTableWidgetItem();
            widgetItems->setText(items[i].Remarks);
            ui->tableWidget->setItem(i,6,widgetItems);

            widgetItems = new QTableWidgetItem();
            widgetItems->setText(items[i].Classification);
            ui->tableWidget->setItem(i,7,widgetItems);
        }


    }
    items.clear();

    return true;
}

//void MainWindow::on_listView_indexesMoved(const QModelIndexList &indexes)
//{
//    qDebug()<<"ID =";

//}

void MainWindow::on_listView_activated(const QModelIndex &index)
{
    qDebug()<<"ID 123333=";
    QStringList slist;
    slist.append( index.data(Qt::DisplayRole ).toString());
    qDebug()<<"Clicked" << slist[0];
    updateView(slist[0].toInt());
}

void MainWindow::on_action_Edit_PR_triggered()
{
    ui->prGroupBox->setEnabled(false);
}

void MainWindow::on_actionEdit_Items_triggered()
{

}

void MainWindow::on_actionEdit_current_PR_triggered()
{
    if(!ui->prGroupBox->isEnabled())
    {
        ui->prGroupBox->setEnabled(true);
    }
    else
    {
        QMessageBox msg ;
        msg.setText("Discard current Change?");
        msg.setStandardButtons(QMessageBox::Save | QMessageBox::Abort | QMessageBox::Discard );
        msg.setDefaultButton(QMessageBox::Save);
        msg.setIcon(QMessageBox::Warning);
        int ret = msg.exec();
        switch (ret)
        {
        case QMessageBox::Save:
            saveViews();
            break;
        case QMessageBox::Abort :
            updateView(currentPR->PR_Number);
            break;
        case QMessageBox::Discard :
            break;
        }
    }

}


bool MainWindow::saveViews()
{

    return true;
}
