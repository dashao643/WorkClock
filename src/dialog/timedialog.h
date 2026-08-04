#ifndef TIMEDIALOG_H
#define TIMEDIALOG_H

#include <QDialog>
#include <QKeySequence>

namespace Ui {
class TimeDialog;
}

class TimeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TimeDialog(QWidget *parent, bool isDirectExit, QKeySequence showHideHotkey);

    int getChangeMinutes();
    bool getIsDirectExit();
    QKeySequence getShowHideHotkey();

private slots:
    void on_btn_ok_clicked();
    void on_btn_cancel_clicked();

private:
    Ui::TimeDialog *ui;
};

#endif // TIMEDIALOG_H
