#!/usr/bin/env bash

echo '===================='
echo '= Buzz Uninstaller ='
echo '===================='
echo
echo 'To complete this operation, you will need administrative privileges.'
echo
read -p 'Press CTRL-C to exit, or any other key to proceed...'
echo
echo 'Uninstalling Buzz... '

sudo rm -rf /usr/local/doc/buzz
sudo rm -rf /usr/local/lib/libbuzz*
sudo rm -rf /usr/local/lib/pkgconfig/buzz.pc
sudo rm -rf /usr/local/include/buzz
sudo rm -rf /usr/local/bin/bzzc
sudo rm -rf /usr/local/bin/bzzparse
sudo rm -rf /usr/local/bin/bzzasm
sudo rm -rf /usr/local/bin/bzzdeasm
sudo rm -rf /usr/local/bin/bzzrun
sudo rm -rf /usr/local/share/buzz
sudo rm -rf /usr/local/share/man/man1/bzzc.1.gz
sudo rm -rf /usr/local/share/man/man1/bzzparse.1.gz
sudo rm -rf /usr/local/share/man/man1/bzzasm.1.gz
sudo rm -rf /usr/local/share/man/man1/bzzdeasm.1.gz
sudo rm -rf /usr/local/share/man/man1/bzzrun.1.gz
sudo rm -rf /usr/local/lib/argos3/libargos3plugin_simulator_buzz.*
sudo rm -rf /usr/share/cmake-3.5/Modules/FindBuzz.cmake
sudo rm -rf /usr/share/cmake-3.5/Modules/UseBuzz.cmake
