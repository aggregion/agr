#! /bin/bash

binaries=(clagr
          agrio-abigen
          agrio-launcher
          agrio-s2wasm
          agrio-wast2wasm
          agriocpp
          kagrd
          nodagr
          agrio-applesdemo)

if [ -d "/usr/local/agrio" ]; then
   printf "\tDo you wish to remove this install? (requires sudo)\n"
   select yn in "Yes" "No"; do
      case $yn in
         [Yy]* )
            if [ "$(id -u)" -ne 0 ]; then
               printf "\n\tThis requires sudo, please run ./agrio_uninstall.sh with sudo\n\n"
               exit -1
            fi

            pushd /usr/local &> /dev/null
            rm -rf agrio
            pushd bin &> /dev/null
            for binary in ${binaries[@]}; do
               rm ${binary}
            done
            popd &> /dev/null
            break;;
         [Nn]* ) 
            printf "\tAborting uninstall\n\n"
            exit -1;;
      esac
   done
fi
