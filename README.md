# Mosquitto Authentication Plugin

[![CI](https://github.com/dkhalife/eclipse-mosquitto-with-auth/actions/workflows/ci.yml/badge.svg)](https://github.com/dkhalife/eclipse-mosquitto-with-auth/actions/workflows/ci.yml)

This repository contains a C++ authentication plugin for the [Eclipse Mosquitto](https://mosquitto.org/) MQTT broker. The plugin supports multiple backends so that client credentials can be validated from different sources.

Currently a file based backend and an HTTP backend are implemented. MySQL and SQLite backends exist as stubs for future work.

## 🛠️ Building

The project uses `clang` and requires Mosquitto development headers. On Alpine based systems the following packages are needed:

```
apk add clang clang-extra-tools mosquitto-dev mariadb-dev sqlite-dev curl
```

To compile the plugin run:

```sh
cd src
make
```

The result will be `auth-plugin.so` in the `src` directory. Use `make clean` to remove build artefacts.

## 🐳 Docker image

A simple Dockerfile is provided under `docker/`. After building the plugin you can create an image containing Mosquitto with the plugin installed:

```sh
cd docker
# copy the built plugin into the docker context first
cp ../src/auth-plugin.so .
docker build -t mosquitto-auth-plugin .
```

## ⚙️ Configuring Mosquitto

Load the plugin in `mosquitto.conf` and specify the desired backend list. Example:

```
plugin /path/to/auth-plugin.so
plugin_opt_backends file,http

# File backend options
plugin_opt_creds_file /etc/mosquitto/creds.txt

# HTTP backend options
plugin_opt_http_host auth.example.com
plugin_opt_use_https true
plugin_opt_http_port 443
plugin_opt_http_auth_path /auth
plugin_opt_http_acl_path /acl
```

The credentials file used by the file backend must contain entries of the form `username::sha256(password)` on separate lines. The HTTP backend sends JSON requests to the configured endpoints.

The HTTP backend initializes libcurl when it is loaded and cleans up the
library on shutdown, so HTTP requests incur no extra initialization overhead.

## 💻 Devcontainer & Codespaces

The repository includes a `.devcontainer` folder so you can open it in Visual Studio Code or GitHub Codespaces and get a fully configured environment automatically.

## 📁 Repository layout

- `src/` – source code and Makefile
- `docker/` – Docker image definition
- `.devcontainer/` – development container configuration


