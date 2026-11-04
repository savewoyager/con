
@echo off
chcp 65001 > nul

cd /d "%~dp0"

echo =================================
echo     MULTILANGUAGE EMULATOR
echo     by TIMUR M. IKBO-64-25
echo =================================
echo.

REM Путь к собранному C++ приложению
set "EXE=x64\Debug\emulator.exe"

REM Проверяем наличие программы
if not exist "%EXE%" (
    echo ERROR: emulator.exe not found!
    echo Check the EXE path in run.bat
    pause
    exit /b 1
)
REM Проверяем Python
echo Сборка Python кода...
if errorlevel 1 (
    echo ERROR: Python is not available!
    pause
    exit /b 1
)
echo Сборка Rust кода...
rustc -Awarnings rust/rev.rs -o runtime/rev.exe

if errorlevel 1 (
    echo ERROR: Rust compilation failed!
    pause
    exit /b 1
)

echo Сборка Go кода...
go build -o runtime/who.exe go/who.go

if errorlevel 1 (
    echo ERROR: Go compilation failed!
    pause
    exit /b 1
)

echo Сборка Java кода...
javac -Awarnings -source 8 -target 8 -Xlint:-options -d runtime java/CpModule.java

if errorlevel 1 (
    echo ERROR: Java compilation failed!
    pause
    exit /b 1
)
echo Сборка JS кода...
node --version > nul 2>&1
if errorlevel 1 (
    echo ERROR: Node.js is not available!
    pause
    exit /b 1
)
echo Сборка C# кода...
dotnet build csharp/KillModule.csproj -o runtime -p:WarningLevel=0 --verbosity quiet
if errorlevel 1 (
    echo ERROR: C# compilation failed!
    pause
    exit /b 1
)
echo Сборка Kotlin кода...
call kotlinc kotlin/NotFound.kt -include-runtime -d runtime/NotFound.jar
if errorlevel 1 (
    echo ERROR: Kotlin compilation failed!
    pause
    exit /b 1
)
REM Запускаем эмулятор
"%EXE%" "%~dp0vfs\base.xml" "%~dp0scripts\rev.txt"

pause
