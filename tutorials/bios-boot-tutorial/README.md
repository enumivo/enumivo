# Bios Boot Tutorial

The `bios-boot-tutorial.py` script simulates the Enumivo bios boot sequence.

``Prerequisites``:

1. Python 3.x
2. CMake
3. git

``Steps``:

1. Install enumivo binaries 
2. Install enumivo.cdt binaries
3. Compile enumivo.contracts

```bash
$ cd ~
$ git clone https://github.com/enumivo/enumivo.contracts.git
$ cd ./enumivo.contracts/
$ ./build.sh
$ pwd

```

4. Make note of the directory where the contracts were compiled
The last command in the previous step printed on the bash console the contracts' directory, make note of it, we'll reference it from now on as `ENUMIVO_CONTRACTS_DIRECTORY`

5. Launch the `bios-boot-tutorial.py` script
Minimal command line to launch the script below, make sure you replace `ENUMIVO_CONTRACTS_DIRECTORY` with actual directory

```bash
$ cd ~
$ git clone https://github.com/enumivo/enumivo.git
$ cd ./enumivo/tutorials/bios-boot-tutorial/
$ python3 bios-boot-tutorial.py --enucli="enucli --wallet-url http://127.0.0.1:6666 " --enunode=enunode --enuwallet=enuwallet --contracts-dir="/ENUMIVO_CONTRACTS_DIRECTORY/" -a

```