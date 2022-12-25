cls

rc.exe OGL.rc

nvcc -c -o sinewave.obj sinewave.cu

cl.exe /c /EHsc /I"C:\glew\include" /I"C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.3\include" OGL.cpp 

link.exe /LIBPATH:"C:\glew\lib\Release\x64" /LIBPATH:"C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.3\lib\x64" OGL.obj OGL.res sinewave.obj user32.lib kernel32.lib gdi32.lib

OGL.exe