#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndexList>
#include "dbmanager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_actionConnect_To_Database_triggered();

    void on_actionAdd_From_Excel_File_triggered();

    void on_listView_clicked(const QModelIndex &index);

    void on_lineEdit_returnPressed();

//    void on_lineEdit_textChanged(const QString &arg1);

    void on_lineEdit_textEdited(const QString &arg1);

//    void on_listView_indexesMoved(const QModelIndexList &indexes);

    void on_listView_activated(const QModelIndex &index);

    void on_action_Edit_PR_triggered();

    void on_actionEdit_Items_triggered();

    void on_actionEdit_current_PR_triggered();

private:
    void InitPR(PR_Number *PR);
    void InitPRItems(PR_Items *Items);
    bool updateView(int prNum);
    bool saveViews();
    PR_Number *currentPR;
    Ui::MainWindow *ui;
    DbManager *db = Q_NULLPTR;
};

#endif // MAINWINDOW_H
