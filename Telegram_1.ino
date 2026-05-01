#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

// ── WiFi Credentials ──
const char* ssid     = "Manas";
const char* password = "Manas@2025";

// ── Telegram Bot ──
#define BOT_TOKEN "8746878156:AAE4rkMSyUwG65vAgz9XTzDmJgHAFsj_a6g"
#define CHAT_ID   "1796567647"

// ── LED Pins (4 LEDs) ──
#define LED1 2
#define LED2 4
#define LED3 5
#define LED4 18

WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);

unsigned long lastTime = 0;
const int delayTime = 1000;

// ── Handle Messages ──
void handleMessages(int numNewMessages) {
  for (int i = 0; i < numNewMessages; i++) {

    String chat_id = bot.messages[i].chat_id;
    String text    = bot.messages[i].text;
    text.toLowerCase();

    // Security check - silently ignore unauthorized users
    if (chat_id != CHAT_ID) {
      continue;
    }

    // ── LED 1 ──
    if (text == "/led1on") {
      digitalWrite(LED1, HIGH);
      bot.sendMessage(chat_id, "LED1 ON", "");
    }
    else if (text == "/led1off") {
      digitalWrite(LED1, LOW);
      bot.sendMessage(chat_id, "LED1 OFF", "");
    }

    // ── LED 2 ──
    else if (text == "/led2on") {
      digitalWrite(LED2, HIGH);
      bot.sendMessage(chat_id, "LED2 ON", "");
    }
    else if (text == "/led2off") {
      digitalWrite(LED2, LOW);
      bot.sendMessage(chat_id, "LED2 OFF", "");
    }

    // ── LED 3 ──
    else if (text == "/led3on") {
      digitalWrite(LED3, HIGH);
      bot.sendMessage(chat_id, "LED3 ON", "");
    }
    else if (text == "/led3off") {
      digitalWrite(LED3, LOW);
      bot.sendMessage(chat_id, "LED3 OFF", "");
    }

    // ── LED 4 ──
    else if (text == "/led4on") {
      digitalWrite(LED4, HIGH);
      bot.sendMessage(chat_id, "LED4 ON", "");
    }
    else if (text == "/led4off") {
      digitalWrite(LED4, LOW);
      bot.sendMessage(chat_id, "LED4 OFF", "");
    }

    // ── All ON / All OFF ──
    else if (text == "/allon") {
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, HIGH);
      digitalWrite(LED3, HIGH);
      digitalWrite(LED4, HIGH);
      bot.sendMessage(chat_id, "All LEDs ON", "");
    }
    else if (text == "/alloff") {
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, LOW);
      digitalWrite(LED4, LOW);
      bot.sendMessage(chat_id, "All LEDs OFF", "");
    }
    // Unknown commands -> no reply (silent)
  }
}

void setup() {
  Serial.begin(115200);

  // Setup all LED pins
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);

  // All OFF initially
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);

  // Connect WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected!");
  Serial.println(WiFi.localIP());

  client.setInsecure(); // required for Telegram

  // Send "ESP32 is Online" only once when both WiFi + Telegram connected
  if (bot.sendMessage(CHAT_ID, "✅ ESP32 is Online", "")) {
    Serial.println("Telegram Connected - Online message sent");
  }
}

void loop() {
  if (millis() - lastTime > delayTime) {
    int newMessages = bot.getUpdates(bot.last_message_received + 1);

    while (newMessages) {
      handleMessages(newMessages);
      newMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    lastTime = millis();
  }
}
