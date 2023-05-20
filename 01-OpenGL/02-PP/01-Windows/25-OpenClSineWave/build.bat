cls
Del OGL.exe 
Del OGL.obj
Del OGL.res
cl.exe /c /EHsc /I C:\glew\include /I "C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.8\include" OGL.cpp


rc.exe OGL.rc


link.exe OGL.res OGL.obj /LIBPATH:C:\glew\lib\Release\x64  /LIBPATH:"C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.8\lib\x64" user32.lib kernel32.lib gdi32.lib /SUBSYSTEM:WINDOWS


