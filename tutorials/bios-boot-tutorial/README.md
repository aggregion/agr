# Bios Boot Tutorial

The `bios-boot-tutorial.py` script simulates the AGRIO bios boot sequence.

``Prerequisites``:

1. Python 3.x
2. CMake
3. git

``Steps``:

1. Install agrio binaries by following the steps outlined in below tutorial
[Install agrio binaries](https://github.com/aggregion/agr#mac-os-x-brew-install)

2. Install agrio.cdt binaries by following the steps outlined in below tutorial
[Install agrio.cdt binaries](https://github.com/aggregion/agrio.cdt#binary-releases)

3. Compile agrio.contracts

```bash
$ cd ~
$ git clone https://github.com/aggregion/agrio.contracts.git
$ cd ./agrio.contracts/
$ ./build.sh
$ pwd

```

4. Make note of the directory where the contracts were compiled
The last command in the previous step printed on the bash console the contracts' directory, make note of it, we'll reference it from now on as `AGRIO_CONTRACTS_DIRECTORY`

5. Launch the `bios-boot-tutorial.py` script
Minimal command line to launch the script below, make sure you replace `AGRIO_CONTRACTS_DIRECTORY` with actual directory

```bash
$ cd ~
$ git clone https://github.com/aggregion/agr.git
$ cd ./agr/tutorials/bios-boot-tutorial/
$ python3 bios-boot-tutorial.py --clagr="clagr --wallet-url http://127.0.0.1:6666 " --nodagr=nodagr --kagrd=kagrd --contracts-dir="/AGRIO_CONTRACTS_DIRECTORY/" -a

```

See [AGRIO Documentation Wiki: Tutorial - Bios Boot](https://github.com/aggregion/agr/wiki/Tutorial-Bios-Boot-Sequence) for additional information.