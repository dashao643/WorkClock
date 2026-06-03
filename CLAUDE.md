# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build

This is a Qt 6.11.1 C++17 desktop application using qmake with MinGW 64-bit (Windows).

```bash
# Configure (from project root)
qmake WorkClock2.pro

# Build
make                    # or: mingw32-make

# The Debug binary goes to:
# build/Desktop_Qt_6_11_1_MinGW_64_bit-Debug/debug/WorkClock2.exe
```

Open `WorkClock2.pro` in Qt Creator for IDE-based development. The `.qtcreator/` and `build/` directories are gitignored.

## Architecture

A work-time tracker with a 4-tab UI (`QTabWidget`), SQLite persistence, and legacy text-file import.

**Entry & setup** — `main.cpp` creates the `QApplication` and `Widget` (the main window). Single-instance check and QSS stylesheet loading are currently commented out.

**Main window** — `widget.h/cpp` + `widget.ui` (400×480 fixed-size window):
- **Tab 0 — Clock** (`uiPage1Init`): date label, running clock display (`label_clock`), start/pause + save buttons, today's elapsed hours label, and action buttons (adjust/reset/settings). Most logic is not yet wired up.
- **Tab 1 — Records** (`uiPage2Init`): `QTableView` for browsing past daily records from SQLite — currently empty init.
- **Tab 2 — Chart** (`uiPage3Init`): labels for 7-day average, 30-day average, and last save time — currently empty init.
- **Tab 3 — Tools** (`uiPage4Init`): lowercase→uppercase converter, whitespace remover, and a `QTextEdit` output area — currently empty init.

**Database** — SQLite via `QSqlDatabase` + `QSqlQuery`:
- File: `workClock.db` (created in the current working directory)
- Schema: `record(id INTEGER PK, date TEXT, seconds INTEGER DEFAULT 0)`
- Initialized in `Widget::sqliteInit()`, also used by `textFileRead()` for legacy import.
- The raw `QSqlQuery* query_` pointer is stored on `Widget` and reused across the class.

**Legacy import** — `Widget::textFileRead()` reads `EverydayRecord.txt` (format: `date=seconds=duration` per line) and inserts into SQLite inside a transaction. Gated by `AppConfig::getTextFile()`, which currently always returns `true` (import disabled).

**Config** — `appconfig.h/cpp` wraps `QSettings` in INI format (`setting.ini`). Currently only stores `textFile/imported` to track whether the legacy import ran.

**Time adjustment dialog** — `timedialog.h/cpp` + `timedialog.ui`: a modal `QDialog` with a `QSpinBox` (1–60 minutes) and add/subtract radio buttons. Returns values via `getChangeMinutes()` / `isAddMode()`.

**Shared types** — `dataStructure.h` defines `TabPage_e` enum (`ClockPage`, `RecordPage`, `ChartPage`, `ToolPage`).

**Resources** — `res.qrc` bundles `PicRes/clock1.ico`, `PicRes/settings.ico`, `PicRes/settings2.ico`, and `style.qss`. The app icon is set to `clock1.ico` via `RC_ICONS`.

## Version

The version string (`1.3.0`) is defined in the `.pro` file and available at compile time via the `APP_VERSION` macro.
