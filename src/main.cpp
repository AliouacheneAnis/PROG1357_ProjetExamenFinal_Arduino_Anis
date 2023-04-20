/*
  Titre      : PROG1357 Projet Final
  Auteur     : Anis Aliouachene
  Date       : 20/04/2023
  Description: Une description du programme
  Version    : 0.0.1
*/

// les libraires 
#include <Arduino.h>
#include <WiFi.h>
#include <DNSServer.h>
#include "ESPAsyncWebServer.h"
#include "page.hpp"    // Page Web 
#include <HTTPClient.h>

DNSServer dnsServer; // creer un serveur DNS
AsyncWebServer server(80); // creer un serveur 
HTTPClient http;

// Variables 
String Ssid, Password, ServerIp, Port, ServerUrl;
bool ServerIpReceived = false, ServerPortReceived = false;
int PotValue = 0; // variable for storing the potentiometer value

// Constantes
const char* ID = "MyESP32_Server";  // ID Hotspot
const char* PASS = "anis1234";  // Password de Hotspot 
const char* PARAM_INPUT_1 = "Ssid";
const char* PARAM_INPUT_2 = "Password";
const char* PARAM_INPUT_3 = "ServerIp";
const char* PARAM_INPUT_4 = "Port";
const int PotPin = 32; // Potentiometer connecte broche 32

// Declaration Function de configuration de serveur et recupuration des donnes pour se connecter au wifi et serveur Java
void setupServer();

// Class pour gérer la requête d'envoi de données 
class CaptiveRequestHandler : public AsyncWebHandler {
public:
  CaptiveRequestHandler() {} // Constructeur par défaut
  virtual ~CaptiveRequestHandler() {}  // Destructeur virtuel
  
  // Fonction pour déterminer si cette class peut gérer la requête
  bool canHandle(AsyncWebServerRequest *request){

    // Toujours retourner vrai, car cette class gère toutes les requêtes
    return true; 
  }
  
  // Function pour traiter la requête
  void handleRequest(AsyncWebServerRequest *request) {

    // Envoyer une réponse HTTP avec le code de statut 200 (OK), le type de contenu "text/html" et le contenu de la variable index_html
    request->send_P(200, "text/html", index_html); 
  }
};

void setup(){
  
  Serial.begin(9600);

  // Lancement Hotspot Point d'Access
  Serial.println("Setting up AP Mode");
  WiFi.softAP(ID, PASS);

  // Lancement DNS afin de redirecter l'utilisateur vers une page web pour saisir les informations automatiquement une fois connecter 
  Serial.println("Starting DNS Server");
  dnsServer.start(53, "*", WiFi.softAPIP());

  // Appeler la fonction pour configurer le serveur 
  Serial.println("Setting up Async WebServer");
  setupServer(); 
  delay(3000);

  // Ajouter un gestionnaire de requêtes de type CaptiveRequestHandler au serveur uniquement lorsque la requête provient du point d'accès Wi-Fi
  server.addHandler(new CaptiveRequestHandler());

  // Démarrer le serveur web
  server.begin();
}

void loop(){

  // Serveur DNS pour proceder aux request 
  dnsServer.processNextRequest();
  
  // Recuperer la valeur de capteur puis utiliser la fonction map pour simuler le niveau
  PotValue = analogRead(PotPin);
  int WaterLevel = map(PotValue, 0, 4095, 0, 10); // Map function to get a variable between 0 and 10 
  Serial.println(WaterLevel);
  Serial.println(PotValue);
  
  // Debut et lancement HTTP
  http.begin(ServerUrl.c_str());

  // Construction string de request JSON avec le niveau d'eau pour l'Envoyer avec un HTTP POST 
  http.addHeader("Content-Type", "application/json"); 
  String requestBody = "{\"data\": " + String(WaterLevel) + "}"; 
  http.POST(requestBody);
  Serial.println(requestBody);
  http.end();

  delay(3000); // attente 3 sencondes pour capter la donnees a nouveau 
}

// Implementation Function
void setupServer(){
  
  // Afficher la page d'accueil index.html pour l'utilisateur afin qu'il saisie les informations 
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send_P(200, "text/html", index_html); 
      Serial.println("Client Connected");
  });
  
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
      
      // Recuperer les parametres (Informations) saise a partir de la page 
      if (request->hasParam(PARAM_INPUT_1)) {
        Ssid = request->getParam(PARAM_INPUT_1)->value();
      }

      if (request->hasParam(PARAM_INPUT_2)) {
        Password = request->getParam(PARAM_INPUT_2)->value();
      }
    
       if (request->hasParam(PARAM_INPUT_3)) {
        ServerIp = request->getParam(PARAM_INPUT_3)->value();
        ServerIpReceived = true; 
      }

       if (request->hasParam(PARAM_INPUT_4)) {
        Port = request->getParam(PARAM_INPUT_4)->value();
        ServerPortReceived = true; 
      }

    Serial.println(Ssid); 
    Serial.println(Password);
    Serial.println(ServerIp); 
    Serial.println(Port); 

    // Concatener un string qui le lien vers le serveur Java selon le Ip de serveur et le port 
    ServerUrl = "http://" + ServerIp + ":" + Port + "/data";
    Serial.println(ServerUrl);

    // Connecter au wifi specifique selon les donnees saisie par l'utilsateur 
    WiFi.begin(Ssid.c_str(), Password.c_str());
    Serial.print("Connecting to ");
    Serial.println(Ssid);
    while (WiFi.status() != WL_CONNECTED) {
         delay(1000);
      Serial.println("...");
    }
    Serial.print("Connected to ");
    Serial.println(Ssid);
    
    // retourner une page html de success une fois les donnees envoyer de la page web vers l'ESP32 pour se conencter au serveur java 
    request->send(200, "text/html", "The values entered by you have been successfully sent to the device <br><a href=\"/\">Return to Home Page</a>");
  }); 
}
