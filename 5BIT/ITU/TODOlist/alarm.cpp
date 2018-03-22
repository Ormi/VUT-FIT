#include "alarm.h"
#include "ui_alarm.h"

QSound song(":/sounds/alarm.wav");

Alarm::Alarm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Alarm)
{
    ui->setupUi(this);

    song.setLoops(QSound::Infinite);
    song.play();
}

Alarm::~Alarm()
{
    delete ui;
    song.stop();

}

void Alarm::set_labelName(QString name)
{
    ui->task->setText(name);
}

void Alarm::set_labelDate(QString dueDate)
{
    ui->time->setText(dueDate);
}



