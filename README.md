# filehandoff
Hand off paths from url files, corrected paths from shortcuts, or files themselves--to correct GNU or Wine programs

## Changes
(2018-10-05)
* added ambiguous mimetypes handling feature (same mimetype different extension): automatically picks program
(2017-12-20)
* added support for the following extensions with "unknown" mimetype: fpp (FlashPrint), mtl (Wavefront material)

## Usage
- Associate URL (and url) files and (wine) Adobe PhotoShop Elements 5.0
  projects with filehandoff
- If wrong browser is opened or no browser opens, edit
    $HOME/.config/filehandoff/browsers.txt
    to include your most preferred browser first (such as /usr/bin/iceweasel)
(NOTE: there is a bug in 2017-04 release of firefox-beta such as in
Antergos which makes firefox always say File not found and try to open a
local file instead of the path given as the first parameter)
- NOTE: Program (C++) must be changed in order to work with other wine
  programs
- If you have KDE Plasma try not to use this for linux filetypes too
  much--as you can configure extension handling via GUI in Plasma--see
  [File
  Associations](https://www.ghacks.net/2018/03/28/how-to-set-default-program-file-associations-in-kde5-plasma/)
  section of System Settings.

## Compiling
- Install qt-creator

### Compiling Issues
#### Bug where Qt Creator opens project but says no valid kits found
- First try to install qmake and build tools for your distro.
  - Debian/Ubuntu/Mint (along with `qt-creator`):
    `sudo apt install build-essential qt5-qmake qt5-default qtdeclarative5-dev`--See also old packages for previous OS releases:
    - changed `qt5-qtdeclarative-devel` to `qtdeclarative5-dev`
  - RHEL:
    `qt5-qtdeclarative-devel` (and others)
- Diagose by going to Tools, Options
  * If "Qt Versions" tab in Tools says, "Qt version is not properly
    installed, please run make install" when you click qmake, the steps
    below will likely not work until you first install the default
    profile such as via `sudo apt install -y qt5-default`
    (The following may be a problem, but I'm not sure:
    `readlink /usr/bin/qmake` says "qtchooser")
  * If Desktop is listed but has warnings: "No debugger set up." and
    "CMake configuration has a path to qmake binary set, even though the
    kit has no valid Qt version"
    - Qt Version says "Qt  in PATH (System) (invalid)"
      - clicking "Manage" (for Qt version) says: "Could not determine
        the path to the binaries of the Qt installation, maybe the qmake
        path is wrong?" even though `/usr/bin/qmake-qt5.sh --version`
        works (see
        <https://forum.qt.io/topic/25217/could-not-determine-the-path-to-the-binaries>)
    - Clicking qmake in "Qt Version" says "no qmlscene installed":
      install the qt5-qtdeclarative-devel or appropriate package for
      your distro as per
      <https://stackoverflow.com/questions/34918115/getting-no-qmlscene-installed-warning-in-qt-creator-on-ubuntu/36667532>
      and/or qmlscene as per
      <https://stackoverflow.com/questions/50736194/no-qmlscene-installed-why-is-a-warning-sign-next-to-qt-version-on-buildrun-opt/50738734>
    - manually open the project from Qt Creator then manually add Build
      targets under "Projects" for Release and Debug, named Release and
      Debug respectively.
    - If On Build&Run, Qt Creator says "Issues (1)" "The Qt version is
        invalid. Could not determine the path to the binaries of the Qt
        installation, maybe the qmake path is wrong?"
      - So click Tools, Options, Build&Run, then it says same error as
        above.
      - For "qmake location," click "Browse" button then change it from
        qmake-qt5.sh to qmake-qt5 and issue is resolved.
    - You may still have to choose something for the following in
      "Kits":
      - "Qt Version"
      - "C++" (in Compilers section)
    - If it works properly, you should have a checkbox for "Desktop" in
      the project configuration screen (instead of just import
      configuration, which will not work unless you have one already)--
      then you can click the "Configure Project" button.
    - If you get "assert was not declared in this scope," `#include
      <assert.h>`

## Known Issues
- handle .desktop files that are not URL (`URL[$e]=`) .desktop files
- make binary release
- add more ambiguous mimetypes: see "mimetype info.md"

## Planned Features
- allow choosing different programs by extension on linux
  (or at least make configuration file)
  - If it runs without a param, show a window where you can configure
    "Default Programs"--allow adding extensions manually for ambiguous
    mimetypes
  - Also allow "Multiple Default Programs" for exe (mono or wine) or
    somehow detect binary vs .NET (OR mono) assembly
  - see "mimetype info.md"
  - THEN ADD install instructions (note to self: also add setup steps to
    Antergos*.txt in "IT/Linux Clients on Windows"
