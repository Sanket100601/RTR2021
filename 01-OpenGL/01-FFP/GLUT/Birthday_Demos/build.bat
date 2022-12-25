cl.exe /c /EHsc /I C:\freeglut\include penguine.cpp
link.exe penguine.obj /LIBPATH:C:\freeglut\lib /SUBSYSTEM:CONSOLE
penguine.exe