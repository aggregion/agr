#! /bin/bash

if [ -d "/usr/local/include/agrio" ]; then
   printf "\n\tOld agrio install needs to be removed.\n\n"
   printf "\tDo you wish to remove this install? (requires sudo)\n"
   select yn in "Yes" "No"; do
      case $yn in
         [Yy]* )
            if [ "$(id -u)" -ne 0 ]; then
               printf "\n\tThis requires sudo, please run ./scripts/clean_old_install.sh with sudo\n\n"
               exit -1
            fi
            pushd /usr/local &> /dev/null

            pushd include &> /dev/null
            rm -rf appbase chainbase agrio agrio.system agriolib fc libc++ musl &> /dev/null
            popd &> /dev/null

            pushd bin &> /dev/null
            rm clagr agrio-abigen agrio-applesdemo agrio-launcher agrio-s2wasm agrio-wast2wasm agriocpp kagrd nodagr &> /dev/null
            popd &> /dev/null

            libraries=(libagrio_testing
            libagrio_chain
            libfc
            libbinaryen
            libWAST
            libWASM
            libRuntime
            libPlatform
            libIR
            libLogging
            libsoftfloat
            libchainbase
            libappbase
            libbuiltins)
            pushd lib &> /dev/null
            for lib in ${libraries[@]}; do
               rm ${lib}.a ${lib}.dylib ${lib}.so &> /dev/null
            done
            popd &> /dev/null

            pushd etc &> /dev/null
            rm agrio &> /dev/null
            popd &> /dev/null

            pushd share &> /dev/null
            rm agrio &> /dev/null
            popd &> /dev/null

            pushd usr/share &> /dev/null
            rm agrio &> /dev/null
            popd &> /dev/null

            pushd var/lib &> /dev/null
            rm agrio &> /dev/null
            popd &> /dev/null

            pushd var/log &> /dev/null
            rm agrio &> /dev/null
            popd &> /dev/null

            popd &> /dev/null
            break;;
         [Nn]* )
            printf "\tAborting uninstall\n\n"
            exit -1;;
      esac
   done
fi
