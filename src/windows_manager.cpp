#include "windows_manager.h"

#include <QKeySequence>

WindowsManager::WindowsManager()
{

}

WindowsManager::~WindowsManager() = default;

// 注册全局快捷键
bool WindowsManager::registerGlobalHotkey(QKeySequence key, WId wId)
{
    if (key.isEmpty()) return false;

    HWND hwnd = reinterpret_cast<HWND>(wId);

    QKeyCombination combo = key[0];
    WORD vk = qtKeyToVk(combo.key());
    WORD mods = qtModsToWin(combo.keyboardModifiers());

    return (RegisterHotKey(hwnd, hotkeyId_, mods, vk));
}

void WindowsManager::unregisterGlobalHotkey(WId wId)
{
    HWND hwnd = reinterpret_cast<HWND>(wId);

    UnregisterHotKey(hwnd, hotkeyId_);
}

bool WindowsManager::eventProcess(const QByteArray &eventType, void *message)
{
    if (eventType == "windows_generic_MSG") {
        MSG *msg = static_cast<MSG *>(message);
        if (msg->message == WM_HOTKEY && msg->wParam == hotkeyId_) {
            emit sgn_hotKeyTrigger();
            return true;
        }
    }

    return false;
}

/*-----------------------------------------------------------------*/

WORD WindowsManager::qtKeyToVk(Qt::Key keyNum)
{
    // 字母 A-Z：编码相同
    if (keyNum >= Qt::Key_A && keyNum <= Qt::Key_Z)
        return (WORD)keyNum;

    // 数字 0-9：相同
    if (keyNum >= Qt::Key_0 && keyNum <= Qt::Key_9)
        return (WORD)keyNum;

    // 功能键 F1-F24：Qt::Key_F1 = 0x01000030，Windows VK_F1 = 0x70，计算偏移
    if (keyNum >= Qt::Key_F1 && keyNum <= Qt::Key_F24)
        return VK_F1 + (keyNum - Qt::Key_F1);

    // 其他常用按键逐个映射
    switch (keyNum) {
    case Qt::Key_Space:     return VK_SPACE;
    case Qt::Key_Tab:       return VK_TAB;
    case Qt::Key_Backspace: return VK_BACK;
    case Qt::Key_Return:
    case Qt::Key_Enter:     return VK_RETURN;
    case Qt::Key_Escape:    return VK_ESCAPE;
    case Qt::Key_Delete:    return VK_DELETE;
    case Qt::Key_Insert:    return VK_INSERT;
    case Qt::Key_Home:      return VK_HOME;
    case Qt::Key_End:       return VK_END;
    case Qt::Key_PageUp:    return VK_PRIOR;
    case Qt::Key_PageDown:  return VK_NEXT;
    case Qt::Key_Left:      return VK_LEFT;
    case Qt::Key_Right:     return VK_RIGHT;
    case Qt::Key_Up:        return VK_UP;
    case Qt::Key_Down:      return VK_DOWN;
    default:                return 0;
    }
}

/**
 * 将 Qt 的修饰键 转换为 Windows 的修饰键标志
 *
 * Qt 修饰键：ControlModifier / AltModifier / ShiftModifier / MetaModifier
 * Windows 修饰键：MOD_CONTROL(0x0002) / MOD_ALT(0x0001) / MOD_SHIFT(0x0004) / MOD_WIN(0x0008)
 */
WORD WindowsManager::qtModsToWin(Qt::KeyboardModifiers mods)
{
    WORD w = 0;

    if (mods & Qt::ControlModifier) w |= MOD_CONTROL;
    if (mods & Qt::AltModifier)     w |= MOD_ALT;
    if (mods & Qt::ShiftModifier)   w |= MOD_SHIFT;
    if (mods & Qt::MetaModifier)    w |= MOD_WIN;

    return w;
}