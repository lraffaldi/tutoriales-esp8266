#include <ESP8266WiFi.h> // Para poner ESP en Modo AP o Station
#include <ESP8266WebServer.h> // Para utilizar servidores asincronos

const char *ssid = "";      // Cambia esto con el nombre de tu red WiFi
const char *password = "";  // Cambia esto con la contraseña de tu red WiFi

ESP8266WebServer server(80); // Puerto 80 -> http (443 -> https)

bool estadoLed = false;
void setup() {
  pinMode(D0,OUTPUT); 
  Serial.begin(115200);
  while(!Serial) { }; // Espera la conexión con el serial
 
  // Conectar a la red WiFi
  WiFi.begin(ssid, password); // Incluida en ESP8266WiFi.h
  while (WiFi.status() != WL_CONNECTED) { // WL_CONNECTED=3
    delay(100);
    Serial.print(WiFi.status());
    Serial.print(" ");
  }
  Serial.println("");
  Serial.print("Conexión WiFi establecida con IP:");
  Serial.println(WiFi.localIP());

  // Rutas para la página web
  server.on("/", HTTP_GET, funcionBarra);
  server.on("/led", HTTP_GET, funcionLed);
  
  // Iniciar servidor web
  server.begin();
}

void loop() {
  server.handleClient(); // Verifica que conexiones entrantes y procesa la solicitud (no queda bloqueado esperando clientes)
}

void funcionLed() {
  String e = server.arg("estado");
  if(e.toUpperCase() == "ON"){
    onLed();
  }
  if(e == "off"){
    offLed();
  }

  // Redirecciono nuevamente a la pagina de inicio
  // El cogigo HTTP 301 indica que el recurso solicitado ha sido movido de manera permanente a una nueva ubicación
  server.sendHeader("Location", "http://"+WiFi.localIP(), true);
  server.send(301, "text/plain", "");
}

void funcionBarra() {
  Serial.println("ENTRA");
  String html = "<!DOCTYPE html>";
  html += "<html lang='es'>";
  html += "<head>";
  html += "<meta charset='UTF-8'>";
  html += "</head>";
  html += "<body>";
    html += "<center>";
      html += "<h1>Tutoriales para EET 343</h1>";
      html += "<p>Bienvenido a nuestra colección de tutoriales para EET 343. Aquí encontrarás recursos y guías para tus estudios.</p>";
      html += "<p>¡Aprende, practica y domina el contenido!</p>";
      html += "</br>";
      html += "<p><h3><b>Estado Led:</b> "
      if(estadoLed){
         html += "Encendido";
      }else{
         html += "Apagado";
      }
      html += "</p></h3>";
      html += "<p><h3>Acciones Led:</h3>";
        html += "<a href='http://"+ WiFi.localIP() +"/led?estado=on'><button>ON LED</button></a>" 
        html += "<a href='http://"+ WiFi.localIP() +"/led?estado=off'><button>OFF LED</button></a>" 
      html += "</p>";
    html += "</center>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void onLed(){
  estadoLed=true;
  digitalWrite(D0,estadoLed);
}

void offLed(){
  estadoLed=false;
  digitalWrite(D0,estadoLed);
}
