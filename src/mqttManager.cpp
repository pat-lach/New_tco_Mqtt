/* 
 * Created by damien on 08/10/23.
 * modified by patrick with assistance of damien 11/10/2023
 * modified by patrick MQTT via Rasberrypi p 14/02/2024
 * modified by patrick added new topic to synchonize all Web Client 13/9/2024
*/

#include "mqttManager.h"
/*
 * static const String mqttUser = "papa";
 * static const String mqttPassword = "papa";
 * static const String mqtt_server = "pat-lach-pil";  // name dockermqtt, http://192.168.1.32:1883  image eclipse-mosquitto:2.0.18
*/
static const String mqttTopicIn = "train/#";   // si Click sur Aig, demande de changement position  et Topic Pub " Aig/cde" mess un nombre
static const IPAddress mqtt_server = {192, 168, 1, 32};  //adress  ou est le brocker modif 14/02 adress Raspi
constexpr uint16_t mqtt_server_port = 1883;
static IOManager *s_ioManager = nullptr;  // Pointeur statique vers une instance de IOManager

/**
 * @brief Function called everytime a message is received from broker.
 * @param topic The topic received
 * @param payload The message received.
 * @param length The length of the message.
 */

// Fonction pour diviser une chaîne en utilisant un séparateur
std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}


static void callback(char *topic, byte *payload, unsigned int length) {
	String Topic(topic); // Conversion des paramètres en String pour une manipulation plus facile
	String Payload;
	String topic_sub;
	String Bobine_id;
    
	String Payload_sub;
	Payload.reserve(length); // Conversion du payload de byte en String

	for (unsigned int i = 0; i < length; ++i) {
		Payload.concat((char) payload[i]);
		}
	Payload_sub = Payload;

	// Séparer le payload en utilisant "/" comme séparateur
    std::vector<std::string> parts = split(Payload.c_str(), '/' );

	// message: app + "/" + aig_id + "/" + client_id;
    if (parts.size() == 3) {
        String app = parts[0].c_str();
        String aig_id = parts[1].c_str();
        String client_id = parts[2].c_str();

	  // Affichage du message reçu sur le port série
		Serial.print(" Callback > Received message: ");
		Serial.print(Topic);
		Serial.print(" > ");
		Serial.print(Payload);
		Serial.print(" ");

    } else {
        Serial.print(" callback > Simple payload: ");
		Serial.print(Payload);
		Serial.println("");
    }

	if (Topic == "train/cmd/aig") { 
		int app = Payload.toInt();  // Conversion du payload en entier
		int Bobine_id =16;
		Serial.print(" Callback > train/cmd/aig > ");
		Serial.print(app);
		const byte CdeBobine[] = {11, 12, 21, 22, 31, 32, 33, 34, 41, 42, 51, 52, 61, 62, 63, 64};
		for (int i = 0; i < 16; i++) {   
			if (CdeBobine[i] == app) {       // Recherche de cmdAig_ID dans le tableau de commandes
				Bobine_id = i;               //  bobine à piloter 
				Serial.print("  Bobine id ");
				Serial.println(Bobine_id);
			}
		}  
		// Vérification de la validité du numéro de sortie et activation du relais correspondant
		if (Bobine_id >= 0 && Bobine_id < 16) { 	//if (Payload.equalsIgnoreCase("on"))
			if (s_ioManager) {
				s_ioManager->setLEDState(Bobine_id, true, topic_sub, Payload_sub);
			}
		} else if (Payload.equalsIgnoreCase("off")) {
			if (s_ioManager) {
				s_ioManager->setLEDState(Bobine_id, false, topic_sub, Payload_sub);
			}
		} else {
			Serial.print(" Invalid Payload: ");
			Serial.println(Payload);
		}
	}
} 

MqttManager::MqttManager() : mqttClient(wifiClient) {} // Constructeur de MqttManager

void MqttManager::setup() {
	mqttClient.setServer(mqtt_server, mqtt_server_port); // Configuration du serveur MQTT
	mqttClient.setKeepAlive(5); // Définition de l'intervalle de keep-alive à 5 secondes
	mqttClient.setCallback(callback); // Définition de la fonction de rappel
}

void MqttManager::loop() {
	if (!mqttClient.connected()) {  // Vérification de la connexion au serveur MQTT
		connect();					// Connexion au serveur MQTT si déconnecté
	}
	if (!mqttClient.loop()) {		 // Gestion d'erreur si nécessaire
	}
}

void MqttManager::connect() {  		// connexion au serveur MQTT

	while (!mqttClient.connected()) {
		Serial.print("Attempting MQTT connection...");
		String mqttClientId = "";
		if (mqttClient.connect(mqttClientId.c_str())) {
			Serial.println("connected");
			mqttClient.subscribe(mqttTopicIn.c_str());
			Serial.print("subscribed to ");
			Serial.println(mqttTopicIn);
			senMessage("train/setup", "hello");

		} else {
			Serial.print("failed, rc=");
			Serial.print(mqttClient.state());
			Serial.println(" will try again in 5 seconds");
			Serial.print("remote ip");
			Serial.print(wifiClient.remoteIP().toString());
			Serial.print(":");
			Serial.println(wifiClient.remotePort());
			delay(100);
		}
	}
}

void MqttManager::senMessage(String topic, String Payload) {
	Serial.print(" MqttManager ->  Sending Message: ");
	Serial.print(topic);
	Serial.print(" Payload > ");
	Serial.println(Payload);
	mqttClient.publish(topic.c_str(), Payload.c_str()); // Publication du message sur le topic
}

void MqttManager::attachIOManager(IOManager *mngr) {
	ioManager = mngr;
	s_ioManager = mngr; // Mise à jour du pointeur statique
}
