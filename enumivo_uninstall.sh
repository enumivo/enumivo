#! /bin/bash

binaries=(enucli
          enugenabi
          enulauncher
          enumivo-s2wasm
          enumivo-wast2wasm
          enunodecpp
          enuwallet
          enunode
          enuapplesdemo)

if [ -d "/usr/local/enumivo" ]; then
   printf "\tDo you wish to remove this install? (requires sudo)\n"
   select yn in "Yes" "No"; do
      case $yn in
         [Yy]* )
            if [ "$(id -u)" -ne 0 ]; then
               printf "\n\tThis requires sudo, please run ./enumivo_uninstall.sh with sudo\n\n"
               exit -1
            fi

            pushd /usr/local &> /dev/null
            rm -rf enumivo
            pushd bin &> /dev/null
            for binary in ${binaries[@]}; do
               rm ${binary}
            done
            # Handle cleanup of directories created from installation
            if [ "$1" == "--full" ]; then
               if [ -d ~/Library/Application\ Support/enumivo ]; then rm -rf ~/Library/Application\ Support/enumivo; fi # Mac OS
               if [ -d ~/.local/share/enumivo ]; then rm -rf ~/.local/share/enumivo; fi # Linux
            fi
            popd &> /dev/null
            break;;
         [Nn]* )
            printf "\tAborting uninstall\n\n"
            exit -1;;
      esac
   done
fi
