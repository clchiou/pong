import os


env = Environment(CPPPATH='#', CPPFLAGS='-O2 -Wall -Werror -std=c++11')
Export('env')

sconscripts = ['base/SConscript',
               'graphics/SConscript',
               'platform/linux/SConscript',
               'platform/graphics/linux/SConscript',
               'third_party/box2d/SConscript']

for sconscript in sconscripts:
    variant_dir = os.path.join('out', os.path.dirname(sconscript))
    SConscript(sconscript, variant_dir=variant_dir, duplicate=0)
