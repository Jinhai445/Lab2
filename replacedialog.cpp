#include "replacedialog.h"
#include "ui_replacedialog.h"
#include <QPlainTextEdit>
#include <QMessageBox>
ReplaceDialog::ReplaceDialog(QWidget *parent,QPlainTextEdit* textEdit)
    : QDialog(parent)
    , ui(new Ui::ReplaceDialog)
{
    ui->setupUi(this);
    curEdit = textEdit;
    ui->rbDown->setChecked(true);
}

ReplaceDialog::~ReplaceDialog()
{
    delete ui;
}

void ReplaceDialog::on_replaceButton_clicked()
{
    QString target = ui->searchText->text();
    QString to = ui->toReplace->text();
    if(curEdit!=nullptr&&target!=""&&to!=""){
        QString seltext = curEdit->textCursor().selectedText();
        if(seltext==target){
            curEdit->insertPlainText(to);
        }
        on_FindNext_clicked();
    }
}


void ReplaceDialog::on_ReplaceAllButton_clicked()
{
    QString target = ui->searchText->text();
    QString to = ui->toReplace->text();
    if(curEdit!=nullptr&&target!=""&&to!=""){
        QString seltext = curEdit->toPlainText();
        seltext.replace(target,to,ui->CaseSensitive->isChecked()?Qt::CaseInsensitive:
                                        Qt::CaseInsensitive);
        curEdit->clear();
        curEdit->insertPlainText(seltext);

    }
}


void ReplaceDialog::on_cancelButton_clicked()
{
    accept();
}


void ReplaceDialog::on_FindNext_clicked()
{
    QString target = ui->searchText->text();
    QString text = curEdit->toPlainText();
    if(target==""||curEdit==nullptr){
        return;
    }
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

