VariantDir('out', '#', duplicate=0)

Program(target='pong',
        source=Split('pong.cc gl.cc glfw.cc'),
        CPPFLAGS='-O2 -Wall -Werror -std=c++11',
        LIBS=Split('-lglfw -lGL -lGLEW'))
