/*
  PILO
  PILO is a project based on a set of soft interfaces,
  These soft interfaces are connected to a main (core) board based on a ESP8266 and a breakout board.
  The core board is powered by a powerbank.
  Every time a soft button in pressed, an MQTT message is sent to a specific topic. 
  MQTT server in this case is a mosquitto in a local network.
  Then there's NodeRed with a flow used to create the interactions between objects.

  This is a simple code running on the DIY teddy bear.
  Basically every soft pocket is a switch, every switch is connected to an ESP8266 board.
  Every time one of these switch is pressed, the board send an mqtt message.
  Then NodeRed process the request and redirect the message to an actuator node (ie a rgb spot, a relay board, etc etc)
  
  CC BY-SA
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char *ssid = "your_ssid";      // change according to your Network - cannot be longer than 32 characters!
const char *pass = "your_password";     // change according to your Network
const char* mqtt_server = "192.168.0.15";
const char* node_name = "PILOcuscino";

WiFiClient espClient;
PubSubClient client(espClient);

int device_type = 0; //0 = door, 1 = machine, 2 = other

unsigned long previousMillis = 0;        // will store last time LED was updated
const long check_interval = 60000;
char msg[50];

const int pins[] = { D2, D3, D4 };

#define BUTTONS 3

// current state of the pins
int pinState[BUTTONS];

// previous state of the pins
int lastPinState[BUTTONS];

void setup() {
  Serial.begin(115200);
  delay(10);

  for (int i = 0; i < BUTTONS; i++) {
    pinMode(pins[i], INPUT_PULLUP);
    digitalWrite(pins[i], HIGH);
    pinState[i] = HIGH;
    lastPinState[i] = HIGH;
  }

  setup_wifi();

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
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
      client.publish("/teddybear", "-1");
      // ... and resubscribe
      client.subscribe(node_name);
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
  String msgS = "";

  for (int i = 0; i < BUTTONS; i++) {
    pinState[i] = digitalRead(pins[i]);

    if (pinState[i] != lastPinState[i]) {
      if (pinState[i] == LOW) {
        Serial.println("hit!");
        msgS = String(i) + " " ;

        msgS.toCharArray(msg, msgS.length());
        client.publish("/teddybear", msg);

        delay(500);
        msgS = "";
      }
      delay(50);
      lastPinState[i] = pinState[i];
    }
  }

  if (currentMillis - previousMillis >= check_interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    //send an ALIVE message
    client.publish("alive", node_name);
  }
}
