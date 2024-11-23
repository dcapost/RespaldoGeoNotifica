//Libreria GPS
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
//Libreria Internet
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
//Libreria WhatsApp
#include <WiFiClient.h>
#include <UrlEncode.h>  // Librería para codificar el mensaje en la URL

// Configuración de GPS
static const int tx_d5 = 14, rx_d6 = 12; // Pines de TX y RX del GPS
TinyGPSPlus gps;
SoftwareSerial ss(tx_d5, rx_d6); // Comunicación serial con el GPS

// Configuración de WiFi
const char* ssid = "PS.Red";           // Nombre de tu red WiFi
const char* password = "123456789.P";  // Contraseña de tu red WiFi

//Configuración CallMeBot
String phoneNumber = "+56956005098";   // Número de teléfono con prefijo internacional
String apiKey = "7406250";             // API Key de CallMeBot

void sendMessage(String message) {
  // Data to send with HTTP POST
  String url = "http://api.callmebot.com/whatsapp.php?phone=" + phoneNumber + "&apikey=" + apiKey + "&text=" + urlEncode(message);
  WiFiClient client;    
  HTTPClient http;
  http.begin(client, url);

  // Specify content-type header
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  
  // Send HTTP POST request
  int httpResponseCode = http.POST(url);
  if (httpResponseCode == 200){
    Serial.print("Message sent successfully");
  }
  else{
    Serial.println("Error sending the message");
    Serial.print("HTTP response code: ");
    Serial.println(httpResponseCode);
  }

  // Free resources
  http.end();
}

void setup() {
  // Inicialización de seriales
  Serial.begin(9600);
  ss.begin(9600);

  // Conexión a WiFi
  Serial.println("Conectando a WiFi...");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConexión WiFi establecida.");
}

void loop() {

  while (ss.available() > 0) {
    // Leer datos del GPS y procesarlos
    byte gpsData = ss.read();
    gps.encode(gpsData);

    if (gps.location.isUpdated()) {
      // Obtener latitud y longitud
      double lat = gps.location.lat();
      double lng = gps.location.lng();

      // Mostrar en el Monitor Serie
      Serial.print("Latitud: ");
      Serial.print(lat, 6);
      Serial.print(", Longitud: ");
      Serial.println(lng, 6);
      Serial.println();

      String message = "Ubicación actual: " + String("Latitud: ") + String(lat, 6) + String(" Longitud: ") + String(lng, 6);
      
      //Enviar el mensaje
      sendMessage(message);
      //Espera
      delay(60000);
    }
  }
}


