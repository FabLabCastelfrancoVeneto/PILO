/*
  PILO
  PILO is a project based on a set of soft interfaces,
  These soft interfaces are connected to a main (core) board based on a ESP8266 and a breakout board.
  The core board is powered by a powerbank.
  Every time a soft button in pressed, an MQTT message is sent to a specific topic. 
  MQTT server in this case is a mosquitto in a local network.
  Then there's NodeRed with a flow used to create the interactions between objects.

  This is a simple code to control a neopixel node (done with an ESP8266 and a neopixel ring) 
  by sending to it the name of the color.
  
  CC BY-SA
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_NeoPixel.h>

#define PIN            D4
#define NUMPIXELS      16

const char* ssid = "your_ssid";
const char* pass = "your_password";
const char* mqtt_server = "192.168.0.15";
const char* node_name = "rgbwspot01";
const char* topic_name = "/rgbwspot/01";

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRBW + NEO_KHZ800);

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long previousMillis = 0;
const long check_interval = 60000;

void setup() {
  Serial.begin(115200);
  delay(10);

  setup_wifi();

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  pixels.begin();
}

void setup_wifi() {

  delay(10);

  //clean up AP mode and force STA mode
  //https://stackoverflow.com/questions/39688410/how-to-switch-to-normal-wifi-mode-to-access-point-mode-esp8266
  WiFi.softAPdisconnect();
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  delay(100);

  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  String msg_received = "";
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    msg_received += (char)payload[i];
  }

  if (msg_received == "rosso") {
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(255, 0, 0, 0));
      pixels.show();
      delay(50);
    }
  }

  if (msg_received == "verde") {
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 255, 0, 0));
      pixels.show();
      delay(50);
    }
  }

  if (msg_received == "blue") {
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 255, 0));
      pixels.show();
      delay(50);
    }
  }

  if (msg_received == "fucsia") {
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(200, 0, 200, 0));
      pixels.show();
      delay(50);
    }
  }

  if (msg_received == "bianco") {
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0, 255));
      pixels.show();
      delay(50);
    }
  }

  if (msg_received == "spenta") {
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0, 0));
      pixels.show();
      delay(50);
    }
  }

  if (msg_received == "misto") {
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(i * 16, 0, 255 - i * 16, 0));
      pixels.show();
      delay(50);
    }
  }
  Serial.println();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(node_name)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //client.publish(topic_name, "-1");
      // ... and resubscribe
      client.subscribe(topic_name);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= check_interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    //send an ALIVE message
    client.publish("alive", node_name);
  }
}
