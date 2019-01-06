#include "dbmanager.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QException>
#include <QSqlRecord>
#include <QMap>
//#define QT_NO_CAST_FROM_ASCII

DbManager::DbManager(QString &path)
{
   m_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE","PR_Master"));
   m_db->setDatabaseName(path);

   if (!m_db->open())
   {
      qDebug() << "Error: connection with database fail";
   }
   else
   {
     qDebug() << "Database: connection ok";
   }


}


bool DbManager::addPR(PR_Number* PR)
{
    //INSERT INTO PR_Tracker (PR_Number)(Submit_Date) VALUES (:num)(:date)");
    QString stament;
    QString values;
    stament = "INSERT INTO PR_Tracker ("+ QString(PR_NUM);
    values = "VALUES (";
    try
    {
    if (PR->PR_Number != 0)
    {
        values.append( QString::number(PR->PR_Number));
        if(PR->Submit_Date.isValid())
        {
            stament.append(", "+QString(PR_SUBMIT_DATE));
            values.append(", '"+ PR->Submit_Date.toString(Qt::ISODate)+"'");
        }
        if(!PR->Department.isEmpty())
        {
            stament.append(", "+QString(PR_DEPARTMENT));
            values.append(", '"+ PR->Department+"'");
        }
        if(!PR->Purpose.isEmpty())
        {
            stament.append(", "+QString(PR_PURPOSE));
            values.append(", '"+ PR->Purpose+"'");
        }
        if(PR->Item_Count != 0)
        {
            stament.append(", "+QString(PR_ITEM_COUNT));
            values.append(", "+ QString::number(PR->Item_Count));
        }
        if(!PR->Status.isEmpty())
        {
            stament.append(", "+QString(PR_STATUS));
            values.append(", '"+PR->Status +"'");
        }
        if(!PR->Remarks.isEmpty())
        {
            stament.append(", "+QString(PR_REMARKS));
            values.append(", '"+PR->Remarks +"'");
        }
        if(!PR->TAG.isEmpty())
        {
            stament.append(", "+QString(PR_TAG));
            values.append(", '"+PR->TAG +"'");
        }
        if(!PR->Subjects.isEmpty())
        {
            stament.append(", "+QString(PR_SUBJECT));
            values.append(", '"+PR->Subjects +"'");
        }
        stament.append(") "+values+")");
//        qDebug()<<stament;

        QSqlQuery queryAdd(m_db->database("PR_Master"));
        queryAdd.prepare(stament);
        if(queryAdd.exec())
        {
            //success = true;
        }
        else
        {
            qDebug()<<queryAdd.executedQuery();
                qDebug() << "SQL Add PR failed: " << queryAdd.lastError().text();
                return false;
        }
        return 1;
    }
    }
    catch (QException e)
    {
        qDebug()<<"Null Error hapened "<<e.what();
    }
    catch(...)
    {
        qDebug()<<"Exception occurred";

    }


    return true;
}

QString DbManager::LastError()
{
    return lastErr;
}

int DbManager::insert_PR()
{

    QSqlQuery queryAdd(m_db->database("PR_Master"));
    QString qq;//="6587687";
    queryAdd.prepare("INSERT INTO PR_Tracker (PR_Number)(Submit_Date) VALUES (:num)(:date)");
    queryAdd.bindValue(":name", 67889789);
    queryAdd.bindValue(":date", qq);
    if(queryAdd.exec())
    {
        //success = true;
    }
    else
    {
        qDebug()<<queryAdd.executedQuery();
            qDebug() << "SQL Add PR failed: " << queryAdd.lastError().text();
    }
    return 1;
}





DbManager::~DbManager()
{
    /*
    qDebug()<<"List all tables";
    QSqlQuery query(m_db->database("PR_Master"));
//    query.
    QStringList tables;
    QString ssql = "SELECT * FROM sqlite_master";

    query.prepare(ssql);
    query.exec();
    int y =1;
    while (query.next())
    {
        qDebug()<<"y="<<y<<".1";
        qDebug() << query.value("sql").toString();
        qDebug()<<"y="<<y++<<".2";
        int i=1;
        if (query.value("type").toString() == "table")
        {
            tables << query.value("name").toString();
            qDebug()<<"i="<<i<<".1";
            qDebug()<<"Table is :" <<query.value("name").toString();
            qDebug()<<"i="<<i++<<".2";
        }
    }
qDebug()<<"NEW";
    foreach (const QString &str,m_db->tables())
    {
        qDebug()<<str;
    }


    query.~QSqlQuery();
    */
    if(m_db->isOpen())
        qDebug()<<"Database is still open";
    m_db->~QSqlDatabase();
    QSqlDatabase::removeDatabase("PR_Master");

}

