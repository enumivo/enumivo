# Run in docker

Simple and fast setup of Enumivo on Docker is also available.

## Install Dependencies

- [Docker](https://docs.docker.com) Docker 17.05 or higher is required
- [docker-compose](https://docs.docker.com/compose/) version >= 1.10.0

## Docker Requirement

- At least 7GB RAM (Docker -> Preferences -> Advanced -> Memory -> 7GB or above)
- If the build below fails, make sure you've adjusted Docker Memory settings and try again.

## Build enu image

```bash
git clone https://github.com/enumivo/enumivo.git --recursive  --depth 1
cd enumivo/Docker
docker build . -t enumivo/eun
```

The above will build off the most recent commit to the master branch by default. If you would like to target a specific branch/tag, you may use a build argument. For example, if you wished to generate a docker image based off of the v1.3.2 tag, you could do the following:

```bash
docker build -t enumivo/enu:v1.3.2 --build-arg branch=v1.3.2 .
```

By default, the symbol in enumivo.system is set to SYS. You can override this using the symbol argument while building the docker image.

```bash
docker build -t enumivo/enu --build-arg symbol=<symbol> .
```

## Start enunode docker container only

```bash
docker run --name enunode -p 8888:8888 -p 9876:9876 -t enumivo/enu nodenud.sh -e --http-alias=enunode:8888 --http-alias=127.0.0.1:8888 --http-alias=localhost:8888 arg1 arg2
```

By default, all data is persisted in a docker volume. It can be deleted if the data is outdated or corrupted:

```bash
$ docker inspect --format '{{ range .Mounts }}{{ .Name }} {{ end }}' enunode
fdc265730a4f697346fa8b078c176e315b959e79365fc9cbd11f090ea0cb5cbc
$ docker volume rm fdc265730a4f697346fa8b078c176e315b959e79365fc9cbd11f090ea0cb5cbc
```

Alternately, you can directly mount host directory into the container

```bash
docker run --name enunode -v /path-to-data-dir:/opt/enumivo/bin/data-dir -p 8888:8888 -p 9876:9876 -t enumivo/enu nodenud.sh -e --http-alias=enunode:8888 --http-alias=127.0.0.1:8888 --http-alias=localhost:8888 arg1 arg2
```

## Get chain info

```bash
curl http://127.0.0.1:8888/v1/chain/get_info
```

## Start both enunode and enuwallet containers

```bash
docker volume create --name=enunode-data-volume
docker volume create --name=enuwallet-data-volume
docker-compose up -d
```

After `docker-compose up -d`, two services named `nodenud` and `enuwallet` will be started. enunode service would expose ports 8888 and 9876 to the host. enuwallet service does not expose any port to the host, it is only accessible to cleos when running cleos is running inside the enuwallet container as described in "Execute cleos commands" section.

### Execute cleos commands

You can run the `cleos` commands via a bash alias.

```bash
alias cleos='docker-compose exec enuwallet /opt/enumivo/bin/cleos -u http://nodenud:8888 --wallet-url http://localhost:8900'
cleos get info
cleos get account inita
```

Upload sample exchange contract

```bash
cleos set contract exchange contracts/exchange/
```

If you don't need enuwallet afterwards, you can stop the enuwallet service using

```bash
docker-compose stop enuwallet
```

### Develop/Build custom contracts

Due to the fact that the enumivo/enu image does not contain the required dependencies for contract development (this is by design, to keep the image size small), you will need to utilize the enumivo/enu-dev image. This image contains both the required binaries and dependencies to build contracts using eosiocpp.

You can either use the image available on [Docker Hub](https://hub.docker.com/r/enumivo/enu-dev/) or navigate into the dev folder and build the image manually.

```bash
cd dev
docker build -t enumivo/enu-dev .
```

### Change default configuration

You can use docker compose override file to change the default configurations. For example, create an alternate config file `config2.ini` and a `docker-compose.override.yml` with the following content.

```yaml
version: "2"

services:
  enunode:
    volumes:
      - enunode-data-volume:/opt/enumivo/bin/data-dir
      - ./config2.ini:/opt/enumivo/bin/data-dir/config.ini
```

Then restart your docker containers as follows:

```bash
docker-compose down
docker-compose up
```

### Clear data-dir

The data volume created by docker-compose can be deleted as follows:

```bash
docker volume rm enunode-data-volume
docker volume rm enuwallet-data-volume
```

### Docker Hub

Docker Hub image available from [docker hub](https://hub.docker.com/r/enumivo/enu/).
Create a new `docker-compose.yaml` file with the content below

```bash
version: "3"

services:
  nodenud:
    image: enumivo/enu:latest
    command: /opt/enumivo/bin/nodenud.sh --data-dir /opt/enumivo/bin/data-dir -e --http-alias=nodenud:8888 --http-alias=127.0.0.1:8888 --http-alias=localhost:8888
    hostname: nodenud
    ports:
      - 8888:8888
      - 9876:9876
    expose:
      - "8888"
    volumes:
      - enunode-data-volume:/opt/enumivo/bin/data-dir

  enuwallet:
    image: enumivo/enu:latest
    command: /opt/enumivo/bin/enuwallet --wallet-dir /opt/enumivo/bin/data-dir --http-server-address=127.0.0.1:8900 --http-alias=localhost:8900 --http-alias=enuwallet:8900
    hostname: enuwallet
    links:
      - nodenud
    volumes:
      - enuwallet-data-volume:/opt/enumivo/bin/data-dir

volumes:
  enunode-data-volume:
  enuwallet-data-volume:

```

*NOTE:* the default version is the latest, you can change it to what you want

run `docker pull enumivo/enu:latest`

run `docker-compose up`

### enumivo Testnet

We can easily set up a enumivo local testnet using docker images. Just run the following commands:

Note: if you want to use the mongo db plugin, you have to enable it in your `data-dir/config.ini` first.

```
# create volume
docker volume create --name=enunode-data-volume
docker volume create --name=enuwallet-data-volume
# pull images and start containers
docker-compose -f docker-compose-enumivo-latest.yaml up -d
# get chain info
curl http://127.0.0.1:8888/v1/chain/get_info
# get logs
docker-compose logs -f nodenud
# stop containers
docker-compose -f docker-compose-enumivo-latest.yaml down
```

The `blocks` data are stored under `--data-dir` by default, and the wallet files are stored under `--wallet-dir` by default, of course you can change these as you want.

### About MongoDB Plugin

Currently, the mongodb plugin is disabled in `config.ini` by default, you have to change it manually in `config.ini` or you can mount a `config.ini` file to `/opt/enumivo/bin/data-dir/config.ini` in the docker-compose file.
