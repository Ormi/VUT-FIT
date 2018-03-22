#ifndef ADDCATEGORY_H
#define ADDCATEGORY_H

#include <QDialog>
#include <QtWidgets>

namespace Ui {
class addCategory;
}

class addCategory : public QDialog
{
    Q_OBJECT

public:
    explicit addCategory(QWidget *parent = 0);
    ~addCategory();

    QString get_name();

private slots:
    void checkCategoryText();

private:
    Ui::addCategory *ui;
};

#endif // ADDCATEGORY_H
