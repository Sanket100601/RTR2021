cls
Del D3D.exe 
Del D3D.obj
Del D3D.res
cl.exe /c /EHsc  D3D.cpp


rc.exe D3D.rc


link.exe D3D.res D3D.obj  user32.lib kernel32.lib gdi32.lib /SUBSYSTEM:WINDOWS
D3D.exe


