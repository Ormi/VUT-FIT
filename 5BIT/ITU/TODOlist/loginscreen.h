#ifndef LOGINSCREEN_H
#define LOGINSCREEN_H

#include <QDialog>

namespace Ui {
class loginScreen;
}

class loginScreen : public QDialog
{
    Q_OBJECT

public:
    explicit loginScreen(QWidget *parent = 0);
    ~loginScreen();

private:
    Ui::loginScreen *ui;
};

#endif // LOGINSCREEN_H
