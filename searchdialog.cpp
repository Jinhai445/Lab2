#include "searchdialog.h"
#include "ui_searchdialog.h"
#include <QPlainTextEdit>
#include <QMessageBox>
SearchDialog::SearchDialog(QWidget *parent,QPlainTextEdit* textEdit)
    : QDialog(parent)
    , ui(new Ui::SearchDialog)
{
    ui->setupUi(this);
    this->curEdit = textEdit;
    ui->rbDown->setChecked(true);
}

SearchDialog::~SearchDialog()
{
    delete ui;
}

void SearchDialog::on_findNext_clicked()
{
    QString target = ui->searchText->text();
    QString text = curEdit->toPlainText();
    QTextCursor c = curEdit->textCursor();


    int index = -1;
    if(ui->rbDown->isChecked()){
        index = text.indexOf(target,c.position(),ui->CaseSensitive->isChecked()?Qt::CaseSensitive:Qt::CaseInsensitive);
        if(index>=0){
            c.setPosition(index);
            c.setPosition(index+target.length(),QTextCursor::KeepAnchor);

            curEdit->setTextCursor(c);
        }
    }else if(ui->rbUp->isChecked()){
        index = text.lastIndexOf(target,c.position()-text.length()-1,ui->CaseSensitive->isChecked()?Qt::CaseSensitive:Qt::CaseInsensitive);
        if(index>=0){
            c.setPosition(index+target.length());
            c.setPosition(index,QTextCursor::KeepAnchor);

            curEdit->setTextCursor(c);
        }
    }
    if(index<0){
        QMessageBox msg(this);
        msg.setWindowTitle("error");
        msg.setText("找不到"+target);
        msg.setIcon(QMessageBox::Information);
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
    }

}


void SearchDialog::on_cancel_clicked()
{
    accept();
}

