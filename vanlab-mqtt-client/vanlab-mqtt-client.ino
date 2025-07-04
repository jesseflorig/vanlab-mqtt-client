// Author: Jesse Florig
// License: MIT
// Board: Adafruit Feather RP2040, Silicognition PoE HAT
// Description: Vanlab MQTT Client

#define DEBUG 1  // Set to 0 for production

// Debug macros
#if DEBUG
  #define DEBUG_BEGIN(baud)    Serial.begin(baud); while (!Serial && millis() < 2000)
  #define DEBUG_PRINT(x)       Serial.print(x)
  #define DEBUG_PRINTLN(x)     Serial.println(x)
  #define DEBUG_PRINTF(...)    Serial.printf(__VA_ARGS__)
#else
  #define DEBUG_BEGIN(baud)
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTLN(x)
  #define DEBUG_PRINTF(...)
#endif

// Unset to supress redefined warnings
#ifdef htons
  #undef htons
#endif

#include <SPI.h>
#include <Ethernet.h>
#include <MQTT.h>

// Network config
byte mac[] = { 0x02, 0xFE, 0xD3, 0xAD, 0xBE, 0xEF };
IPAddress ip(10, 1, 1, 20);
IPAddress subnet(255, 255, 255, 0);

// MQTT config
IPAddress broker(10, 1, 1, 10);
const int mqttPort = 1883;
const char* mqttUser = "pico";
const char* mqttPass = "password";
const char* mqttClientID = "feather-pico";

// Ethernet and MQTT clients
EthernetClient net;
MQTTClient client(128);  // 128-byte message buffer

// PUB MAP
struct PubTopic {
  const char* name;
  const char* topic;
};
PubTopic pubTopics[] = {
  { "status", "fleet1/pico/status" },  
};
const int pubCount = sizeof(pubTopics) / sizeof(pubTopics[0]);

// SUB MAP
typedef void (*MqttHandler)(String payload);
struct SubTopic {
  const char* name;
  const char* topic;
  MqttHandler handler;
};

// SUB HANDLERS
void handleTest(String payload){
  DEBUG_PRINTF("Handling TEST: %s\n", payload.c_str());
};

SubTopic subTopics[] = {
  { "test", "fleet1/pico/test", handleTest },
};
const int subCount = sizeof(subTopics) / sizeof(subTopics[0]);

// Return local IP as a string
const char* getLocalIP() {
  static char ipStr[16];  // Enough for "255.255.255.255"
  IPAddress ip = Ethernet.localIP();
  snprintf(ipStr, sizeof(ipStr), "%u.%u.%u.%u", ip[0], ip[1], ip[2], ip[3]);
  return ipStr;
};

// Initialize ethernet connection
void connectToEthernet(){
  DEBUG_BEGIN(115200);
  Ethernet.begin(mac, ip, subnet);
  DEBUG_PRINTF("IP Address: %s\n", getLocalIP());
};

// Publish dispatcher
void publishTo(const char* name, const char* message, bool retained = true, int qos = 1){
  for (int i = 0; i < pubCount; i++) {
    if ( strcmp(name, pubTopics[i].name) == 0){
      DEBUG_PRINTF("Publishing to %s: %s\n", pubTopics[i].topic, message);
      client.publish(pubTopics[i].topic, message, retained, qos);
      return;
    };
  };
  DEBUG_PRINTF("WARNING - Unknown pub topic name: %s\n", name);
};

// MQTT callback with dispatch
void onMqttMessage(String &topic, String &payload){
  for (int i = 0; i < subCount; i++) {
    if ( topic == subTopics[i].topic) {
      subTopics[i].handler(payload);
      return;
    };
  };
  DEBUG_PRINTF("WARNING - No handler for topic: %s\n", topic.c_str());
};

// Connect to broker and publish online message
void connectToMQTT() {
  // Client settings
  client.begin(broker, mqttPort, net);
  client.onMessage(onMqttMessage);
  client.setWill(pubTopics[0].topic, "offline", true, 1);
  
  DEBUG_PRINT("Connecting to MQTT...");
  while (!client.connect(mqttClientID, mqttUser, mqttPass)) {
    DEBUG_PRINT(".");
    delay(1000);
  };
  DEBUG_PRINTLN("connected!");

  // Publish initial state
  publishTo("status", "online");

  // Subscribe to all sub topics
  for (int i = 0; i < subCount; i++) {
    client.subscribe(subTopics[i].topic);
    DEBUG_PRINTF("Subscribed to: %s\n", String(subTopics[i].topic).c_str());
  };
};

// Publish a heartbeat status every 10 seconds
void heartbeat(){
  static unsigned long lastPublish = 0;
  if (millis() - lastPublish > 10000) {
    lastPublish = millis();
    publishTo("status", "heartbeat");
  };
};

void setup() {
  connectToEthernet();
  connectToMQTT();
};

void loop() {
   // Reconnect if dropped
  if (!client.connected()) {
    connectToMQTT();
  };
  client.loop();
  
  heartbeat();
  delay(10);
};
