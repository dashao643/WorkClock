# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build

This is a Qt 6 C++17 desktop application for Windows (MinGW 64-bit). CMake is the primary build system; the `.pro` file is outdated (references stale file paths).

```bash
# Configure (edit CMakePresets.json for Qt6 path first)
cmake --preset release
# Build
cmake --build build/release -j 16
```

Binary: `build/release/WorkClock.exe` (release) or `build/Desktop_Qt_6_11_1_MinGW_64_bit-Debug/debug/WorkClock2.exe` (qmake debug).

Version string (`1.3.2`) is defined in `CMakeLists.txt` and available at compile time via `APP_VERSION` macro. The `.qtcreator/` and `build/` directories are gitignored.

The app uses single-instance enforcement via `QSharedMemory` in `main.cpp`. QSS styling is loaded from `:/style.qss` (bundled in `assets/assets.qrc`) and applied globally before the main window is created.

## Architecture

A work-time tracker with a 4-tab `QTabWidget`, SQLite persistence, system tray, and global hotkey support.

### Main window — `widget.h/cpp` + `widget.ui` (390×480)

- **Tab 0 — Clock** (`uiTimeShowInit`): Date label, running clock display (`label_clock`), start/pause + save + reset + adjust + target settings buttons, today's elapsed hours label.
- **Tab 1 — Records** (`uiRecordInit`): `QTableView` backed by `RecordModel` with a grouped-by-date SQL query. Target columns use `IconDelegate` for visual status.
- **Tab 2 — Chart** (`uiChartInit`): Day/week/month bar charts via `RecordChart`, displayed in scrollable `QChartView` widgets.
- **Tab 3 — Tools** (`uiToolInit`): Text editor for `temp.txt` with case conversion (upper→CONSTANT_CASE, lower→lowercase) and backslash→forward slash path conversion.

### System tray & hotkey — `trayInit()` + `hotkeyInit()`

A `QSystemTrayIcon` with a right-click menu (show window / exit). Global hotkey is registered via `WindowsManager` (Win32 `RegisterHotKey`). `hotkeyInit()` is currently empty — the wiring to call `windowsManager_->registerGlobalHotkey()` is not yet implemented.

`closeEvent` behavior depends on `config_.isDirectExit`: if true, shows unsaved-data confirmation; if false, hides to tray.

### Database — SQLite via `QSqlDatabase` + `QSqlQuery`

- File: `workClock.db` (working directory)
- Schema (from `Widget::sqliteInit`):

```sql
create table if not exists record (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    date TEXT NOT NULL,
    seconds INTEGER DEFAULT 0,
    target0 INTEGER,
    target1 INTEGER,
    target2 INTEGER
)
```

- `query_` is a raw `QSqlQuery*` created once in `sqliteInit()` and reused for all DB operations. It is **never deleted**.
- `fillMissingDays()` inserts placeholder rows for any days between `config_.lastSaveDate` and today, inside a transaction.
- `saveTimerRecord()` inserts a new row with the given seconds delta (can be negative for adjustments).

### Config — `appconfig.h/cpp` + `dataStructure.h`

Wraps `QSettings` in INI format (`setting.ini`). The `Config_t` struct:

```cpp
struct Config_t {
    bool hasImported;               // legacy text import done?
    QDate lastSaveDate;             // last save date
    QStringList targetNameList;     // custom target names (0–3 items)
    QTime lastSaveTime;             // last save/pause timestamp
    bool isDirectExit;              // close button exits directly (vs. hide to tray)
    QKeySequence showHideHotkey;    // global show/hide hotkey
};
```

Config is read once in the `Widget` constructor and saved in `closeEvent` or on explicit quit via tray menu (`aboutToExit`). `aboutToExit` also saves `temp.txt`.

### RecordModel — `recordmodel.h/cpp`

Subclass of `QSqlQueryModel`. Overrides `data()` and `headerData()`:
- Column 0 (date): shown as-is
- Column 1 (seconds): formatted via static `formatSeconds()` as `X时X分`
- Column 2+ (target columns): 1 → green icon, 0 → no icon, NULL → empty

`IconDelegate` (in the same file) renders target status icons in table cells via `QPainter`.

### RecordChart — `recordchart.h/cpp`

Generates 3 `QChart` objects (day/week/month) from the SQLite data. Each chart uses horizontal `QBarSeries` with `QBarCategoryAxis` for Y-axis labels. The `chartLoad()` method takes a `QSqlQuery*` and a `QList<QChart*>&`.

### TimeDialog — `dialog/timedialog.h/cpp` + `timedialog.ui`

Modal dialog that now serves double duty: time adjustment (add/subtract minutes via `QSpinBox`) **and** settings (direct exit toggle, show/hide hotkey). Constructor takes `(QWidget *parent, bool isDirectExit, QKeySequence showHideHotkey)`. Key methods: `getChangeMinutes()`, `getIsDirectExit()`, `getShowHideHotkey()`.

### TargetDialog — `dialog/targetdialog.h/cpp` + `targetdialog.ui`

Modal dialog for managing up to 3 daily goals:
- Add/rename targets (max 20 chars)
- Clock-in (mark complete) for today with visual feedback
- `targetFlagV_` is hardcoded to size 3 — if `setting.ini` has >3 targets (manual edit), `listWidgetInit` will crash with out-of-bounds access.

### TMessageBox — `dialog/tmessagebox.h/cpp`

Custom `QMessageBox` subclass with 3 buttons: OK (save), Cancel, Discard. Used for the unsaved-data exit confirmation flow.

### WindowsManager — `windows_manager.h/cpp`

Win32-specific global hotkey registration. Maps Qt key codes to Windows virtual keys and modifier flags. Emits `sgn_hotKeyTrigger()` when the registered hotkey is pressed. Singleton-like usage in `Widget` (not QObject-parented).

### Resources — `assets/assets.qrc`

Icons under `assets/picture/`: `play2.ico`, `play2_h.ico`, `pause.png`, `pause_h.png`, `save.ico`, `save_h.ico`, `settings.ico`, `settings_h.ico`, `target.png`, `target_n.png`, `check.png`, `edit.png`, `edit_h.png`, `edit_p.png`, `reset.png`, `reset_h.png`, `reset_p.png`, `arrow_up.png`, `arrow_up_h.png`, `arrow_down.png`, `arrow_down_h.png`, `clockIn2.png`, `clockIn2_h.png`, `clockIn2_p.png`, `clock1.ico`, `clock1_white.ico`. Also bundles `style.qss` and `app_ico.rc`.

### QSS Styling

The start/pause button uses Qt dynamic properties — `[action="start"]` / `[action="stop"]` — to switch between play/pause icons and colors. These are toggled in `updateTimerState()` via `setProperty()` + `unpolish()`/`polish()`.

### `dataStructure.h` — shared types

```cpp
enum TabPage_e { ClockPage = 0, RecordPage, ChartPage, ToolPage };
```
