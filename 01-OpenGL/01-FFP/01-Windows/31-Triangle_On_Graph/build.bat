cls


cl.exe /c /EHsc /I C:\freeglut\include OGL.cpp 

rc.exe OGL.rc  

link.exe OGL.obj OGl.res user32.lib gdi32.lib /SUBSYSTEM:WINDOWS
