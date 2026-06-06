# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build

This is a Qt 6.11.1 C++17 desktop application using qmake with MinGW 64-bit (Windows).

```bash
qmake WorkClock2.pro
make                    # or: mingw32-make
```

Binary: `build/Desktop_Qt_6_11_1_MinGW_64_bit-Debug/debug/WorkClock2.exe`

Open `WorkClock2.pro` in Qt Creator for IDE-based development. `.qtcreator/` and `build/` are gitignored.

The version string (`1.3.0`) is defined in the `.pro` file and available at compile time via `APP_VERSION` macro.

## Architecture

A work-time tracker with a 4-tab `QTabWidget`, SQLite persistence, and a legacy text-file import.

### Main window — `widget.h/cpp` + `widget.ui` (400×480 fixed)

- **Tab 0 — Clock** (`uiTimeShowInit`): Date label, running clock display (`label_clock`), start/pause + save + reset + adjust + settings buttons, today's elapsed hours label. **All wired up.**
- **Tab 1 — Records** (`uiRecordInit`): `QTableView` backed by `RecordModel` for browsing daily records. **Init code is commented out** — the SQL query and model wiring exist but are disabled.
- **Tab 2 — Chart** (`uiChartInit`): Empty.
- **Tab 3 — Tools** (`uiPage4Init`): Empty.

### Database — SQLite via `QSqlDatabase` + `QSqlQuery`

- File: `workClock.db` (working directory)
- Actual schema (from `Widget::sqliteInit`):

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

- `query_` is a raw `QSqlQuery*` created once in `sqliteInit()` and reused for all DB operations across the class. It is **never deleted** (memory leak — acceptable for now).
- **Known crash risk**: If `db_.open()` fails, `sqliteInit()` shows a warning and returns, but the constructor continues calling `textFileRead()` and `uiTimeShowInit()`, both of which call `query_->prepare()` on a closed database → crash.

### Config — `appconfig.h/cpp` + `dataStructure.h`

Wraps `QSettings` in INI format (`setting.ini`). The `Config_t` struct holds:

```cpp
struct Config_t {
    bool hasImported;               // legacy text import done?
    QDate lastSaveDate;             // last save date
    QStringList targetNameList;     // custom target names (0–3 items)
};
```

Persisted keys: `textFile/imported`, `saveRecord/lastDate`, `target/count`, `target/name0`–`target/name2`.

**Bug**: `hasImported` is set to `true` during import but only saved to disk in `closeEvent()`. If the app crashes between import and close, the import re-runs next launch.

### Legacy import — `Widget::textFileRead()`

Reads `EverydayRecord.txt` (format: `date=seconds=duration` per line), inserts into SQLite inside a transaction. Gated by `config_.hasImported` (not `AppConfig::getTextFile()` — that method no longer exists).

### RecordModel — `recordmodel.h/cpp`

Subclass of `QSqlQueryModel`. Overrides `data()` and `headerData()`:
- Column 0 (date): shown as-is
- Column 1 (seconds): formatted as `X时X分`
- Column 2+ (target columns): 1 → green background + "完成", 0 → orange background + "未完成", NULL → empty

### TimeDialog — `timedialog.h/cpp` + `timedialog.ui`

Modal dialog with a `QSpinBox` (1–60 min) and add/subtract radio buttons.

**Known bug**: `isAddMode()` is documented in the old CLAUDE.md but **does not exist** in the current header — only `getChangeMinutes()` is exposed, and it always returns a positive value. The subtract radio button is ignored. When wiring this, add `bool isAddMode()` and use it in `Widget::on_btn_change_clicked()`.

### TargetDialog — `targetdialog.h/cpp` + `targetdialog.ui`

Modal dialog for managing up to 3 daily goals:
- Add/rename targets (max 20 chars)
- Clock-in (mark complete) for today with visual feedback (green background + icon)
- `targetFlagV_` is hardcoded to size 3 in the constructor — if `setting.ini` has >3 targets (manual edit), `listWidgetInit` will crash with out-of-bounds access.

### QSS Styling

`style.qss` is loaded and applied globally in `main.cpp`. Notable pattern: the start/pause button uses Qt dynamic properties — `[action="true"]` / `[action="false"]` — to switch between play/pause icons. These properties are set/removed at runtime (not currently visible in the code — check `widget.ui` or runtime setup).

### Resources — `res.qrc`

Icons under `PicRes/`: `play2.ico`, `play2_h.ico`, `pause.png`, `pause_h.png`, `save.ico`, `save_h.ico`, `settings.ico`, `settings_h.ico`, `target.png`. App icon set via `RC_ICONS = PicRes/clock1.ico` in `.pro`.

## `dataStructure.h` — shared types

```cpp
enum TabPage_e { ClockPage = 0, RecordPage, ChartPage, ToolPage };
```
