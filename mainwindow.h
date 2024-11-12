#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QLabel>
#include <QListWidgetItem>
#include <QPlainTextEdit>
#include <QStackedWidget>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionAbout_triggered();

    void on_actionFind_triggered();

    void on_actionReplace_triggered();

    void on_actionNew_triggered();

    void on_actionOpenFile_triggered();

    void on_actionSave_triggered();

    void on_actionSaveAs_triggered();



    void on_listWidget_itemClicked(QListWidgetItem *item);



    void on_actionUndo_triggered();

    void on_actionCut_triggered();

    void on_actionCopy_triggered();

    void on_actionPaste_triggered();

    void on_actionRedo_triggered();

    void on_actionFontColor_triggered();

    void on_actionBackgroundColor_triggered();


    void on_actionShowToolBar_triggered();

    void on_actionShowStatusBar_triggered();

    void on_actionExit_triggered();

    void on_actionSelectAll_triggered();

    void on_actionLineWrap_triggered();

    void on_actionFont_triggered();


    void onCopyAvailable(bool b);
    void onRedoAvailable(bool b);
    void onUndoAvailable(bool b);


private:
    Ui::MainWindow *ui;
    QLabel statusCursorLabel;
    QLabel statusLabel;
    QPlainTextEdit * curEdit;
    QListWidgetItem *curWidgetItem;
    QStackedWidget* stackedWidget;
    int untitledCount;
};
#endif // MAINWINDOW_H
