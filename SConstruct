import os


build_root = Dir('#out')

env = Environment(BUILD_ROOT=build_root,
                  CPPPATH='#',
                  CPPFLAGS='-O2 -Wall -Werror -std=c++11')
Export('env')

sconscripts = ['base/SConscript',
               'graphics/SConscript',
               'platform/linux/SConscript',
               'platform/linux/graphics/SConscript',
               'third_party/box2d/SConscript']

for sconscript in sconscripts:
    variant_dir = os.path.join(build_root.path, os.path.dirname(sconscript))
    SConscript(sconscript, variant_dir=variant_dir, duplicate=0)
