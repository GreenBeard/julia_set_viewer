# Licensed under the wxWindows Library Licence (soon to be called wxWidgets Library License)
cp /opt/mingw_libs/wxwidgets/lib/*.dll ./bin
# Licensed under MIT license if you look at the COPYING file
# It is not called the MIT license (however the text appears the same)
cp /usr/x86_64-w64-mingw32/lib/libwinpthread-1.dll ./bin

# The following three are licensed under the runtime exception
cp /usr/lib/gcc/x86_64-w64-mingw32/5.3-win32/libgomp-1.dll ./bin
cp /usr/lib/gcc/x86_64-w64-mingw32/5.3-win32/libstdc++-6.dll ./bin
cp /usr/lib/gcc/x86_64-w64-mingw32/5.3-win32/libgcc_s_seh-1.dll ./bin
