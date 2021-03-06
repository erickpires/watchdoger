mkdir "lib\native\win32\8\x64\"
mkdir "lib\native\win32\9\x64\"
mkdir "lib\native\win32\10\x64\"
mkdir "lib\native\win32\11\x64\"

mkdir "lib\native\win32\8\ia32\"
mkdir "lib\native\win32\9\ia32\"
mkdir "lib\native\win32\10\ia32\"
mkdir "lib\native\win32\11\ia32\"

call node-gyp clean configure build --arch=x64 --target=v8.14.0
move /Y "build\Release\watchdoger.node" "lib\native\win32\8\x64\"
call node-gyp clean configure build --arch=ia32 --target=v8.14.0
move /Y "build\Release\watchdoger.node" "lib\native\win32\8\ia32\"

call node-gyp clean configure build --arch=x64 --target=v9.11.2
move /Y "build\Release\watchdoger.node" "lib\native\win32\9\x64\"
call node-gyp clean configure build --arch=ia32 --target=v9.11.2
move /Y "build\Release\watchdoger.node" "lib\native\win32\9\ia32\"

call node-gyp clean configure build --arch=x64 --build_v8_with_gn=false --target=v10.14.2
move /Y "build\Release\watchdoger.node" "lib\native\win32\10\x64\"
call node-gyp clean configure build --arch=ia32 --build_v8_with_gn=false --target=v10.14.2
move /Y "build\Release\watchdoger.node" "lib\native\win32\10\ia32\"

call node-gyp clean configure build --arch=x64 --build_v8_with_gn=false --target=v11.4.0
move /Y "build\Release\watchdoger.node" "lib\native\win32\11\x64\"
call node-gyp clean configure build --arch=ia32 --build_v8_with_gn=false --target=v11.4.0
move /Y "build\Release\watchdoger.node" "lib\native\win32\11\ia32\"

rmdir /S /Q build
