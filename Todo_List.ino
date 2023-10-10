#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include "index.h"

const char *ssid = "SSID"; // SSID of Wi-Fi Network
const char *password = "PASSWORD"; // Password of Wi-Fi Network

ESP8266WebServer server(80); // Start web server from port 80
WebSocketsServer webSocket(81);

String eksiklerListesi = "";


void setup() {
  Serial.begin(115200);
  
  // Connect Wi-Fi Network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }
  Serial.println("Connected!");
  Serial.print("IP Adress: ");
  Serial.println(WiFi.localIP());

  // Start Web Server
  server.on("/", HTTP_GET, handle_OnConnect);
  server.on("/getEksikler", HTTP_GET, handle_GetEksikler);
  server.begin();

  // Start WebSocket Server
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop() {
  webSocket.loop();
  server.handleClient();
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
  if (type == WStype_TEXT) {
    String message = String((char *)payload);
    if (message.startsWith("remove:")) {
      // If message starts with "remove:", delete it
      String itemToRemove = message.substring(7); // take after "remove:"
      eksiklerListesi.replace("<li>" + itemToRemove + "</li>", ""); // Remove item
    } else {
      // Add new item in different conditions
      eksiklerListesi += "<li>" + message + "</li>";
    }
    webSocket.broadcastTXT(eksiklerListesi);
  }
}


void handle_OnConnect() {
  String s = webpage;
  server.send(200, "text/html", s);
}

void handle_GetEksikler() {
  server.send(200, "text/plain", eksiklerListesi);
}