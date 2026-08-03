#ifndef TIMEDIALOG_H
#define TIMEDIALOG_H

#include <QDialog>

namespace Ui {
class TimeDialog;
}

class TimeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TimeDialog(QWidget *parent = nullptr);

    int getChangeMinutes();

private slots:
    void on_btn_ok_clicked();
    void on_btn_cancel_clicked();

private:
    Ui::TimeDialog *ui;
};

#endif // TIMEDIALOG_H
