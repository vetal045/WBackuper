# Backuper

### About
This solution represents a tool to make and restore backups.

### Sub projects.

Details about projects can be found at the links below.

[Backup classes and functions](modules/BackupManager).

[Command driven app](modules/ConsoleApp).

[Gui app](modules/GuiApp).

### Build

To generate a solution use `cmakegen-*` files in the scripts folder.

Before generation you should add to system (user) variables the `QTDIR` variable with a path to Qt directory (e.g. `C:\Qt\5.8\msvc2015_64`).

The result solution will be at `cmake-build/*` folder.

For now there are 2 scripts:
- vc14-x86
- vc14-x64
