#ifndef ALARM_H
#define ALARM_H

#include <QDialog>
#include <QSound>

namespace Ui {
class Alarm;
}

class Alarm : public QDialog
{
    Q_OBJECT

public:
    explicit Alarm(QWidget *parent = 0);
    ~Alarm();
    void set_labelName(QString);
    void set_labelDate(QString);


private:
    Ui::Alarm *ui;
};


#endif // ALARM_H
