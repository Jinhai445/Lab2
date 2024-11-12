#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "searchdialog.h"
#include "replacedialog.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>
#include <QPushButton>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QScrollBar>
#include <QStackedWidget>
#include <QColorDialog>
#include <QFontDialog>
#include <QTimer>
#include <QToolTip>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    stackedWidget = new QStackedWidget();
    ui->centralwidget->layout()->addWidget(stackedWidget);
    QScrollBar *ScrollBar = ui->listWidget->horizontalScrollBar();
    ScrollBar->setStyleSheet("QScrollBar:horizontal { height: 10px; }");


    untitledCount = 0;
    statusLabel.setMaximumWidth(150);
    statusLabel.setText("length: "+QString::number(0)+"  Line:  "+QString::number(1));
    ui->statusbar->addPermanentWidget(&statusLabel);
    statusCursorLabel.setMaximumWidth(150);
    statusCursorLabel.setText("Ln: "+QString::number(0)+"  col:  "+QString::number(1));
    ui->statusbar->addPermanentWidget(&statusCursorLabel);
    QLabel* author = new QLabel("黄金海");
    ui->statusbar->addPermanentWidget(author);
    ui->actionShowToolBar->setChecked(true);
    ui->actionShowStatusBar->setChecked(true);

    ui->actionCopy->setEnabled(false);
    ui->actionCut->setEnabled(false);
    ui->actionPaste->setEnabled(false);
    ui->actionUndo->setEnabled(false);
    ui->actionRedo->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionAbout_triggered()
{
    AboutDialog aboutDialog;
    aboutDialog.setWindowTitle("关于");
    aboutDialog.exec();
}


void MainWindow::on_actionFind_triggered()
{
    SearchDialog searchDialog;
    searchDialog.setWindowTitle("查找");
    searchDialog.exec();
}


void MainWindow::on_actionReplace_triggered()
{
    ReplaceDialog replaceDialog;
    replaceDialog.setWindowTitle("替换");
    replaceDialog.exec();
}


void MainWindow::on_actionNew_triggered()
{
    QListWidgetItem *item = new QListWidgetItem();
    QPushButton *deleteButton = new QPushButton("×");
    QFont font = deleteButton->font();
    QWidget *widget = new QWidget();
    QHBoxLayout *hLayout = new QHBoxLayout(widget);
    QPlainTextEdit* Edit = new QPlainTextEdit();
    QLabel* label = new QLabel("Untitled-"+QString::number(++untitledCount));

    item->setSizeHint(QSize(100, 50));
    font.setPointSize(8); // 设置字体大小为 20
    deleteButton->setFont(font); // 应用新的字体
    deleteButton->setStyleSheet("border: none;");
    hLayout->addWidget(label);
    hLayout->addWidget(deleteButton);
    widget->setLayout(hLayout);


    Edit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    stackedWidget->addWidget(Edit);
    stackedWidget->setCurrentWidget(Edit);

    //item存储数据
    //Qt::UserRole 为对应QPlainText指针
    //Qt::UserRole+1 为打开文件路径
    //Qt::UserRole+2 为文件是否修改的标志  ，0为否，1为是
    item->setData(Qt::UserRole,QVariant::fromValue(static_cast<void*>(Edit)));
    qDebug()<<QVariant::fromValue(static_cast<void*>(Edit));
    item->setData(Qt::UserRole+1,"");
    item->setData(Qt::UserRole+2,0);

    ui->listWidget->addItem(item);
    ui->listWidget->setItemWidget(item, widget);

    ui->listWidget->setCurrentItem(item);

    connect(deleteButton, &QPushButton::clicked, [this, item]() {
        qDebug()<<item;
        qDebug()<<"del"<<static_cast<QPlainTextEdit*>(item->data(Qt::UserRole).value<void*>());
        stackedWidget->removeWidget(static_cast<QPlainTextEdit*>(item->data(Qt::UserRole).value<void*>()));
        delete item;
    });
    //文件修改，对应文件状态修改
    connect(Edit, &QPlainTextEdit::textChanged, [this, item]() {
        int isChanged = item->data(Qt::UserRole+2).toInt();
        if(isChanged == 1){
            return;
        }
        //获取Label对象并修改文字
        QWidget *associatedWidget = ui->listWidget->itemWidget(item);
        QLabel* la =  qobject_cast<QLabel *>(associatedWidget->layout()->itemAt(0)->widget());
        la->setText("*"+la->text());
        QPoint center = ui->listWidget->rect().center();
        QPoint globalCenter = ui->listWidget->mapToGlobal(center);
        QToolTip::showText(globalCenter, "文本已修改");
        // 设置定时器，2秒后隐藏工具提示
        QTimer::singleShot(10000, []() {
            QToolTip::hideText();  // 隐藏工具提示
        });

        item->setData(Qt::UserRole+2,1);


    });
    //关联复制粘贴等操作的槽函数
    connect(Edit, &QPlainTextEdit::copyAvailable, this, &MainWindow::onCopyAvailable);
    connect(Edit, &QPlainTextEdit::redoAvailable, this, &MainWindow::onRedoAvailable);
    connect(Edit, &QPlainTextEdit::undoAvailable, this, &MainWindow::onUndoAvailable);
    curWidgetItem = item;
    curEdit = Edit;
}


