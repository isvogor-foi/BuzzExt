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

sudo rm -rf /usr/local/khepera4-yocto/build/tmp/sysroots/i686-linux/usr/doc/buzz
sudo rm -rf /usr/local/khepera4-yocto/build/tmp/sysroots/i686-linux/usr/lib/libbuzz.*
sudo rm -rf /usr/local/khepera4-yocto/build/tmp/sysroots/i686-linux/usr/lib/pkgconfig/buzz.pc
sudo rm -rf /usr/local/khepera4-yocto/build/tmp/sysroots/i686-linux/usr/include/buzz
sudo rm -rf /usr/local/khepera4-yocto/build/tmp/sysroots/i686-linux/usr/bin/bzzc
sudo rm -rf /usr/local/khepera4-yocto/build/tmp/sysroots/i686-linux/usr/bin/bzzparse
sudo rm -rf /usr/local/khepera4-yocto/build/tmp/sysroots/i686-linux/usr/bin/bzzasm
sudo rm -rf /usr/local/khepera4-yocto/build/tmp/sysroots/i686-linux/usr/bin/bzzdeasm
sudo rm -rf /usr/local/khepera4-yocto/build/tmp/sysroots/i686-linux/usr/bin/bzzrun
sudo rm -rf /usr/local/khepera4-yocto/build/tmp/sysroots/i686-linux/usr/share/buzz
sudo rm -rf /usr/local/khepera4-yocto/build/tmp/sysroots/i686-linux/usr/share/man/man1/bzzc.1.gz
sudo rm -rf /usr/local/khepera4-yocto/build/tmp/sysroots/i686-linux/usr/share/man/man1/bzzparse.1.gz
sudo rm -rf /usr/local/khepera4-yocto/build/tmp/sysroots/i686-linux/usr/share/man/man1/bzzasm.1.gz
sudo rm -rf /usr/local/khepera4-yocto/build/tmp/sysroots/i686-linux/usr/share/man/man1/bzzdeasm.1.gz
sudo rm -rf /usr/local/khepera4-yocto/build/tmp/sysroots/i686-linux/usr/share/man/man1/bzzrun.1.gz
sudo rm -rf /lib/argos3/libargos3plugin_simulator_buzz.*
