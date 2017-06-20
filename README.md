# filehandoff
Hand off paths from url files, corrected paths from shortcuts, or files themselves--to correct GNU or Wine programs

## Usage:
* Associate URL (and url) files and (wine) Adobe PhotoShop Elements 5.0 projects with filehandoff
* If wrong browser is opened or no browser opens, edit 
    $HOME/.config/filehandoff/browsers.txt
    to include your most preferred browser first (such as /usr/bin/iceweasel)
(NOTE: there is a bug in 2017-04 release of firefox-beta in antergos which makes firefox always say File not found and try to open a local file instead of the path given as the first parameter)
* NOTE: Program (C++) must be changed in order to work with other wine programs

## Planned Features
* ambiguous mimetypes parsing feature:
allow choosing different programs by extension on linux (such as, so that various project files that are all same content type don't all use same IDE)
	* if runs without param, configure "Default Programs"
	* also allow "Multiple Default Programs" for exe (mono or wine) or somehow detect binary vs .NET (OR mono) assembly
	* see "mimetype info.md"
	* then make a release
	* THEN ADD install instructions (note to self: also add setup steps to Antergos*.txt in "IT/Linux Clients on Windows" and remember to add associating file extensions manually of types mentioned in "mimetype info.md"
