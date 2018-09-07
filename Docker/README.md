# Run in docker

Simple and fast setup of AGR.IO on Docker is also available.

## Install Dependencies

- [Docker](https://docs.docker.com) Docker 17.05 or higher is required
- [docker-compose](https://docs.docker.com/compose/) version >= 1.10.0

## Docker Requirement

- At least 7GB RAM (Docker -> Preferences -> Advanced -> Memory -> 7GB or above)
- If the build below fails, make sure you've adjusted Docker Memory settings and try again.

## Build agr image

```bash
git clone https://github.com/aggregion/agr.git --recursive  --depth 1
cd agr/Docker
docker build . -t agrio/agr
```

The above will build off the most recent commit to the master branch by default. If you would like to target a specific branch/tag, you may use a build argument. For example, if you wished to generate a docker image based off of the v1.2.4 tag, you could do the following:

```bash
docker build -t agrio/agr:v1.2.4 --build-arg branch=v1.2.4 .
```

By default, the symbol in agrio.system is set to SYS. You can override this using the symbol argument while building the docker image.

```bash
docker build -t agrio/agr --build-arg symbol=<symbol> .
```

## Start nodagr docker container only

```bash
docker run --name nodagr -p 8888:8888 -p 9876:9876 -t agrio/agr nodagrd.sh -e --http-alias=nodagr:8888 --http-alias=127.0.0.1:8888 --http-alias=localhost:8888 arg1 arg2
```

By default, all data is persisted in a docker volume. It can be deleted if the data is outdated or corrupted:

```bash
$ docker inspect --format '{{ range .Mounts }}{{ .Name }} {{ end }}' nodagr
fdc265730a4f697346fa8b078c176e315b959e79365fc9cbd11f090ea0cb5cbc
$ docker volume rm fdc265730a4f697346fa8b078c176e315b959e79365fc9cbd11f090ea0cb5cbc
```

Alternately, you can directly mount host directory into the container

```bash
docker run --name nodagr -v /path-to-data-dir:/opt/agrio/bin/data-dir -p 8888:8888 -p 9876:9876 -t agrio/agr nodagrd.sh -e --http-alias=nodagr:8888 --http-alias=127.0.0.1:8888 --http-alias=localhost:8888 arg1 arg2
```

## Get chain info

```bash
curl http://127.0.0.1:8888/v1/chain/get_info
```

## Start both nodagr and kagrd containers

```bash
docker volume create --name=nodagr-data-volume
docker volume create --name=kagrd-data-volume
docker-compose up -d
```

After `docker-compose up -d`, two services named `nodagrd` and `kagrd` will be started. nodagr service would expose ports 8888 and 9876 to the host. kagrd service does not expose any port to the host, it is only accessible to clagr when running clagr is running inside the kagrd container as described in "Execute clagr commands" section.

### Execute clagr commands

You can run the `clagr` commands via a bash alias.

```bash
alias clagr='docker-compose exec kagrd /opt/agrio/bin/clagr -u http://nodagrd:8888 --wallet-url http://localhost:8900'
clagr get info
clagr get account inita
```

Upload sample exchange contract

```bash
clagr set contract exchange contracts/exchange/
```

If you don't need kagrd afterwards, you can stop the kagrd service using

```bash
docker-compose stop kagrd
```

### Develop/Build custom contracts

Due to the fact that the agrio/agr image does not contain the required dependencies for contract development (this is by design, to keep the image size small), you will need to utilize the agrio/agr-dev image. This image contains both the required binaries and dependencies to build contracts using agriocpp.

You can either use the image available on [Docker Hub](https://hub.docker.com/r/agrio/agr-dev/) or navigate into the dev folder and build the image manually.

```bash
cd dev
docker build -t agrio/agr-dev .
```

### Change default configuration

You can use docker compose override file to change the default configurations. For example, create an alternate config file `config2.ini` and a `docker-compose.override.yml` with the following content.

```yaml
version: "2"

services:
  nodagr:
    volumes:
      - nodagr-data-volume:/opt/agrio/bin/data-dir
      - ./config2.ini:/opt/agrio/bin/data-dir/config.ini
```

Then restart your docker containers as follows:

```bash
docker-compose down
docker-compose up
```

### Clear data-dir

The data volume created by docker-compose can be deleted as follows:

```bash
docker volume rm nodagr-data-volume
docker volume rm kagrd-data-volume
```

### Docker Hub

Docker Hub image available from [docker hub](https://hub.docker.com/r/agrio/agr/).
Create a new `docker-compose.yaml` file with the content below

```bash
version: "3"

services:
  nodagrd:
    image: agrio/agr:latest
    command: /opt/agrio/bin/nodagrd.sh --data-dir /opt/agrio/bin/data-dir -e --http-alias=nodagrd:8888 --http-alias=127.0.0.1:8888 --http-alias=localhost:8888
    hostname: nodagrd
    ports:
      - 8888:8888
      - 9876:9876
    expose:
      - "8888"
    volumes:
      - nodagr-data-volume:/opt/agrio/bin/data-dir

  kagrd:
    image: agrio/agr:latest
    command: /opt/agrio/bin/kagrd --wallet-dir /opt/agrio/bin/data-dir --http-server-address=127.0.0.1:8900 --http-alias=localhost:8900 --http-alias=kagrd:8900
    hostname: kagrd
    links:
      - nodagrd
    volumes:
      - kagrd-data-volume:/opt/agrio/bin/data-dir

volumes:
  nodagr-data-volume:
  kagrd-data-volume:

```

*NOTE:* the default version is the latest, you can change it to what you want

run `docker pull agrio/agr:latest`

run `docker-compose up`

### AGRIO Testnet

We can easily set up a AGRIO local testnet using docker images. Just run the following commands:

Note: if you want to use the mongo db plugin, you have to enable it in your `data-dir/config.ini` first.

```
# create volume
docker volume create --name=nodagr-data-volume
docker volume create --name=kagrd-data-volume
# pull images and start containers
docker-compose -f docker-compose-agrio-latest.yaml up -d
# get chain info
curl http://127.0.0.1:8888/v1/chain/get_info
# get logs
docker-compose logs -f nodagrd
# stop containers
docker-compose -f docker-compose-agrio-latest.yaml down
```

The `blocks` data are stored under `--data-dir` by default, and the wallet files are stored under `--wallet-dir` by default, of course you can change these as you want.

### About MongoDB Plugin

Currently, the mongodb plugin is disabled in `config.ini` by default, you have to change it manually in `config.ini` or you can mount a `config.ini` file to `/opt/agrio/bin/data-dir/config.ini` in the docker-compose file.
