#! /bin/bash

OPT_LOCATION=$HOME/opt

binaries=(
   clagr
   agrio-abigen
   agrio-launcher
   agrio-s2wasm
   agrio-wast2wasm
   agriocpp
   kagrd
   nodagr
   agrio-applesdemo
)

if [ -d $OPT_LOCATION/agrio ]; then
   printf "Do you wish to remove this install? (requires sudo)\n"
   select yn in "Yes" "No"; do
      case $yn in
         [Yy]* )
            if [ "$(id -u)" -ne 0 ]; then
               printf "\nThis requires sudo, please run ./agrio_uninstall.sh with sudo\n\n"
               exit -1
            fi

            pushd $HOME &> /dev/null
            pushd opt &> /dev/null
            rm -rf agrio
            # Handle cleanup of directories created from installation
            if [ "$1" == "--full" ]; then
               if [ -d ~/Library/Application\ Support/agrio ]; then rm -rf ~/Library/Application\ Support/agrio; fi # Mac OS
               if [ -d ~/.local/share/agrio ]; then rm -rf ~/.local/share/agrio; fi # Linux
            fi
            popd &> /dev/null
            pushd bin &> /dev/null
            for binary in ${binaries[@]}; do
               rm ${binary}
            done
            popd &> /dev/null
            pushd lib/cmake &> /dev/null
            rm -rf agrio
            popd &> /dev/null

            break;;
         [Nn]* )
            printf "Aborting uninstall\n\n"
            exit -1;;
      esac
   done
fi

if [ -d "/usr/local/agrio" ]; then
   printf "Do you wish to remove this install? (requires sudo)\n"
   select yn in "Yes" "No"; do
      case $yn in
         [Yy]* )
            if [ "$(id -u)" -ne 0 ]; then
               printf "\nThis requires sudo, please run ./agrio_uninstall.sh with sudo\n\n"
               exit -1
            fi

            pushd /usr/local &> /dev/null
            pushd opt &> /dev/null
            rm -rf agrio
            # Handle cleanup of directories created from installation
            if [ "$1" == "--full" ]; then
               if [ -d ~/Library/Application\ Support/agrio ]; then rm -rf ~/Library/Application\ Support/agrio; fi # Mac OS
               if [ -d ~/.local/share/agrio ]; then rm -rf ~/.local/share/agrio; fi # Linux
            fi
            popd &> /dev/null
            pushd bin &> /dev/null
            for binary in ${binaries[@]}; do
               rm ${binary}
            done
            popd &> /dev/null
            pushd lib/cmake &> /dev/null
            rm -rf agrio
            popd &> /dev/null

            break;;
         [Nn]* )
            printf "Aborting uninstall\n\n"
            exit -1;;
      esac
   done
fi
