# Prerequisites

Please make sure you have installed next utilities:
* docker & docker-compose (How to install on ubuntu-like systems https://docs.docker.com/install/linux/docker-ce/ubuntu/)
* git

Also make sure that you have at least 500 GB free space for docker containers.

# Installation
```sh
mkdir -p ~/agr
cd ~/agr
git clone https://github.com/aggregion/agr.git .
```

# Run TESTNET node
Please make sure that you are inside the directory where AGR is installed.
Go to the Docker directory:
```sh
cd ./Docker
```
First, copy the config.sample.testnet.ini to config.ini, using the following command:
```sh
cp config.sample.testnet.ini config.ini
```

Run testnet docker-containers:
```sh
sudo docker-compose up -f docker-compose.testnet.yml -d
```

Check that node is running:

```sh
curl 'http://127.0.0.1:8888/v1/chain/get_info'
```

This command should output information about blockchain. For example:
```json
{"server_version":"31134471","chain_id":"cf057bbfb72640471fd910bcb67639c22df9f92470936cddc1ade0e2f2e7dc4f","head_block_num":1513438,"last_irreversible_block_num":1513437,"last_irreversible_block_id":"001717dd818a1ebdf045e3a5d7c76649700250926443d654e2f283ac236c4139","head_block_id":"001717de83945ba35ef9cb170ed89413035f44724031c382d87b5f6119654ac1","head_block_time":"2018-09-18T06:24:10.000","head_block_producer":"agrio","virtual_block_cpu_limit":200000000,"virtual_block_net_limit":1048576000,"block_cpu_limit":199900,"block_net_limit":1048576,"server_version_string":"v1.2.4-1-g311344717"}
```

# Run MAINNET node

### Generate key

First you need to generate AGR key-pair and account.
You may do it within AGR Wallet desktop application.
*Do not share your private key with anyone!*

### Build and run containers

Make sure that you have next:
* Your account name
* Public key (started with AGR)
* Private key

Copy the config.sample.ini to config.ini, using the following command:

```sh
cp config.sample.ini config.ini
```

Edit next parameters in the config.ini file:
* signature-provider = <public_key>=KEY:<private_key> - uncomment line and write your public and private key here
* producer-name = <your_account_name> - enter your account name here

Run mainnet docker-containers:
```sh
sudo docker volume create nodagr-data-volume
sudo docker volume create kagrd-data-volume
sudo docker-compose up -f docker-compose.yml -d
```

Check that node is running:

```sh
curl 'http://127.0.0.1:8888/v1/chain/get_info'
```

This command should output information about blockchain. For example:
```json
{"server_version":"1fc065f4","chain_id":"41c4c54a375c767f11654a907541633bdddf158a8f0c394f657980bc484c91c9","head_block_num":1863547,"last_irreversible_block_num":1863546,"last_irreversible_block_id":"001c6f7ad42ba54ec3eb7adb7d93ae867bf0c8a6d05a9f9a68e9683e7af2b40a","head_block_id":"001c6f7b784f21192d8682cd843d5b1bbc77d66becbfe0d87538af1909b9121c","head_block_time":"2018-09-18T06:40:58.500","head_block_producer":"agrio","virtual_block_cpu_limit":200000000,"virtual_block_net_limit":1048576000,"block_cpu_limit":199900,"block_net_limit":1048576,"server_version_string":"v1.2.1-27-g1fc065f40"}
```

# Troubleshooting

You can check that all containers are running using the following command:

```sh
sudo docker ps
```

Output should looks like this:
```
bc102b6e90aa        mongo                    "docker-entrypoint.s…"   44 hours ago        Up 44 hours         27017/tcp                                         docker_mongodb_1
c2e6dd779e65        agrio/agr                "/opt/agrio/bin/kagr…"   44 hours ago        Up 44 hours                                                           docker_kagrd_1
7d07e5319daf        agrio/agr                "/opt/agrio/bin/noda…"   44 hours ago        Up 44 hours         0.0.0.0:8888->8888/tcp, 0.0.0.0:9876->9876/tcp    docker_nodagrd_1
```

It means that all 3 containers are running:
* docker_mongodb_1
* docker_kagrd_1
* docker_nodagrd_1

If you don't see any of this containers, it means that something went wrong.

You can just try to run specific container again. For example docker_nodagrd_1:
```sh
sudo docker start docker_nodagrd_1
```

If after this container did not apear in the list, then you need see logs for detailed information. For example if you need logs for docker_nodagrd_1, run this command:

```sh
sudo docker logs docker_nodagrd_1 --tail 200
```

For any questions you can contact us using e-mail support@aggregion.com.