// QMessageBox *msgBox = new QMessageBox();
// msgBox->setIcon(QMessageBox::Information);  // 设置消息框图标
// msgBox->setText("TEXT CHANGED");  // 设置消息文本
// msgBox->setWindowTitle("INFO");  // 设置窗口标题
// // 使用定时器，自动关闭消息框
// QTimer::singleShot(900, msgBox, &QMessageBox::close);
// ui->listWidget->addItem(item);
// ui->listWidget->setItemWidget(item, widget);
// ui->listWidget->setCurrentItem(item);
// if(untitledCount==1){
//     ui->centralwidget->layout()->addWidget(qvariant_cast<QPlainTextEdit*>(item->data(Qt::UserRole)));
//     text =  qvariant_cast<QPlainTextEdit*>(item->data(Qt::UserRole));
// }else{
//     ui->centralwidget->layout()->removeWidget(text);

//     ui->centralwidget->layout()->addWidget(qvariant_cast<QPlainTextEdit*>(item->data(Qt::UserRole)));
// }
// //QPushButton* fileButton = new QPushButton("Untitled-"+QString::number(++untitledCount));
// // MyFile* newfile = new MyFile();
// // QString* newfileName =new QString("Untitled-"+QString::number(++untitledCount));
// // newfile->setFileName(newfileName);
// // if(curFile != nullptr){
// //     ui->centralwidget->layout()->removeWidget(curFile->getLineEdit());
// // }
// // curFile = newfile;
//
// //widget->setMinimumWidth(200);
// QListWidgetItem *item = new QListWidgetItem("Item");

// //
//
// // ui->centralwidget->layout()->addWidget(curFile->getLineEdit());
// // ui->listWidget->addItem(*newfile->getFileName());
// //ui->textEdit->clear();

