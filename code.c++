#include <WiFi.h>
#include <DHTesp.h>
#include <PubSubClient.h>

// Configurações de WiFi
const char *SSID = "Wokwi-GUEST";
const char *PASSWORD = "";  // Substitua pelo sua senha

// Configurações de MQTT
const char *BROKER_MQTT = "46.17.108.113";
const int BROKER_PORT = 1883;
const char *ID_MQTT = "fiware_sensorAlert002";
const char *TOPIC_PUBLISH_TEMP = "/TEF/sensorAlert002/attrs/t";
const char *TOPIC_PUBLISH_UMID = "/TEF/sensorAlert002/attrs/h";

// Configurações de Hardware
#define PIN_DHT 12
#define PIN_BUZZER 13  // Substitua pelo pino ao qual o Buzzer está conectado
#define PUBLISH_DELAY 2000

// Variáveis globais
WiFiClient espClient;
PubSubClient MQTT(espClient);
DHTesp dht;
unsigned long publishUpdate = 0;
TempAndHumidity sensorValues;
const int TAMANHO = 200;

// Protótipos de funções
void updateSensorValues();
void initWiFi();
void initMQTT();
void reconnectMQTT();
void reconnectWiFi();
void checkWiFIAndMQTT();
void checkTemperatureThreshold(float temperature);

void updateSensorValues() {
  sensorValues = dht.getTempAndHumidity();
}

void initWiFi() {
  Serial.print("Conectando com a rede: ");
  Serial.println(SSID);
  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Conectado com sucesso: ");
  Serial.println(SSID);
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void initMQTT() {
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);
}

void reconnectMQTT() {
  while (!MQTT.connected()) {
    Serial.print("Tentando conectar com o Broker MQTT: ");
    Serial.println(BROKER_MQTT);

    if (MQTT.connect(ID_MQTT)) {
      Serial.println("Conectado ao broker MQTT!");
    } else {
      Serial.println("Falha na conexão com MQTT. Tentando novamente em 2 segundos.");
      delay(2000);
    }
  }
}

void checkWiFIAndMQTT() {
  if (WiFi.status() != WL_CONNECTED) reconnectWiFi();
  if (!MQTT.connected()) reconnectMQTT();
}

void reconnectWiFi(void) {
  if (WiFi.status() == WL_CONNECTED)
    return;

  WiFi.begin(SSID, PASSWORD); // Conecta na rede WI-FI

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Wifi conectado com sucesso");
  Serial.print(SSID);
  Serial.println("IP: ");
  Serial.println(WiFi.localIP());
}

void checkTemperatureThreshold(float temperature) {
  const float TEMPERATURE_THRESHOLD = 25.0;  // Limite de temperatura em graus Celsius

  if (temperature > TEMPERATURE_THRESHOLD) {
    tone(PIN_BUZZER, 1000);  // Ativa o Buzzer com uma frequência de 1000 Hz
  } else {
    noTone(PIN_BUZZER);   // Desativa o Buzzer
  }
}

void setup() {
  Serial.begin(115200);
  dht.setup(PIN_DHT, DHTesp::DHT22);
  pinMode(PIN_BUZZER, OUTPUT);  // Configura o pino do Buzzer como saída
  initWiFi();
  initMQTT();
}

void loop() {
  checkWiFIAndMQTT();
  MQTT.loop();
  if ((millis() - publishUpdate) >= PUBLISH_DELAY) {
    publishUpdate = millis();
    updateSensorValues();

    // temperatura
    char msgBuffer1[5];
    float sensorValue1 = sensorValues.temperature;
    Serial.print("Temperature: ");
    Serial.println(sensorValue1);
    dtostrf(sensorValue1, 4, 2, msgBuffer1);
    MQTT.publish(TOPIC_PUBLISH_TEMP, msgBuffer1);

    // umidade
    char msgBuffer2[5];
    float sensorValue2 = sensorValues.humidity;
    Serial.print("Humidity: ");
    Serial.println(sensorValue2);
    dtostrf(sensorValue2, 4, 2, msgBuffer2);
    MQTT.publish(TOPIC_PUBLISH_UMID, msgBuffer2);

    // Verifica o limite de temperatura e aciona o Buzzer se necessário
    checkTemperatureThreshold(sensorValue1);
  }
}
