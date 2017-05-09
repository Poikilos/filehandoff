# filehandoff
Hand off paths from url files, corrected paths from shortcuts, or files themselves--to correct GNU or Wine programs

## Usage:
* Associate URL (and url) files and (wine) Adobe PhotoShop Elements 5.0 projects with filehandoff
* If wrong browser is opened or no browser opens, edit 
    $HOME/.config/filehandoff/browsers.txt
    to include your most preferred browser first (such as /usr/bin/iceweasel)
(NOTE: there is a bug in 2017-04 release of firefox-beta in antergos which makes firefox always say File not found and try to open a local file instead of the path given as the first parameter)
* NOTE: Program (C++) must be changed in order to work with other wine programs