// // this->setWindowTitle("新建文本文件");
void MainWindow::on_actionOpenFile_triggered()
{
    QString filePath = QFileDialog::getOpenFileName(this,tr("打开文件"), ".",
            tr("文本文件 (*.txt);;所有文件 (*.*)"));
    QFile file(filePath);
    if(!file.open(QFile::ReadOnly|QFile::Text)){
        QMessageBox::warning(this,
                             tr("警告"),
                             tr("文件打开失败！"),
                             QMessageBox::Ok);
        return;
    }
    QTextStream in(&file);
    QString text = in.readAll();

    //创建对应文件的QListWidgetItem，Widget 和 QPlainTextEdit
    QListWidgetItem *item = new QListWidgetItem();
    QPushButton *deleteButton = new QPushButton("×");
    QFont font = deleteButton->font();
    QWidget *widget = new QWidget();
    QHBoxLayout *hLayout = new QHBoxLayout(widget);
    QPlainTextEdit* Edit = new QPlainTextEdit();
    QLabel* label = new QLabel(QFileInfo(filePath).fileName());

    item->setSizeHint(QSize(100, 50));
    font.setPointSize(8); // 设置字体大小为 20
    deleteButton->setFont(font); // 应用新的字体
    deleteButton->setStyleSheet("border: none;");
    hLayout->addWidget(label);
    hLayout->addWidget(deleteButton);
    widget->setLayout(hLayout);
    Edit->setPlainText(text);


    Edit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    stackedWidget->addWidget(Edit);
    stackedWidget->setCurrentWidget(Edit);

    //item存储数据
    //Qt::UserRole 为对应QPlainText指针
    //Qt::UserRole+1 为打开文件路径
    //Qt::UserRole+2 为文件是否修改的标志  ，0为否，1为是
    item->setData(Qt::UserRole,QVariant::fromValue(static_cast<void*>(Edit)));
    qDebug()<<QVariant::fromValue(static_cast<void*>(Edit));
    item->setData(Qt::UserRole+1,filePath);
    item->setData(Qt::UserRole+2,0);

    ui->listWidget->addItem(item);
    ui->listWidget->setItemWidget(item, widget);

    ui->listWidget->setCurrentItem(item);
    connect(deleteButton, &QPushButton::clicked, [this, item]() {
        qDebug()<<item;
        qDebug()<<"del"<<static_cast<QPlainTextEdit*>(item->data(Qt::UserRole).value<void*>());
        stackedWidget->removeWidget(static_cast<QPlainTextEdit*>(item->data(Qt::UserRole).value<void*>()));
        delete item;
    });
    connect(Edit, &QPlainTextEdit::textChanged, [this, item]() {
        int isChanged = item->data(Qt::UserRole+2).toInt();
        if(isChanged == 1){
            return;
        }
        //获取Label对象并修改文字
        QWidget *associatedWidget = ui->listWidget->itemWidget(item);
        QLabel* la =  qobject_cast<QLabel *>(associatedWidget->layout()->itemAt(0)->widget());
        la->setText("*"+la->text());
        QPoint center = ui->listWidget->rect().center();
        QPoint globalCenter = ui->listWidget->mapToGlobal(center);
        QToolTip::showText(globalCenter, "文本已修改");
        // 设置定时器，2秒后隐藏工具提示
        QTimer::singleShot(10000, []() {
            QToolTip::hideText();  // 隐藏工具提示
        });
        item->setData(Qt::UserRole+2,1);

    });
    //关联复制粘贴等操作的槽函数
    connect(Edit, &QPlainTextEdit::copyAvailable, this, &MainWindow::onCopyAvailable);
    connect(Edit, &QPlainTextEdit::redoAvailable, this, &MainWindow::onRedoAvailable);
    connect(Edit, &QPlainTextEdit::undoAvailable, this, &MainWindow::onUndoAvailable);
    //将当前操作的文件替换
    curWidgetItem = item;
    curEdit = Edit;


}


void MainWindow::on_actionSave_triggered()
{
    //获取当前文件的路径
    QString filePath = curWidgetItem->data(Qt::UserRole+1).toString();
    qDebug()<<"curopath"<<filePath;
    //文件未保存，文件路径不存在
    if(filePath==nullptr||filePath==""){
        on_actionSaveAs_triggered();
        return;
    }
    //文件路径存在
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out <<qobject_cast<QPlainTextEdit*>(stackedWidget->currentWidget())->toPlainText();
        file.flush();
        file.close();
        QMessageBox::information(this, tr("成功"), tr("文件已成功保存到：%1").arg(filePath));
    } else {
        QMessageBox::warning(this, tr("警告"), tr("无法保存文件。"));
    }
    //修改是否修改的标志
    curWidgetItem->setData(Qt::UserRole+2,0);

    //文件如果已保存，修改窗口文件名
    if(curWidgetItem->data(Qt::UserRole+1).toInt()==0){
        //获取Label对象并修改文字
        QWidget *associatedWidget = ui->listWidget->itemWidget(curWidgetItem);
        QLabel* la =  qobject_cast<QLabel *>(associatedWidget->layout()->itemAt(0)->widget());
        QString oriName =la->text();
        if(oriName.startsWith("*")){
            la->setText(oriName.right(oriName.length()-1));
        }
    }


}


