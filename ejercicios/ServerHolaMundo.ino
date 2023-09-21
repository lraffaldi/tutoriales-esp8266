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
  String html = "<!DOCTYPE html>";
  html += "<html lang='es'>";
  html += "<head>";
  html += "<meta charset='UTF-8'>";
  html += "</head>";
  html += "<center>";
  html += "<h1>Tutoriales para EET 343</h1>";
  html += "<p>Bienvenido a nuestra colección de tutoriales para EET 343. Aquí encontrarás recursos y guías para tus estudios.</p>";
  html += "<p>¡Aprende, practica y domina el contenido!</p>";
  html += "</br>";
  html += "<p><h3>HOLA MUNDO</h3>";
  html += "</center>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}
