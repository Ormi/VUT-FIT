#include "edittask.h"
#include "ui_edittask.h"

editTask::editTask(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::editTask)
{
    ui->setupUi(this);
}

editTask::~editTask()
{
    delete ui;
}

void editTask::set_taskName(QString name)
{
    ui->lineEdit->setText(name);
}

void editTask::set_dueDate(QDateTime dueDate)
{
    ui->dateTimeEdit->setDateTime(dueDate);
}

void editTask::set_completed(int index)
{
    ui->comboBox->setCurrentIndex(index);
}

void editTask::set_notes(QString notes)
{
    ui->textEdit->setText(notes);
}

QString editTask::get_taskName()
{
    return ui->lineEdit->text();
}

QString editTask::get_dueDate()
{
    return ui->dateTimeEdit->dateTime().toString("ddd dd.MM.yyyy, hh:mm");
}

QString editTask::get_completed()
{
    return ui->comboBox->currentText();
}

QString editTask::get_notes()
{
    return ui->textEdit->toPlainText();
}
