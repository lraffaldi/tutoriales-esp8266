#include <ESP8266WiFi.h> // Para poner ESP en Modo AP o Station
#include <ESP8266WebServer.h> // Para utilizar servidores asincronos

const char *ssid = "";      // Cambia esto con el nombre de tu red WiFi
const char *password = "";  // Cambia esto con la contraseña de tu red WiFi

ESP8266WebServer server(80); // Puerto 80 -> http (443 -> https)

void setup() {
  pinMode(D0,OUTPUT); 
  Serial.begin(115200);
  while(!Serial) { }; // Espera la conexión con el serial
 
  // Conectar a la red WiFi
  WiFi.begin(ssid, password); // Incluida en ESP8266WiFi.h
  while (WiFi.status() != WL_CONNECTED) { // WL_CONNECTED=3
    delay(100);
    Serial.print(".");
    Serial.print(" ");
  }
  Serial.println("");
  Serial.print("Conexión WiFi establecida con IP:");
  Serial.println(WiFi.localIP());

  // Rutas para la página web
  server.on("/", HTTP_GET, funcionBarra);
  
  // Iniciar servidor web
  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
  
}

void funcionBarra() {
  server.send(200, "text/html", "HOLA MUNDO");
}