void MainWindow::on_actionSaveAs_triggered()
{

    QString orifilePath = curWidgetItem->data(Qt::UserRole+1).toString();
    QString orifileName = "";
    qDebug()<<orifileName<<orifilePath<<"ew";
    if(orifilePath!=nullptr&&orifilePath!=""){
        orifileName = QFileInfo(orifilePath).fileName();
    }
    qDebug()<<orifileName<<orifilePath;
    QString filePath = QFileDialog::getSaveFileName(this,
                                                    tr("保存文件"), orifileName,
                                                    tr("文本文件 (*.txt);;所有文件 (*)"));
    QString fileName = QFileInfo(filePath).fileName();

    //未获得文件路径，失败返回
    if(filePath==nullptr||filePath.isEmpty()){
        QMessageBox::warning(this, tr("警告"), tr("无法保存文件"));
        return;
    }

    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out <<curEdit->toPlainText();
        file.flush();
        file.close();
        QMessageBox::information(this, tr("成功"), tr("文件已成功保存到：%1").arg(filePath));
    } else {
        QMessageBox::warning(this, tr("警告"), tr("无法保存文件。"));
    }

    //更新文件路径
    //文件修改状态变为未修改
    qDebug()<<"saveAs"<<filePath;
    curWidgetItem->setData(Qt::UserRole+1,filePath);
    curWidgetItem->setData(Qt::UserRole+2,0);


    //文件如果已保存，修改窗口文件名，去除前缀*
    if(curWidgetItem->data(Qt::UserRole+1).toInt()==0){
        //获取Label对象并修改文字
        QWidget *associatedWidget = ui->listWidget->itemWidget(curWidgetItem);
        QLabel* la =  qobject_cast<QLabel *>(associatedWidget->layout()->itemAt(0)->widget());
        QString oriName =la->text();
        if(oriName.startsWith("*")){
            la->setText(fileName);
        }
    }

}


void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{

    curEdit = static_cast<QPlainTextEdit*>(item->data(Qt::UserRole).value<void*>());
    curWidgetItem = item;
    stackedWidget->setCurrentWidget(static_cast<QPlainTextEdit*>(item->data(Qt::UserRole).value<void*>()));

}




void MainWindow::on_actionUndo_triggered()
{
   curEdit->undo();

}


void MainWindow::on_actionCut_triggered()
{
    curEdit->cut();
    ui->actionPaste->setEnabled(true);
}


void MainWindow::on_actionCopy_triggered()
{
    curEdit->copy();
    ui->actionPaste->setEnabled(true);
}


void MainWindow::on_actionPaste_triggered()
{
   curEdit->paste();
}


void MainWindow::on_actionRedo_triggered()
{
    curEdit->redo();
}
void MainWindow::on_actionSelectAll_triggered()
{
    curEdit->selectAll();
}


void MainWindow::on_actionFontColor_triggered()
{
    QColor color = QColorDialog::getColor(Qt::black,this,"选择颜色");
    if(color.isValid()){
        curEdit->setStyleSheet(QString("QPlainTextEdit {color:%1}").arg(color.name()));
    }
}




void MainWindow::on_actionBackgroundColor_triggered()
{
    QColor color = QColorDialog::getColor(Qt::black,this,"选择颜色");
    if(color.isValid()){
        curEdit->setStyleSheet(QString("QPlainTextEdit {background-color:%1}").arg(color.name()));
    }
}




void MainWindow::on_actionShowToolBar_triggered()
{
    bool visible = ui->toolBar->isVisible();
    ui->toolBar->setVisible(!visible);
    ui->actionShowToolBar->setChecked(!visible);
}


void MainWindow::on_actionShowStatusBar_triggered()
{
    bool visible = ui->statusbar->isVisible();
    ui->statusbar->setVisible(!visible);
    ui->actionShowStatusBar->setChecked(!visible);
}


void MainWindow::on_actionExit_triggered()
{
    exit(0);
}




void MainWindow::on_actionLineWrap_triggered()
{
    QPlainTextEdit::LineWrapMode mode = curEdit->lineWrapMode();
    if(mode == QTextEdit::NoWrap){
        curEdit->setLineWrapMode(QPlainTextEdit::WidgetWidth);
        ui->actionLineWrap->setChecked(true);
    }else{
        curEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
        ui->actionLineWrap->setChecked(false);
    }
}


void MainWindow::on_actionFont_triggered()
{
    bool ok = false;
    QFont font = QFontDialog::getFont(&ok,this);
    if(ok){
        curEdit->setFont(font);
    }
}

void MainWindow::onCopyAvailable(bool b){
    ui->actionCopy->setEnabled(b);
    ui->actionCut->setEnabled(b);
}

void MainWindow:: onRedoAvailable(bool b){
    ui->actionRedo->setEnabled(b);
}
void MainWindow:: onUndoAvailable(bool b){
    ui->actionUndo->setEnabled(b);
}
