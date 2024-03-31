# Papa MQTT

Programme d'essai pour piloter un ESP8266 par MQTT.

## Configuration

### étape 1 le brocker MQTT

* pré-requis: installation de docker

Démarrage d'un serveur de messagerie (avec le minimum vital).

* récupération de l'image docker:
    * `docker pull eclipse-mosquitto`
* créer des dossiers pour les données persistantes de mosquitto `ROOT_MOSQUITTO`:
    * `ROOT_MOSQUITTO/config`
        * `ROOT_MOSQUITTO/config/mosquitto.conf`
* Minimum vital du fichier conf
    * `listener 1883`
    * `allow_anonymous true`
* Lancement du serveur:
    * `docker run -d -p 1883:1883 -v ${ROOT_MOSQUITTO}/config/mosquitto.conf:/mosquitto/config/mosquitto.conf eclipse-mosquitto`

### étape 2 le client MQTT

On peut recommander l'utilisation d'un client MQTT a des fins de debug: [MQTTX](https://mqttx.app/)

On crée une nouvelle connexion:

* `name`: selon l'envie
* `Client ID`: quelque chose d'unique
* `Host`: `mqtt://` `localhost` (ou le nom ou l'IP de la machine qui fait tourner le broker)
* `Port`: `1883` (ou un autre port si mappé différement)

On lance la connexion.

On crée une nouvelle Subscription

* Topic: `TopicESP/#`

### étape 3: l'ESP

Il faut câbler la cathode d'une LED sur le port `D5`

Ainsi qu'un intérupteur sur le port `D6` (pouvant basculer du niveau `HIGH` à `LOW`).

Adaptation au réseau: Dasn le fichier `WifiManager.cpp`, changer les valeurs
de `ssid` et `password` pour coller à votre config réseau.

Compiler et uploader le sketch.
Il faudra penser à adapter dans le fichier platformio.ini le port d'upload et
de monitor en fonction de la configuration locale.

Le serial monitor est là pour le debug.

## Utilisation

### Ecoute du changement d'état.

Lors de la bascule d'état du port `D6`, un message devrait faire son apparition
Dans la console de MQTTX avec le topic `TopicESP/e1` et le message `Switch On` ou `Switch Off`.

### Passer un ordre

Dans MQTTX, tout en bas, Saisir le topic `TOPICESP/order` (attention à la casse).
Dans le message mettre `on` ou `off` en fonction de votre volonté d'alumer ou éteindre la LED.
