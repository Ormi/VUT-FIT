#include "addcategory.h"
#include "ui_addcategory.h"

addCategory::addCategory(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addCategory)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText("Zrušiť");
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(checkCategoryText()));
}

QString addCategory::get_name()
{
    return ui->lineEdit->text();
}

void addCategory::checkCategoryText()
{
    if(ui->lineEdit->text().isEmpty())
    {
        QMessageBox::warning(this, "Error", "Meno je prazdne");
    }
    else
    {
        addCategory::accept();
    }
}

addCategory::~addCategory()
{
    delete ui;
}
