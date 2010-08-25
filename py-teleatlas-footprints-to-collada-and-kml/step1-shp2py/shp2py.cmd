@echo off
"C:\Program Files\Blender Foundation\Blender\blender.exe" -b empty.blend -P shp2py_main.py %*
del C:\tmp\quit.blend
