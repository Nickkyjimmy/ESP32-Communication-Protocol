#include <WiFi.h>
#include <config.h>

// WiFi credentials
// Server settings
#ifndef CONFIG_H
#define CONFIG_H

const char *ssid = "YOUR_SSID";
const char *password = "YOUR_PASSWORD";

#endif

const int port = 8080;
const int stopSignal = 20;
WiFiServer server(port);

void setup()
{
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi!");
  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());

  // Start the serverd
  server.begin();
}

void spamSeries(WiFiClient client)
{
  int counter = 0; // Example: Start with a counter
  int counterLim = 10;
  int cooldown = 250;
  int stopSignal = 20;

  // client.readStringUntil
  while (client.connected() && counter < counterLim)
  {
    int random1 = random(0, 101); // Random number between 0 and 100
    int random2 = random(0, 101);
    int random3 = random(0, 101);
    int random4 = random(0, 101);
    
    String dataToSend = String(random1) + "," + String(random2) + "," + String(random3) + "," + String(random4);
    client.print(dataToSend);

    Serial.printf("Sent: %s\n", dataToSend.c_str());

    counter++;
    delay(cooldown);
  }
  
  client.print(stopSignal);
  client.stop(); // Disconnect the client
  Serial.println("Client disconnected.");
}

void loop()
{
  WiFiClient client = server.available();

  if (client) {
    spamSeries(client);
  }
}
