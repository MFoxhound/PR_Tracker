#ifndef DBMANAGER_H
#define DBMANAGER_H
#include <QObject>
#include <QSql>
#include <QSqlDatabase>
//#include <QWidget>
#include <QString>
#include "Items_Definition.h"


class DbManager // : public QObject
{
//    Q_OBJECT
public:
    DbManager( QString &path);
    ~DbManager();
    int insert_PR();
    bool addPR(PR_Number* PR);
    bool addPRBatch(QList<PR_Number> pr);
    bool addItem(PR_Items* Item);
    bool updatePRItemCount(int pR,int Count);
    QString LastError();
    QList<int> GetPRNumber();
    bool getPR(int prNum,PR_Number *pr);
    QList<PR_Items> getItems(int Pr);
    QList<QString> GetPRItems(int PR);
private:
    QSqlDatabase *m_db;
    QString lastErr;


};

#endif // DBMANAGER_H
