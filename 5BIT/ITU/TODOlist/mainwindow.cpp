#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QFile f(":/qdarkstyle/style.qss");
    if (!f.exists())
    {
        printf("Unable to set stylesheet, file not found\n");
    }
    else
    {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        qApp->setStyleSheet(ts.readAll());
    }

    DataLoc << "";
    //this->setStyleSheet("background-color: white;");
    //ui->centralWidget->setStyleSheet("background-color: white;");
    tray = new QSystemTrayIcon;
    trayMenu = new QMenu;
    trayMenu->addAction("Skryť");
    trayMenu->addAction("Zobraz");
    trayMenu->addAction("Ukončiť");
    tray->setContextMenu(trayMenu);
    tray->setIcon(QIcon(":/images/icon.png"));
    tray->show();
    ui->editTask_button->setDisabled(true);
    //ui->menuHelp->setStyleSheet("color: rgb(0, 0, 0)");
    //ui->editTask_button->setStyleSheet("border-image:url(:/images/button-grey.png); color: black");
    //ui->addTask_button->setStyleSheet("border-image:url(:/images/button-grey.png); color: black");
    //ui->addCategory_button->setStyleSheet("border-image:url(:/images/button-grey.png); color: black");
    //ui->removeCategory_button->setStyleSheet("border-image:url(:/images/button-grey.png); color: black");
    //ui->removeTask_button->setStyleSheet("border-image:url(:/images/button-grey.png); color: black");
    //ui->login_button->setStyleSheet("border-image:url(:/images/button-grey.png); color: black");
    //ui->tableView->setStyleSheet("background-color: white;");
    ui->removeTask_button->setDisabled(true);
    ui->removeCategory_button->setDisabled(true);
    ui->notes->setWordWrap(true);
    connect(ui->tableView, SIGNAL(clicked(QModelIndex)), this, SLOT(show_description()));
    connect(ui->tableView, SIGNAL(clicked(QModelIndex)), this, SLOT(enableRemoveTask_button()));
    connect(ui->tableView_2, SIGNAL(clicked(QModelIndex)), this, SLOT(enableRemoveCategory_button()));
    connect(ui->tableView, SIGNAL(clicked(QModelIndex)), this, SLOT(enableEditTask_button()));
    connect(trayMenu, SIGNAL(triggered(QAction*)), this, SLOT(trayMenu_triggered(QAction*)));
    init_db();

    QString querz="select * from taskis";
    QSqlQuery query (querz,db);
    query.exec();

    while (query.next()) {
           QString name = query.value(0).toString();
           QString time = query.value(1).toString();

           QDateTime now = QDateTime::currentDateTime();
           QDateTime then=QDateTime::fromString(time,"ddd dd.MM.yyyy, hh:mm");
           int pocet_sekund=now.secsTo(then);

           if (pocet_sekund>0) {
               QTimer::singleShot(1000*(pocet_sekund+2), this, SLOT(spusti_alarm()));
           }
       }
}

void MainWindow::show_description() {

    if(ui->tableView->currentIndex().isValid()) {
        ui->task->setText(ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0)).toString());
        ui->time->setText(ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(), 1)).toString());
        ui->done->setText(ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(), 2)).toString());
        ui->notes->setText(ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(), 4)).toString());

    }
}

void MainWindow::restoreGUI() {

}

void MainWindow::closeEvent(QCloseEvent *event)
{
    qDebug() << "closeEvent(): triggered()";
    db.exec("VACUUM");
    db.close();
    tray->hide();
    event->accept();
}

// private slots
void MainWindow::on_actionReport_A_Bug_triggered()
{
    QDesktopServices::openUrl(QUrl("mailto:xgallo03@stud.fit.vutbr.cz"));
}

void MainWindow::on_actionQuit_triggered()
{
    this->close();
}


void MainWindow::on_actionCheck_For_Update_triggered()
{
    QMessageBox::information(this, "Aktualizácia", "Vaša verzia je aktuálna!");
}

void MainWindow::on_actionDonate_triggered()
{
    QDesktopServices::openUrl(QUrl("http://www.paypalmoneyfree.com/wp-content/uploads/2016/06/paypal_ITSS-1.jpg"));
}

void MainWindow::on_actionLicense_triggered()
{
    LicenseDialog *licenseDialog = new LicenseDialog;
    licenseDialog->exec();
    licenseDialog->deleteLater();
}

void MainWindow::trayMenu_triggered(QAction *a)
{
    if(a->text().compare("Zobraz") == 0)
        this->show();
    else if(a->text().compare("Skryť") == 0)
        this->hide();
    else if(a->text().compare("Ukončiť") == 0)
        this->close();
}

