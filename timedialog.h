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
    ~TimeDialog();

    Ui::TimeDialog *ui;
private:

};

#endif // TIMEDIALOG_H
