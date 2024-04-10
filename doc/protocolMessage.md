# Protocole de messagerie MQTT

Un message MQTT est composé de 2 éléments, un `topic` et un `payload`.

Le `topic` permet de gérer les abonnements.

## Encodage du topic

Le topic permettant de gérer les abonnements, il est important de bien 'ranger'
les messages pour éviter que chaque périphérique ne soit contraint de traiter
tous les messages (un premier filtre est donc réalisé en ne s'abonnant pas à tout).

Ainsi, le choix pour le 'codage' du topic sera:

* prefix (`train`)
* type
    * `cmd` encodage d'un ordre.
    * `state` message publiant un état
    * `debug` message informatif pour le debug
    * `emerg` pour traiter les urgences (sécurité)
* device type
    * `aig` aiguillage
    * `pos` capteur de position
    * `feu` pour un feu de signalisation
* device id -> quel device est concerné ou a publié

Ainsi un topic comme `train/cmd/aig/4` sera identifié comme "le payload est une
commande à destination de l'aiguillage numéro 4"

## Encodage du payload

L'encodage du payload va dépendre fortement du type de device concerné par le message.

De même que le format sera assez libre pour les messages de debug.
