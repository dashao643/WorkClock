#ifndef TMESSAGEBOX_H
#define TMESSAGEBOX_H

#include <QMessageBox>
#include <QObject>

class TMessageBox : public QMessageBox
{
    Q_OBJECT
public:
    TMessageBox(const QString& text, bool discardBtn, QWidget *parent = nullptr);

    QPushButton *okButton() const;
    QPushButton *cancelButton() const;
    QPushButton *discardButton() const;

private:
    QPushButton *btnOk_ = nullptr;
    QPushButton *btnCancel_ = nullptr;
    QPushButton *discardSave_ = nullptr;
};

#endif // TMESSAGEBOX_H
