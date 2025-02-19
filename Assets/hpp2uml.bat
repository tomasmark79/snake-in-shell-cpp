@echo off
REM MIT License
REM
REM Copyright (c) 2024 Tomas Mark
REM
REM Permission is hereby granted, free of charge, to any person obtaining a copy
REM of this software and associated documentation files (the "Software"), to deal
REM in the Software without restriction, including without limitation the rights
REM to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
REM copies of the Software, and to permit persons to whom the Software is
REM furnished to do so, subject to the following conditions:

REM The above copyright notice and this permission notice shall be included in all
REM copies or substantial portions of the Software.

REM THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
REM IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
REM FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
REM AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
REM LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
REM OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
REM SOFTWARE.

REM Other resources
REM script is using python script: https://github.com/thibaultmarin/hpp2plantuml
REM 
chcp 1250 > nul

echo "hpp2uml - Copyright (c) 2024 Tomas Mark"
echo "using: hpp2uml [file.hpp] | [/r]"

REM Z�sk�n� aktu�ln�ho pracovn�ho adres��e
set "target_directory=%CD%"

REM Nastaven� n�zvu v�sledn�ho souboru
set "output_file=%target_directory%\uml.uml"

if "%~1"=="" (
    echo ��dn� parametr nebyl zad�n.
	chcp 850 > nul
	exit /b
)

REM Nastaven� rekurzivn�ho re�imu
if "%~1"=="/r" (
    set "recursive_mode=1"
    shift
) else (
    set "recursive_mode=0"
)

REM Pokud byl zad�n rekurzivn� re�im, spust�me zpracov�n� slo�ky
if "%recursive_mode%"=="1" (
	REM Vytvo�en� pr�zdn�ho v�stupn�ho souboru
	echo. > "%output_file%"

    call :ProcessFolder "%target_directory%"
)

REM Pokud byl zad�n konkr�tn� soubor, provedeme zpracov�n� tohoto souboru
if not "%~1"=="" (
	REM Vytvo�en� pr�zdn�ho v�stupn�ho souboru
	echo. > "%output_file%"
    
	call :ProcessFile "%~1"
)

REM Pokud nebyl zad�n ��dn� parametr, nebo byl zad�n pouze rekurzivn� re�im, skript nevykon� nic

REM Zpr�va o dokon�en� �lohy
echo �loha dokon�ena.
chcp 850 > nul
exit /b

REM Funkce pro rekurzivn� zpracov�n� slo�ky
:ProcessFolder
for /r "%~1" %%F in (*.hpp) do (
    call :ProcessFile "%%F"
)
goto :eof

REM Funkce pro zpracov�n� jednoho souboru
:ProcessFile
REM Z�sk�n� n�zvu zpracov�van�ho souboru (bez cesty a koncovky)
for %%F in ("%~1") do set "file_path=%%~dpnxF"
REM Z�sk�n� n�zvu souboru bez koncovky
for %%F in ("%~1") do set "base_name=%%~nF"
REM Spu�t�n� p�evodu na PlantUML a p�ipojen� k v�sledn�mu souboru
python "c:\tools\utils\devutils\hpp2plantuml-0.8.4\src\hpp2plantuml\hpp2plantuml.py" -i "%file_path%" >> "%output_file%"
REM Vytvo�en� samostatn�ho .uml souboru pro zpracovan� soubor
echo. > %file_path%.uml
python "c:\tools\utils\devutils\hpp2plantuml-0.8.4\src\hpp2plantuml\hpp2plantuml.py" -i "%file_path%" >> "%file_path%.uml"
echo Processing file: %file_path%

goto :eof
