#  !/bin/bash #

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
ROOT_MOSQUITTO="${SCRIPT_DIR}/mosquitto"

echo ">>> docker run --rm -it -p 1883:1883 -p 9001:9001 -v "${ROOT_MOSQUITTO}/config":/mosquitto/config -v "${ROOT_MOSQUITTO}/data":/mosquitto/data -v "${ROOT_MOSQUITTO}/log":/mosquitto/log eclipse-mosquitto"
docker run --rm -d -p 1883:1883 -p 7001:9001 -v "${ROOT_MOSQUITTO}/config":/mosquitto/config -v "${ROOT_MOSQUITTO}/data":/mosquitto/data -v "${ROOT_MOSQUITTO}/log":/mosquitto/log eclipse-mosquitto
