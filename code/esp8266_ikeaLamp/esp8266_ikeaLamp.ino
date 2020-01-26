/*
  PILO
  PILO is a project based on a set of soft interfaces,
  These soft interfaces are connected to a main (core) board based on a ESP8266 and a breakout board.
  The core board is powered by a powerbank.
  Every time a soft button in pressed, an MQTT message is sent to a specific topic. 
  MQTT server in this case is a mosquitto in a local network.
  Then there's NodeRed with a flow used to create the interactions between objects.

  This is a simple code to control an IKEA LED lamp with a P2D node (https://github.com/FabLabCastelfrancoVeneto/P2D)
  
  CC BY-SA
*/

#include <Servo.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

Servo p2dServo;

const char* ssid = "your wifi";
const char* password = "your password";
const char* mqtt_server = "192.168.0.16";

const char* node_name = "PILOlamp";
const char* action1 = "Turn_off";
const char* action2 = "Turn_on";
//const char* action1 = "Accendi";
//const char* action2 = "Spegni";
const char* action3 = "";

int node_status = 0;
const char* node_status0 = "";
const char* node_status1 = "";

unsigned long lastMsg = 0;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  p2dServo.attach(2);
  p2dServo.write(90);

  pinMode(BUILTIN_LED, OUTPUT); 
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}
void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

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

  if (action1 != "" && msg_received == action1) {
    Serial.println("Action 1");
    digitalWrite(BUILTIN_LED, LOW);
    p2dServo.write(160);
    delay(2000);
    p2dServo.write(90);
    node_status = 1;
  }
  if (action2 != "" && msg_received == action2) {
    Serial.println("Action 2");
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
    p2dServo.write(15);
    delay(2000);
    p2dServo.write(90);
    node_status = 0;
  }
  if (action3 != "" && msg_received == action3) {
    Serial.println("Action 3");
    client.publish(node_name, node_status == 0 ? node_status0 : node_status1);
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(node_name)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //client.publish("outTopic", "hello world");
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

 //every minute send the node name to say: hi i'm here
    long now = millis();
    if (now - lastMsg > 60000) {
      lastMsg = now;
      /*
      ++value;
      snprintf (msg, 75, "hello world #%ld", value);
      Serial.print("Publish message: ");
      Serial.println(msg);
      */
      client.publish("node_alive", node_name);
    }
  
}
