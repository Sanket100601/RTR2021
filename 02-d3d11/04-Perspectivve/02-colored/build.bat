cls

cl.exe /c /EHsc /I C:\glew\include d3d.cpp 

rc.exe d3d.rc  

link.exe d3d.obj d3d.res user32.lib gdi32.lib /SUBSYSTEM:WINDOWS



