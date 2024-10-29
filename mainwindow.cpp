#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    statusLabel.setMaximumWidth(150);
    statusLabel.setText("length: "+QString::number(0)+"  Line:  "+QString::number(1));
    ui->statusbar->addPermanentWidget(&statusLabel);
    statusCursorLabel.setMaximumWidth(150);
    statusCursorLabel.setText("Ln: "+QString::number(0)+"  col:  "+QString::number(1));
    ui->statusbar->addPermanentWidget(&statusCursorLabel);
    QLabel* author = new QLabel("黄金海");
    ui->statusbar->addPermanentWidget(author);
}

MainWindow::~MainWindow()
{
    delete ui;
}
