mkdir -p OGL.app/Contents/MacOS
clang++ -c Sphere.mm
clang++ -Wno-deprecated-declarations -c OGL.mm
clang++ -o OGL.app/Contents/MacOS/OGL OGL.o Sphere.o -framework Cocoa -framework QuartzCore -framework OpenGL 