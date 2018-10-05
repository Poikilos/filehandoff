# filehandoff
Hand off paths from url files, corrected paths from shortcuts, or files themselves--to correct GNU or Wine programs

## Changes
(2018-10-05)
* added ambiguous mimetypes handling feature (same mimetype different extension): automatically picks program
(2017-12-20)
* added support for the following extensions with "unknown" mimetype: fpp (FlashPrint), mtl (Wavefront material)

## Usage
* Associate URL (and url) files and (wine) Adobe PhotoShop Elements 5.0 projects with filehandoff
* If wrong browser is opened or no browser opens, edit
    $HOME/.config/filehandoff/browsers.txt
    to include your most preferred browser first (such as /usr/bin/iceweasel)
(NOTE: there is a bug in 2017-04 release of firefox-beta in antergos which makes firefox always say File not found and try to open a local file instead of the path given as the first parameter)
* NOTE: Program (C++) must be changed in order to work with other wine programs
* If you have KDE Plasma try not to use this for linux filetypes too much--as you can configure extension handling via GUI in Plasma--see [File Associations](https://www.ghacks.net/2018/03/28/how-to-set-default-program-file-associations-in-kde5-plasma/) section of System Settings.

## Compiling
* Install qt-creator

### Compiling Issues
#### Bug where Qt Creator opens project but says no valid kits found
* Diagose by going to Tools, Options (if notes below are relevant, you have this bug)
* Desktop is listed but has warnings: "No debugger set up." and "CMake configuration has a path to qmake binary set, even though the kit has no valid Qt version"
* Qt Version says "Qt  in PATH (System) (invalid)"
  * clicking "Manage" (for Qt version) says: "Could not determine the path to the binaries of the Qt installation, maybe the qmake path is wrong?" even though `/usr/bin/qmake-qt5.sh --version` works
  (see <https://forum.qt.io/topic/25217/could-not-determine-the-path-to-the-binaries>)
* manually open the project from Qt Creator then manually add Build targets under "Projects" for Release and Debug, named Release and Debug respectively.
  On Build&Run, Qt Creator says "Issues (1)" "The Qt version is invalid. Could not determine the path to the binaries of the Qt installation, maybe the qmake path is wrong?"
  So click Tools, Options, Build&Run, then it says same error as above.
  For "qmake location," click "Browse" button then change it from qmake-qt5.sh to qmake-qt5 and issue is resolved.


## Known Issues
* handle .desktop files that are not URL (`URL[$e]=`) .desktop files
* make binary release
* add more ambiguous mimetypes: see "mimetype info.md"

## Planned Features
* allow choosing different programs by extension on linux
  (or at least make configuration file)
  * if runs without param, show a window where you can configure "Default Programs"--allow adding extensions manually for ambiguous mimetypes
  * also allow "Multiple Default Programs" for exe (mono or wine) or somehow detect binary vs .NET (OR mono) assembly
  * see "mimetype info.md"
  * THEN ADD install instructions (note to self: also add setup steps to Antergos*.txt in "IT/Linux Clients on Windows"
