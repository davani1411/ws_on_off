/*********
  Rui Santos
  Complete instructions at https://RandomNerdTutorials.com/build-web-servers-ebook/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*********/

#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"

// Replace with your network credentials
const char *ssid = 
const char *password =

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Create a WebSocket object
AsyncWebSocket ws("/ws");

// Set LED GPIO
const int ledPin = 15;
const int ledPin27 = 27;
const int ledPin26 = 26;
const int ledPin25 = 25;
const int ledPin33 = 33;
// Stores LED state
bool ledState = 0;

// Initialize SPIFFS
void initSPIFFS() {
  if (!SPIFFS.begin(true)) {
    Serial.println("An error has occurred while mounting SPIFFS");
  }
  Serial.println("SPIFFS mounted successfully");
}

// Initialize WiFi
void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

// Replaces placeholder with LED state value
String processor(const String& var) {
  if(var == "STATE") {
    if(digitalRead(ledPin)) {
      ledState = 1;
      return "forward";
    }
    else {
      ledState = 0;
      return "backward";
    }
  }
 return String();

   if(var == "STATE2") {
    if(digitalRead(ledPin)) {
      ledState = 1;
      return "on";
    }
    else {
      ledState = 0;
      return "off";
    }
  }
    return String();
}

void notifyClients2(String state) {
  ws.textAll(state);
}
void notifyClients(String state2) {
  ws.textAll(state2);
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    if (strcmp((char*)data, "bON") == 0) {
      ledState = 1;
      digitalWrite(ledPin27,  HIGH);
      digitalWrite(ledPin26,  LOW);
      digitalWrite(ledPin25,  HIGH);
      digitalWrite(ledPin33,  LOW);
      notifyClients2("forward");
    }
    if (strcmp((char*)data, "bOFF") == 0) {
      ledState = 0;
      digitalWrite(ledPin27, LOW);
      digitalWrite(ledPin26,  HIGH);
      digitalWrite(ledPin25,  LOW);
      digitalWrite(ledPin33,  HIGH);

      notifyClients2("backward");
    }
    if (strcmp((char*)data, "b2ON") == 0) {
      ledState = 1;
      digitalWrite(ledPin27,  HIGH);
      digitalWrite(ledPin26,  HIGH);
      digitalWrite(ledPin25,  HIGH);
      digitalWrite(ledPin33,  HIGH);
      notifyClients("ON");
    }
    if (strcmp((char*)data, "b2OFF") == 0) {
      ledState = 0;
      digitalWrite(ledPin27, LOW);
      digitalWrite(ledPin26,  LOW);
      digitalWrite(ledPin25,  LOW);
      digitalWrite(ledPin33,  LOW);

      notifyClients("OFF");
    }
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

void setup() {
  pinMode(ledPin, OUTPUT);
   pinMode(ledPin27, OUTPUT);
    pinMode(ledPin26, OUTPUT);
     pinMode(ledPin25, OUTPUT);
      pinMode(ledPin33, OUTPUT);


  Serial.begin(115200);
  initSPIFFS();
  initWiFi();
  initWebSocket();

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/index.html", "text/html",false, processor);
  });

  server.serveStatic("/", SPIFFS, "/");

  // Start server
  server.begin();
}

void loop() {
  ws.cleanupClients();
  digitalWrite(ledPin, ledState);
}
