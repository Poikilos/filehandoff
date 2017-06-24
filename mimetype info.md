# Mimetypes
## Ambiguous Mimetypes
A list of Mimetypes that cannot possibly launch the correct program always, making mimetype-based file association irreperably broken
"unknown": kbdx, gcode, bat, ps1 (powershell script), website (MS Edge (R) internet shortcut),
"CSV document": csv, (opens with Krita by default), fpp files (FlashPrint project), all files where mimetype is not determinable (such as a 0-byte file with no extension)

## content examples
(content that filehandoff should parse; remember to trim lines to remove \r if file loaded had \r\n line endings and loaded by Qt using \n as newline)
	* website:
	[{000214A0-0000-0000-C000-000000000046}]
	Prop4=31,What is the System Update Readiness Tool? - Windows Help
	Prop3=19,2
	[{A7AF692E-098D-4C08-A225-D433CA835ED0}]
	Prop5=3,0
	Prop9=19,0
	Prop2=65,2C0000000000000001000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7E00000049000000BA040000FB030000A0
	[InternetShortcut]
	URL=http://windows.microsoft.com/en-US/windows7/What-is-the-System-Update-Readiness-Tool
	IDList=
	IconFile=http://windows.microsoft.com/favicon.ico
	IconIndex=1
	[{9F4C2855-9F79-4B39-A8D0-E1D42DE1D5F3}]
	Prop5=8,Microsoft.Website.B4BD2547.C2AAC8A7

## non-ambiguous mimetype examples:
	"Keepass database": kdb
	"Internet shortcut": url
	"Windows link": lnk
	"Zip archive": zip
	"Window Registry extract": reg
	"plain text document": txt
	"MS VBScript": vbs
	"Python script": py
	"Microsoft Word Document": docx
	"Microsoft Excel Worksheet": xlsx


	
