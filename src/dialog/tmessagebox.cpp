#include "tmessagebox.h"

#include <QPushButton>

TMessageBox::TMessageBox(const QString& text, bool discardBtn, QWidget *parent)
    : QMessageBox(parent)
{
    setWindowTitle("提示");
    setText(text);
    setStandardButtons(QMessageBox::NoButton);
    setIcon(QMessageBox::Question);

    btnOk_ = new QPushButton("确定", this);
    btnCancel_ = new QPushButton("取消", this);

    // 绿色调
    btnOk_->setStyleSheet(R"(
        QPushButton {
            font-size: 14px;
            border: 1px solid #4CAF50;
            border-radius: 4px;
            background-color: #A5D6A7;
            color: #1B5E20;
        }
        QPushButton:hover {
            background: #C8E6C9;
        }
        QPushButton:pressed {
            background: #81C784;
        }
    )");

    // 白色调
    btnCancel_->setStyleSheet(R"(
        QPushButton {
            font-size: 14px;
            border: 1px solid #DCDFE6;
            border-radius: 4px;
            background-color: #FFFFFF;
            color: #333333;
        }
        QPushButton:hover {
            background: #F5F7FA;
        }
        QPushButton:pressed {
            background: #E5E6EB;
        }
    )");

    addButton(btnOk_, QMessageBox::AcceptRole);
    addButton(btnCancel_, QMessageBox::RejectRole);
    setDefaultButton(btnOk_);
    setEscapeButton(btnCancel_);

    if (discardBtn) {
        discardSave_ = new QPushButton("丢弃", this);
        // 红色调
        discardSave_->setStyleSheet(R"(
            QPushButton {
                font-size: 14px;
                border: 1px solid #E05C5C;
                border-radius: 4px;
                background-color: #FFB3B3;
                color: #B71C1C;
            }
            QPushButton:hover {
                background: #FFD6D9;
            }
            QPushButton:pressed {
                background: #F2AFAF;
            }
        )");
        addButton(discardSave_, QMessageBox::DestructiveRole);
    }
}

QPushButton *TMessageBox::okButton() const
{
    return btnOk_;
}

QPushButton *TMessageBox::cancelButton() const
{
    return btnCancel_;
}

QPushButton *TMessageBox::discardButton() const
{
    return discardSave_;
}
