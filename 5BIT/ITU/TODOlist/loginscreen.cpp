#include "loginscreen.h"
#include "ui_loginscreen.h"

loginScreen::loginScreen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::loginScreen)
{
    ui->setupUi(this);
    ui->lineEdit_2->setEchoMode(QLineEdit::Password);
    ui->label_3->setText("<a href=\"http://www.google.com/\">Nemáte účet? Registrujte sa kliknutím sem</a>");
    ui->label_3->setTextFormat(Qt::RichText);
    ui->label_3->setTextInteractionFlags(Qt::TextBrowserInteraction);
    ui->label_3->setOpenExternalLinks(true);
}

loginScreen::~loginScreen()
{
    delete ui;
}
