#ifndef REPLACEDIALOG_H
#define REPLACEDIALOG_H

#include <QDialog>
#include <QPlainTextEdit>
namespace Ui {
class ReplaceDialog;
}

class ReplaceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReplaceDialog(QWidget *parent = nullptr,QPlainTextEdit* textEdit=nullptr);
    ~ReplaceDialog();

private slots:
    void on_replaceButton_clicked();

    void on_ReplaceAllButton_clicked();

    void on_cancelButton_clicked();

    void on_FindNext_clicked();

private:
    Ui::ReplaceDialog *ui;
    QPlainTextEdit* curEdit;
};

#endif // REPLACEDIALOG_H
