#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QtWidgets>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QSettings>
#include <QFileDialog>
#include <QSystemTrayIcon>
#include <QMenu>

#include "insertitem.h"
#include "addcategory.h"
#include "edittask.h"
#include "alarm.h"
#include "licensedialog.h"
#include "loginscreen.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected slots:
    void closeEvent(QCloseEvent *event);
    void restoreGUI();
    void show_description();

    // action Buttons
    void on_actionReport_A_Bug_triggered();
    void on_actionQuit_triggered();
    void on_actionCheck_For_Update_triggered();
    void on_actionDonate_triggered();
    void trayMenu_triggered(QAction *a);
    void on_actionLicense_triggered();
    void on_actionHide_triggered();
    void on_login_button_clicked();

    //push Buttons
    void on_addTask_button_clicked();
    void on_addCategory_button_clicked();
    void on_removeTask_button_clicked();
    void on_editTask_button_clicked();
    void on_removeCategory_button_clicked();

    //button hooks
    void enableRemoveTask_button();
    void enableEditTask_button();
    void enableRemoveCategory_button();

    //table
    void init_db();
    void updateTable();
    void on_tableView_doubleClicked(QModelIndex);
    void on_tableView_2_doubleClicked(QModelIndex);
    void spusti_alarm();


protected:
    QStringList DataLoc;
    Ui::MainWindow *ui;
    QSqlDatabase db;
    QSqlDatabase db2;
    QSqlTableModel *all_model;
    QSqlTableModel *all_model2;
    QSqlQueryModel search_model;
    QDir dir;
    QSettings *settings;
    QSystemTrayIcon *tray;
    QMenu *trayMenu;
    QString category;
};

#endif // MAINWINDOW_H