QList<int> DbManager::GetPRNumber()
{
    QSqlRecord rec ;
    QList<int> Numbers;
    QSqlQuery query(m_db->database("PR_Master"));

    query.prepare("SELECT PR_NUMBER FROM PR_Tracker");
    query.exec();
    while(query.next())
    {
        Numbers.append(query.record().value(0).toInt());;
//        rec = query.record();
//    for (int x=0;x<rec.count();x++)
//        qDebug()<<rec.fieldName(x)<<"   "<<rec.value(x).toString();

//        qDebug()<<"Record Count "<<rec.count();


    }
    qDebug()<<Numbers.count()<<" Total returned recordes";

return Numbers;

}

bool DbManager::addItem(PR_Items *Item)
{
    if((Item->PR_Number !=0)&(!Item->Item_Code.isEmpty())&(!Item->Description.isEmpty()))
    {
        QString stament = "INSERT INTO PR_Items ("+ QString(ITEM_PR_NUM)+", "+QString(ITEM_CODE)+", "+QString(ITEM_DESC);
//        QString values = ") VALUES ("+QString::number(Item->PR_Number)+", '"+Item->Item_Code+"', '"+Item->Description+"'";
           QString values = ") VALUES (:num, :code, :dec";
        if(!Item->UOM.isEmpty())
        {
            stament.append(", "+QString(ITEM_UOM));
//            values.append(", '"+Item->UOM+"'");
            values.append(", :uom");
        }
        stament.append(", "+QString(ITEM_QUANTITY));
//        values.append(", "+QString::number(Item->Quantity));
        values.append(", :quanty");


        stament.append(", "+QString(ITEM_PROGRESS));
//        values.append(", "+QString::number(Item->Progress));
        values.append(", :prog");

        if(!Item->Status.isEmpty())
        {
            stament.append(", "+QString(ITEM_STATUS));
//            values.append(", '"+Item->Status+"'");
            values.append(", :stat");
        }
        if(!Item->Remarks.isEmpty())
        {
            stament.append("' "+QString(ITEM_REMARKS));
//            values.append(", '"+Item->Remarks+"'");
            values.append(", :rem");
        }
        if(!Item->Classification.isEmpty())
        {
            stament.append("' "+QString(ITEM_CLASS));
            values.append(", :class");
        }
        stament.append(values+")");
        QSqlQuery queryAdd(m_db->database("PR_Master"));
        queryAdd.prepare(stament);
//        VALUES (:num, :code, :dec)";
        queryAdd.bindValue(":num", Item->PR_Number);
        queryAdd.bindValue(":code",Item->Item_Code);
        queryAdd.bindValue(":dec",Item->Description);
//        if(!Item->UOM.isEmpty()) queryAdd.bindValue(":uom",Item->UOM);
        queryAdd.bindValue(":uom",Item->UOM);
         queryAdd.bindValue(":quanty",Item->Quantity);
         queryAdd.bindValue(":prog",Item->Progress);
//         if(!Item->Status.isEmpty())queryAdd.bindValue(":stat",Item->Status);
         queryAdd.bindValue(":stat",Item->Status);
//         if(!Item->Remarks.isEmpty())queryAdd.bindValue(":rem",Item->Remarks);
         queryAdd.bindValue(":rem",Item->Remarks);
//         if(!Item->Classification.isEmpty())queryAdd.bindValue(":class",Item->Classification);
         queryAdd.bindValue(":class",Item->Classification);


        if(queryAdd.exec())
        {
            //success = true;
        }
        else
        {
            qDebug()<<stament;
                qDebug() << "SQL Add PR Item failed: " << queryAdd.lastError().text();
                return false;
        }
    }
    return true;
}

QList<QString> DbManager::GetPRItems(int PR)
{
    QList<QString> items ;
    if (PR!=0)
    {
//        qDebug()<<"Finding PR = "+QString::number(PR);
        QSqlQuery queryGetItem(m_db->database("PR_Master"));
        queryGetItem.prepare("SELECT "+QString(ITEM_CODE)+", "+QString(ITEM_DESC)+" FROM PR_Items WHERE PR_Number = "+QString::number(PR));
        if(queryGetItem.exec())
        {
            while (queryGetItem.next()) {
                items.append(queryGetItem.record().value(0).toString());
            }
        }
        else
        {
            qDebug()<<queryGetItem.executedQuery();
            qDebug() << "SQL Search failed: " << queryGetItem.lastError().text();
            qDebug()<<queryGetItem.lastQuery();
        }
    }

return items;
}


bool DbManager::updatePRItemCount(int pR, int Count)
{
//    qDebug()<<"Item count"<<QString::number(Count)<<" For PR#"<<QString::number(pR);
    QSqlQuery update(m_db->database("PR_Master"));
    update.prepare("UPDATE PR_Tracker SET "+QString(PR_ITEM_COUNT)+"= "+QString::number(Count)+" WHERE "+QString(PR_NUM)+" = "+QString::number(pR));
//    update.bindValue(":count", Count);
//    update.bindValue(":num",pR);
    if(update.exec())
    {
//        qDebug()<<update.executedQuery();

    }
    else
    {
        qDebug() << "SQL Update Error " << update.lastError().text();
        qDebug()<<update.lastQuery();
        return false;
    }
    return true;
}

