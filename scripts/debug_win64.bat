@echo off

echo Checking
if not exist src (echo Didn't find src dictory && pause && exit)
if not exist build (echo Diring build && mkdir build)
if not exist bin\win64 (echo Diring bin\win64 && mkdir bin\win64)

echo Mocing
moc src/Application.h -o build/moc_Application.cpp

echo Objecting
g++ -c src/main.cpp -o build/main.o -I %QT_HOME%/include
g++ -c src/Application.cpp -o build/Application.o -I %QT_HOME%/include
g++ -c src/enycpt.cpp -o build/enycpt.o
g++ -c src/console.cpp -o build/console.o

echo Resources Complating
windres -i src/Resources/resources.rc -o build/rc.o -F pe-x86-64

echo Linking Core
g++ build/enycpt.o -shared -fPIC -o bin/win64/enycpt.dll、

echo Linking
g++ build/Application.o build/main.o build/moc_*.cpp bin/win64/enycpt.dll %QT_HOME%/lib/*.a -I %QT_HOME%/include -o bin/win64/enycpt.UI.exe
g++ build/console.o bin/win64/enycpt.dll -o bin/win64/enycpt.exe

echo Complate Suffuily!