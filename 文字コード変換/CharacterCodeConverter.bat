@echo off
rem Shift_JIS -> BOM UTF-8
setlocal EnableDelayedExpansion

CHCP 65001

call FolderSelecter.exe

set selectDirectory=error

for /f %%a in (temp.txt) do (
 set selectDirectory=%%a
)
echo %selectDirectory%


for /r %selectDirectory% %%A in (*.*) do (
     echo %%A| findstr /l /e /i ".cpp .h"
     if !ERRORLEVEL! equ 0 (
    powershell -nop -c "&{[IO.File]::WriteAllText($args[1], [IO.File]::ReadAllText($args[0], [Text.Encoding]::GetEncoding(932)), [TEXT.Encoding]::UTF8)}" \"%%A\" \"%%A\"
   )
)

pause