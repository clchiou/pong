from scons_package import *


register_root_env(Environment(
    CPPPATH = Split('#'),
    CPPFLAGS = Split('-O2 -Wall -Werror -std=c++11'),
))


sconscripts = ['base/SConscript',
               'graphics/SConscript',
               'platform/linux/SConscript',
               'platform/linux/graphics/SConscript',
               'third_party/box2d/SConscript']

for sconscript in sconscripts:
    SConscript(sconscript)


SConscript('SConscript', variant_dir='#out', duplicate=0)
