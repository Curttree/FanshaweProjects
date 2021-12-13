InstallDir $DESKTOP

OutFile "PhysicsFinal_installer.exe"

Name PhysicsFinal

Function CheckExistence
#Check if PhysicsFinal.exe already exists.
IfFileExists $INSTDIR\PhysicsFinal\PhysicsFinal.exe 0 +2
MessageBox MB_OKCANCEL "The Physics Final Exam has already been installed. Continuing the installation will overwrite it." IDOK ok_label IDCANCEL cancel_label
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

#Create a folder called PhysicsFinal
CreateDirectory $INSTDIR\PhysicsFinal

# define the output path for this file
SetOutPath $INSTDIR\PhysicsFinal

# define what to install and put in the output path
File /r assets
File *.json
File PhysicsFinal.exe

# define uninstaller
WriteUninstaller "$INSTDIR\PhysicsFinal\PhysicsFinal_uninstaller.exe"

# end our default section
SectionEnd

#Uninstaller Section
Section "Uninstall"

Delete "$INSTDIR\PhysicsFinal_uninstaller.exe"

RMDir /r "$INSTDIR\assets"
Delete "$INSTDIR\*.json"
Delete "$INSTDIR\PhysicsFinal.exe"

RMDir $INSTDIR

SectionEnd