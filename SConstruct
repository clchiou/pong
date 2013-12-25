from scons_package import *


package_environment(Environment(
    CPPPATH = Split('#'),
    CPPFLAGS = Split('-O2 -Wall -Werror -std=c++11'),
))


sconscripts = ['base/SConscript',
               'graphics/SConscript',
               'platform/android/SConscript',
               'platform/android/pong/SConscript',
               'platform/linux/SConscript',
               'platform/linux/graphics/SConscript',
               'third_party/box2d/SConscript']
for sconscript in sconscripts:
    SConscript(sconscript)


make_builders(
    sconscript = 'SConscript',
    build_root = '#out',
    variants = ['host', 'android'],
    duplicate = 0
)