void MainWindow::on_addTask_button_clicked()
{
    insertItem *dialogBox = new insertItem;
    connect(dialogBox, SIGNAL(finished(int)), dialogBox, SLOT(deleteLater()));

    if( dialogBox->exec() )
    {

        QSqlQuery query ("insert into taskis (Task, DueDate, Completed, Priority, Notes, Category) values (:name, :dueDate, :completed, :priority, :notes, :category)",db);
        query.bindValue(0, dialogBox->get_task());
        query.bindValue(1, dialogBox->get_dueDate());
        query.bindValue(2, dialogBox->get_isCompleted());
        query.bindValue(4, dialogBox->get_notes());
        query.bindValue(5,category);
        if(query.exec())
        {
            updateTable();
        }

        QDateTime now = QDateTime::currentDateTime();
        QDateTime then=QDateTime::fromString(dialogBox->get_dueDate(),"ddd dd.MM.yyyy, hh:mm");

        int pocet_sekund=now.secsTo(then);

        QString task=dialogBox->get_task();
        QTimer::singleShot(1000*(pocet_sekund+2), this, SLOT(spusti_alarm()));

        ui->editTask_button->setDisabled(true);
        ui->removeTask_button->setDisabled(true);
    }
}

void MainWindow::spusti_alarm() {
    QDateTime now = QDateTime::currentDateTime();
    QString str=now.toString("ddd dd.MM.yyyy, hh:mm");
    QString querz="select * from taskis where dueDate='"+str+"'";
    QSqlQuery query (querz,db);
    query.exec();
    QString name;
    int found=0;
    while (query.next()) {
        found=1;
           name = query.value(0).toString();
       }
     qDebug() << "prave spustam alarm pre " << name;

    if (found==1) {
        Alarm *alarmBox = new Alarm;
        connect(alarmBox, SIGNAL(finished(int)), alarmBox, SLOT(deleteLater()));

        alarmBox->set_labelName(name);
        alarmBox->set_labelDate(str);

        alarmBox->exec();
    }
}

void MainWindow::on_addCategory_button_clicked()
{
    addCategory *dialogBox = new addCategory;
    connect(dialogBox, SIGNAL(finished(int)), dialogBox, SLOT(deleteLater()));

    if( dialogBox->exec() )
    {
        QSqlQuery query ("insert into categories (Category) values (:name)",db2);
        query.bindValue(0, dialogBox->get_name());
        if(query.exec())
        {
            updateTable();
        }
    }
    ui->removeCategory_button->setDisabled(true);
}

void MainWindow::on_login_button_clicked()
{
    loginScreen *dialogBox = new loginScreen;
    connect(dialogBox, SIGNAL(finished(int)), dialogBox, SLOT(deleteLater()));
    dialogBox->exec();

}

void MainWindow::on_removeTask_button_clicked()
{
    if(QMessageBox::No == QMessageBox::question(this, "Odstrániť úlohu", "Naozaj chcete odstrániť zvolenú úlohu?"))
    {
        return;
    }
    QSqlQuery query(db);
    query.prepare("delete from taskis where Task = \'" + ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0)).toString().replace("'", "''") +"\'");
    if(!query.exec())
    {
        QMessageBox::warning(0,"Chyba", "Nemôžem odstrániť požadovanú úlohu, prosím kontaktujte podporu: " + ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0)).toString() + ".");
        return;
    }
    updateTable();
    ui->editTask_button->setDisabled(true);
    ui->removeTask_button->setDisabled(true);
}

void MainWindow::on_editTask_button_clicked()
{
    editTask *dialog = new editTask;
    connect(dialog, SIGNAL(finished(int)), dialog, SLOT(deleteLater()));

    dialog->set_taskName(ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0)).toString());

    dialog->set_dueDate(QDateTime::fromString(ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(), 1)).toString(), "ddd dd.MM.yyyy, hh:mm"));

    QString completed = ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(), 2)).toString();

    if(completed.compare("No") == 0)
        dialog->set_completed(0);
    else
        dialog->set_completed(1);

    dialog->set_notes(ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(), 4)).toString()); //set Notes
    if(dialog->exec())
    {
        qDebug() <<  ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0)).toString();
        QSqlQuery query("update taskis set Task=:name,DueDate=:dueDate,Completed=:completed,Priority=:priority,Notes=:notes,Category=:category WHERE Task=\'" + ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0)).toString().replace("'", "''") + "\'",db);
        query.bindValue(0, dialog->get_taskName());
        query.bindValue(1, dialog->get_dueDate());
        query.bindValue(2, dialog->get_completed());
        query.bindValue(4, dialog->get_notes());
        query.bindValue(5,category);

        qDebug() << "prave spustam alarm pre " << dialog->get_dueDate();
        query.exec();

    }
    updateTable();
}

