InstallDir $DESKTOP

OutFile "hello_installer.exe"

Name Hello

Function CheckExistence
#Check if hello.exe already exists.
IfFileExists $INSTDIR\Hello\hello.exe 0 +2
MessageBox MB_OKCANCEL "Hello has already been installed. Continuing the installation will overwrite it." IDOK ok_label IDCANCEL cancel_label
ok_label:
Goto next
# Continue with installation.
cancel_label:
Abort
next:
FunctionEnd

#Default Installer section
Section

Call CheckExistence

#Create a folder called Hello
CreateDirectory $INSTDIR\Hello

# define the output path for this file
SetOutPath $INSTDIR\Hello

# define what to install and put in the output path
File hello.txt
File hello.exe

# define uninstaller
WriteUninstaller "$INSTDIR\Hello\hello_uninstaller.exe"

# end our default section
SectionEnd

#Uninstaller Section
Section "Uninstall"

Delete "$INSTDIR\hello_uninstaller.exe"

Delete "$INSTDIR\hello.txt"
Delete "$INSTDIR\hello.exe"

RMDir $INSTDIR

SectionEnd