bool DbManager::getPR(int prNum, PR_Number *pr)
{
    QSqlQuery getPR(m_db->database("PR_Master"));
    getPR.prepare("SELECT "+QString(PR_ID)+", "+QString(PR_NUM)+", "+QString(PR_SUBMIT_DATE)+", "
                  +QString(PR_DEPARTMENT)+", "+QString(PR_PURPOSE)+", "+QString(PR_ITEM_COUNT)+", "
                  +QString(PR_STATUS)+", "+QString(PR_REMARKS)+", "+QString(PR_TAG)+", "+QString(PR_SUBJECT)
                  +" FROM PR_Tracker WHERE PR_Number = :num");
    getPR.bindValue(":num",prNum);
    if(getPR.exec())
    {
        if(getPR.next())
        {
//            Numbers.append(query.record().value(0).toInt());
            pr->ID = getPR.record().value(0).toInt();
            pr->PR_Number = getPR.record().value(1).toInt();
            pr->Submit_Date = getPR.record().value(2).toDate();
            pr->Department =  getPR.record().value(3).toString();
            pr->Purpose =  getPR.record().value(4).toString();
            pr->Item_Count =  getPR.record().value(5).toInt();
            pr->Status =  getPR.record().value(6).toString();
            pr->Status =  getPR.record().value(7).toString();
            pr->Remarks=  getPR.record().value(8).toString();
            pr->Subjects =  getPR.record().value(9).toString();
        }

    }
    else
    {
        qDebug()<<"Can't get PR";
        qDebug() << "SQL Select Error " << getPR.lastError().text();
        qDebug()<<getPR.lastQuery();
        return false;
    }

    return true;
}

QList<PR_Items> DbManager::getItems(int Pr)
{
    QList<PR_Items> itemList;
    PR_Items *item;
    QSqlQuery query(m_db->database("PR_Master"));
    query.prepare("SELECT "+QString(ITEM_CODE)+", "
            +QString(ITEM_DESC)+", "
            +QString(ITEM_UOM)+", "
            +QString(ITEM_QUANTITY)+", "
            +QString(ITEM_PROGRESS)+", "
            +QString(ITEM_STATUS)+", "
            +QString(ITEM_REMARKS)+", "
            +QString(ITEM_CLASS)
            +" FROM PR_Items Where "+
            QString(ITEM_PR_NUM)+" = :num");
    query.bindValue(":num", Pr);
    if(query.exec())
    {
       while(query.next())
       {
           item = new PR_Items;
           item->Item_Code = query.record().value(0).toString();
           item->Description = query.record().value(1).toString();
           item->UOM = query.record().value(2).toString();
           item->Quantity = query.record().value(3).toInt();
           item->Progress = query.record().value(4).toInt();
           item->Status = query.record().value(5).toString();
           item->Remarks = query.record().value(6).toString();
           item->Classification = query.record().value(7).toString();
           itemList.append(*item);
       }
    }
    else
    {
        qDebug()<<"Can't get PR Items";
        qDebug() << "SQL Select Error " << query.lastError().text();
        qDebug()<<query.lastQuery();
    }
    return itemList;
}

bool DbManager::addPRBatch(QList<PR_Number> PR)
{
    //////////////////////////////////////////////
    ///
    //////////////////////////////////////////////

    QSqlQuery query(m_db->database("PR_Master"));
    query.prepare("INSERT INTO PR_Tracker (" + QString(PR_NUM)
                                            +", "+QString(PR_SUBMIT_DATE)
                                            +", "+QString(PR_DEPARTMENT)
                                            +", "+QString(PR_PURPOSE)
                                            +", "+QString(PR_STATUS)
                                            +", "+QString(PR_REMARKS)
                                            +", "+QString(PR_TAG)
                                            +", "+QString(PR_SUBJECT)
                                            +") VALUES (:num, :date, :dep, :pur, :stat, :rem, :tag, :sub)");

   QVariantList num;
   QVariantList date,dep,pur,stat,rem,tag,sub;
    foreach (PR_Number pp, PR) {
        num.append(pp.PR_Number);
        date.append(pp.Submit_Date.toString(Qt::ISODate));
        dep.append(pp.Department);
        pur.append(pp.Purpose);
        stat.append(pp.Status);
        rem.append(pp.Remarks);
        tag.append(pp.TAG);
        sub.append(pp.Subjects);
    }
    query.bindValue(":num",num);
    query.bindValue(":date", date);
    query.bindValue(":dep",dep);
    query.bindValue(":pur",pur);
    query.bindValue(":stat", stat);
    query.bindValue(":rem",rem);
    query.bindValue(":tag", tag);
    query.bindValue(":sub",sub);

    if(!query.execBatch())
    {
        qDebug()<<query.executedQuery();
            qDebug() << "SQL Add PR failed: " << query.lastError().text();
            return false;
    }
    return false;


    //////////////////////////////////////////////
    ///
    //////////////////////////////////////////////


}
