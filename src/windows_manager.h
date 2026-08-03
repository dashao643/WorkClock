#ifndef __WINDOWS_MANAGER_H__
#define __WINDOWS_MANAGER_H__

#include <windows.h>
#include <QWidget>

class WindowsManager : public QObject
{
    Q_OBJECT

public:
    WindowsManager();
    ~WindowsManager();
    void registerGlobalHotkey(QKeySequence key, WId wId);
    void unregisterGlobalHotkey(WId wId);
    bool eventProcess(const QByteArray &eventType, void *message);

signals:
    void sgn_hotKeyTrigger();

private:
    WORD qtKeyToVk(Qt::Key keyNum);
    WORD qtModsToWin(Qt::KeyboardModifiers mods);

private:
    int hotkeyId_ = 1;
};

#endif
