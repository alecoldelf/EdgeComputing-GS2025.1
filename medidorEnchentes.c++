#include <WiFi.h>
#include <HTTPClient.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Wi-Fi
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// TagoIO
const char* TAGO_TOKEN = "831b14ed-c8a0-443a-b084-89c277334a1a";
const char* TAGO_ENDPOINT = "https://api.tago.io/data";

// MQTT (Mosquitto)
const char* mqtt_server = "test.mosquitto.org";  // ou seu IP local, ex: "192.168.1.100"
const int mqtt_port = 1883;
const char* mqtt_topic = "esp32/sensores";  // Tópico para os dados

// Objetos
WiFiClient espClient;
PubSubClient mqttClient(espClient);
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pinos
#define DHTPIN 4
#define DHTTYPE DHT22
#define TRIG_PIN 5
#define ECHO_PIN 18

DHT dht(DHTPIN, DHTTYPE);

// Conectar Wi-Fi
void setup_wifi() {
  Serial.print("Conectando ao Wi-Fi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi conectado com sucesso");
}

// Reconnectar ao MQTT
void reconnect_mqtt() {
  while (!mqttClient.connected()) {
    Serial.print("Conectando ao MQTT...");
    if (mqttClient.connect("ESP32Client")) {
      Serial.println("Conectado!");
    } else {
      Serial.print("Falha, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" tentando novamente em 2 segundos");
      delay(2000);
    }
  }
}

// Ler distância
float readUltrasonicDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH);
  return duration * 0.0343 / 2;
}

void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  dht.begin();
  setup_wifi();

  mqttClient.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!mqttClient.connected()) {
    reconnect_mqtt();
  }
  mqttClient.loop();

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  float distance = readUltrasonicDistance();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Erro lendo sensor DHT22");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Erro no DHT22");
    delay(2000);
    return;
  }

  // LCD e Serial
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.printf("T:%.1fC H:%.1f%%", temperature, humidity);
  lcd.setCursor(0, 1);
  lcd.printf("Dist:%.1f cm", distance);

  Serial.print("Temp: ");
  Serial.print(temperature);
  Serial.print("°C, Hum: ");
  Serial.print(humidity);
  Serial.print("%, Dist: ");
  Serial.print(distance);
  Serial.println(" cm");

  // --- Envio via HTTP para TagoIO ---
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(TAGO_ENDPOINT);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Device-Token", TAGO_TOKEN);

    String payload = "[";
    payload += "{\"variable\":\"temperatura\",\"value\":" + String(temperature) + "},";
    payload += "{\"variable\":\"umidade\",\"value\":" + String(humidity) + "},";
    payload += "{\"variable\":\"distancia\",\"value\":" + String(distance) + "}";
    payload += "]";

    int httpCode = http.POST(payload);
    Serial.print("HTTP TagoIO: ");
    Serial.println(httpCode);

    if (httpCode > 0) {
      Serial.println(http.getString());
    }

    http.end();
  }

  // --- Envio via MQTT ---
  char mqtt_payload[128];
  snprintf(mqtt_payload, sizeof(mqtt_payload),
           "{\"temperatura\":%.1f,\"umidade\":%.1f,\"distancia\":%.1f}",
           temperature, humidity, distance);
  mqttClient.publish(mqtt_topic, mqtt_payload);

  delay(10000);  // Espera 10s
}
