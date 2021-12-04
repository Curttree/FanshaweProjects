InstallDir $DESKTOP

OutFile "hello_installer.exe"

#Default Installer section
Section

# define the output path for this file
SetOutPath $INSTDIR

# define what to install and put in the output path
File hello.txt

# define uninstaller
WriteUninstaller "$INSTDIR\hello_uninstaller.exe"

# end our default section
SectionEnd

#Uninstaller Section
Section "Uninstall"

Delete "$INSTDIR\hello_uninstaller.exe"

Delete "$INSTDIR\hello.txt"

RMDir $INSTDIR

SectionEnd