void MainWindow::on_removeCategory_button_clicked()
{
    if(QMessageBox::No == QMessageBox::question(this, "Odstrániť kategoriu", "Naozaj chcete odstrániť zvolenú kategoriu?"))
    {
        return;
    }
    QSqlQuery query(db);
    query.prepare("delete from categories where Category = \'" + ui->tableView_2->model()->data(ui->tableView_2->model()->index(ui->tableView_2->currentIndex().row(), 0)).toString().replace("'", "''") +"\'");
    if(!query.exec())
    {
        QMessageBox::warning(0,"Chyba", "Nemôžem odstrániť požadovanú kategoriu, prosím kontaktujte podporu: " + ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0)).toString() + ".");
        return;
    }
    updateTable();
    //disable the edit button because we just deleted a task so we are no longer on a task making this button invalid currently
    ui->editTask_button->setDisabled(true);
    ui->removeTask_button->setDisabled(true);
    ui->removeCategory_button->setDisabled(true);
}

void MainWindow::on_tableView_doubleClicked(QModelIndex index)
{
    qDebug() << ui->tableView->currentIndex().column();
    if (ui->tableView->currentIndex().column()==2) {
        QString done=ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(), 2)).toString();
        if (done.compare("Áno")==0) {
            QSqlQuery query("update taskis set Completed='Nie' WHERE Task=\'" + ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0)).toString().replace("'", "''") + "\'",db);
            query.exec();
            ui->done->setText("Nie");
        } else if (done.compare("Nie")==0) {
            QSqlQuery query("update taskis set Completed='Áno' WHERE Task=\'" + ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0)).toString().replace("'", "''") + "\'",db);
            query.exec();
            ui->done->setText("Áno");
        }
        updateTable();
    } else {
        on_editTask_button_clicked();
    }

}

void MainWindow::on_tableView_2_doubleClicked(QModelIndex index)
{
    ui->removeCategory_button->setDisabled(true);
    category= ui->tableView_2->model()->data(ui->tableView_2->model()->index(ui->tableView_2->currentIndex().row(), 0)).toString();
    updateTable();
}


void MainWindow::on_actionHide_triggered()
{
    this->hide();
}

void MainWindow::enableRemoveTask_button()
{
    if(ui->tableView->currentIndex().isValid())
        ui->removeTask_button->setEnabled(true);
}

void MainWindow::enableEditTask_button()
{
    if(ui->tableView->currentIndex().isValid())
        ui->editTask_button->setEnabled(true);
}

void MainWindow::enableRemoveCategory_button()
{
    if(ui->tableView_2->currentIndex().isValid())
        ui->removeCategory_button->setEnabled(true);
}

//table
void MainWindow::init_db()
{

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(DataLoc.at(0) + "/tasks.db");
    db.open();
    db.exec("create table if not exists taskis (Task, DueDate, Completed, Priority, Notes, Category)");
    db.exec("create table if not exists categories (Category)");
    db.exec("create table if not exists sort_preferences (id PRIMARY KEY, row, type)");

    all_model = new QSqlTableModel(this, db);
    all_model2 = new QSqlTableModel(this, db);
    ui->tableView->setModel(all_model);
    updateTable();

}

void MainWindow::updateTable()
{


    QString querz="select * from taskis where category='"+category+"'";
    search_model.setQuery(querz,db);
    ui->tableView->setModel(&search_model);

    ui->tableView->setColumnWidth(0, 298);
    ui->tableView->setColumnWidth(1, 30);
    ui->tableView->setColumnWidth(2, 60);
    ui->tableView->setColumnWidth(3, 334);

    ui->tableView->setColumnHidden(1, true);
    ui->tableView->setColumnHidden(3, true);
    ui->tableView->setColumnHidden(4, true);
    ui->tableView->setColumnHidden(5, true);

    ui->tableView->horizontalHeader()->hide();
    ui->tableView->verticalHeader()->hide();
    //ui->tableView->setStyleSheet("color: rgb(0, 0, 0);background-color: #f8ff82");
    ui->tableView->setShowGrid(false);

    all_model2->setTable("categories");
    all_model2->select();
    ui->tableView_2->setModel(all_model2);
    //ui->tableView_2->setStyleSheet("color: rgb(0, 0, 0);background-color: #f8ff82");
    ui->tableView_2->setColumnWidth(0, 230);

    ui->tableView_2->horizontalHeader()->hide();
    ui->tableView_2->verticalHeader()->hide();
    ui->tableView_2->setShowGrid(false);

}

MainWindow::~MainWindow()
{
    delete all_model;
    delete ui;
    delete tray;
}
