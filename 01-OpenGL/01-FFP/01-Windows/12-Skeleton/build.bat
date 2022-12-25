cls


cl.exe /c /EHsc /I C:\freeglut\include skeleton.cpp 

rc.exe OGL.rc  

link.exe Skeleton.obj OGL.res user32.lib gdi32.lib /SUBSYSTEM:WINDOWS

skeleton.exe
