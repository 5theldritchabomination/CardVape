#include <SPI.h>
#include <MFRC522.h>
#include <WiFi101.h>
#include <PubSubClient.h>

char ssid[] = "TP-Link_A05C";
char pass[] = "EpsiWorkShop123&";   //Connexion au Wi-Fi

const char* mqttServer = "192.168.0.150"; // IP du broker Mosquitto
int mqttPort = 1883;                 // port du broker de base
const char* mqttUser = "admin";      // si auth activée
const char* mqttPassword = "memel";  //MDP du broker

WiFiClient wifiClient;
PubSubClient client(wifiClient);

#define RST_PIN         6         // Configurable, see typical pin layout above
#define SS_PIN          7         // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

void setup() {
	Serial.begin(9600);		// Initialize serial communications with the PC
		// Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
	
  // --- Wi-fi ---
  Serial.print("Connexion au Wi-Fi...") ;
  int status = WiFi.begin(ssid, pass); //inititalise la variable de status du wifi
  while (status != WL_CONNECTED) //WL_CONNECTED est une variable de la fonction Wifi.status()
  {
    delay(500);
    Serial.print(".");
    status = WiFi.status(); //Récupère le status du wifi.
  }
  Serial.println("Wi-fi Connecté !");
  Serial.print("IP : "); Serial.println(WiFi.localIP());

  // --- MQTT ---
  client.setServer(mqttServer, mqttPort);
  client.setCallback(mqttCallback);

  reconnectMQTT();



  SPI.begin();			// Init SPI bus
	mfrc522.PCD_Init();		// Init MFRC522
	delay(4);				// Optional delay. Some board do need more time after init to be ready, see Readme
	mfrc522.PCD_DumpVersionToSerial();	// Show details of PCD - MFRC522 Card Reader details
	Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
}

String uidToString(MFRC522::Uid *uid) { //définition d'une fonction
	String s = ""; //Définition d'une variable
	for (byte i = 0; i < uid->size; i++) {
			if (uid->uidByte[i] < 0x10) s+= "0" ; //Ici l'on regarde si chaque uidByte uid (un octet) est inférieur à 0x10 (16 décimal) on met la valeur string '0' avant l'hexadécimal utilisé
			//cet hexadécimal (de 0x00 à 0x10 (16 octets) est pour déterminer 1 chiffre, d'où le fait que l'on ajoute un 0 juste avant afin de permettre que l'hex soit de deux caractère. Cela permet à ce que l'hexa, si sous 16, ne soit pas 0xa mais, 0x0A)
			s += String (uid->uidByte[i], HEX);

	}
	s.toUpperCase();
	return s;
}


void loop() {
    // put your main code here, to run repeatedly:
  if ( ! mfrc522.PICC_IsNewCardPresent()) //Vérifie si il y a une carte devant le lecteur (vient de la librairie MFRC522)
  {
    delay (50); //permet d'éviter de lire trop de fois la carte.
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    delay(50);
    return;
  }
  String uid = uidToString(&mfrc522.uid); // permet de récupérer l'uid de la carte (en hexa) dans la var uid (en string)
  Serial.print("UID:");
  Serial.println(uid);
	mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
 	mfrc522.PICC_DumpToSerial(&(mfrc522.uid)); // version du lecteur ainsi que toutes les données
  if ( uid == "E3B4C717") 
  {

  }
  if (!client.connected()) {
    reconnectMQTT();
  }
  client.loop();
  static unsigned long last = 0;
  client.publish("uid/topic", uid.c_str()); //normalement le code prend uniquement un char en pur, l'insertion de la variable uid normalement string est convertie en Char par c_str de la librairie standard de C.
}

  //=== Callback pour messages entrants ===
void mqttCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message reçu [");
  Serial.print(topic);
  Serial.print("] : ");
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

// === Fonction de reconnexion ===
void reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("Connexion au broker MQTT...");
    if (client.connect("MKR1000Client", mqttUser, mqttPassword)) {
      Serial.println("connecté !");
      client.subscribe("test/topic"); // souscription pour recevoir des messages
    } else {
      Serial.print("échec, rc=");
      Serial.print(client.state());
      Serial.println(" nouvelle tentative dans 5s");
      delay(5000);
    }
  }
}
