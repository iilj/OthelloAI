# OthelloAI

## Overview

[OthelloAI](https://iilj.github.io/OthelloAI/) is an Othello game application for web browsers.

This application uses WebAssembly compiled form C++ Game maneger class, which wraps [schil\-ler/OthelloAI](https://github.com/schil-ler/OthelloAI) Othello AI core.

## Prerequisites

### Install Emscripten

- [Download and install — Emscripten 1\.39\.19 documentation](https://emscripten.org/docs/getting_started/downloads.html)

```sh
$ git clone https://github.com/emscripten-core/emsdk.git
$ cd emsdk
$ ./emsdk install latest
$ ./emsdk activate latest
$ source ./emsdk_env.sh
```

### Verify Emscripten

```sh
$ emcc -v
```

## Generate OthelloGame WebAssembly

```sh
$ cd cpp
$ make
```

## Run locally

```sh
$ sh serve.sh
```

`serve.sh` uses Python http.server class.

## Deploy

Any update of master branch is automatically reflected in GitHub Pages.
For details of the configuration, please refer to `.travis.yml` file and [Emscripten's official documentation](https://emscripten.org/docs/compiling/Travis.html).

## Using Docker container to compile

If using Docker to compile C++ files is preferrable, you can use [trzeci/emscripten](https://hub.docker.com/r/trzeci/emscripten/tags/) container.

```sh
$ docker run -dit --name emscripten -v $(pwd):/src trzeci/emscripten:sdk-incoming-64bit bash
$ docker exec -it emscripten /bin/sh -c "cd cpp && make"
```

## Links

- [OthelloAI](https://iilj.github.io/OthelloAI/)
- [schil\-ler/OthelloAI: α\-β法を使ったオセロAIです。](https://github.com/schil-ler/OthelloAI)
- [Building projects on Travis CI — Emscripten 1\.39\.19 documentation](https://emscripten.org/docs/compiling/Travis.html)
- [trzeci/emscripten Tags \- Docker Hub](https://hub.docker.com/r/trzeci/emscripten/tags/)
