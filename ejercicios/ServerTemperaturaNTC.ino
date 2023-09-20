#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define SERIESRESISTOR 10000
#define NOMINAL_RESISTANCE 10000
#define NOMINAL_TEMPERATURE 25
#define BCOEFFICIENT 3435

const char *ssid = "CoWork";      // Cambia esto con el nombre de tu red WiFi
const char *password = "tre2022!";  // Cambia esto con la contraseña de tu red WiFi
long t = 0;
float temperatura = 0;

ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);
  while(!Serial) { }; // Espera la conexión con el serial
  
  // Conectar a la red WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Conexión WiFi establecida");

  // Rutas para la página web
  server.on("/", HTTP_GET, handleRoot);
  
  // Iniciar servidor web
  server.begin();
  Serial.println("Conectado con la IP: ");
  Serial.print(WiFi.localIP());
}

void loop() {
  server.handleClient();
  if ( millis() - t >= 10000) {
    t = millis();
    temperatura = readTemp();
  }
}

float readTemp(){
  float aux = 0.0;
  for (int i = 0; i < 100; i++){
    aux = aux + analogRead(A0);
    delay(1);
  }
  float adcValue = aux/100;

  double bits = 1023.0;
  double vin = 3.3;
  float adcRange = (bits/vin) * 3.3;

  //ecuación de Steinhart-Hart simplificada o (B Parameter Equation) https://www.blascarr.com/?p=2460
  float resistance = adcRange / adcValue - 1;
  resistance = SERIESRESISTOR / resistance;
  float steinhart = resistance / NOMINAL_RESISTANCE; // (R/Ro)
  steinhart = log(steinhart); // ln(R/Ro)
  steinhart /= BCOEFFICIENT; // 1/B * ln(R/Ro)
  steinhart += 1.0 / (NOMINAL_TEMPERATURE + 273.15); // + (1/To)
  steinhart = 1.0 / steinhart; // Invert
  steinhart -= 273.15; // convert to C
  Serial.println(steinhart);
  return steinhart;
}

void handleRoot() {
  String estado = "20°";
  
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
  html += "<p><h3><b>Temperatura:</b> " + String(temperatura) + "°C</p></h3>";
  html += "</center>";
  html += "</body></html>";
  
  server.send(200, "text/html", html);
}
