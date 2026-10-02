#include <SPI.h>
#include <MFRC522.h>
#include <ESP32Servo.h>

#define SS_PIN      5     // RFID SS
#define RST_PIN     22    // RFID RST

#define TRIG_PIN    13    // Ultrasonic sensor TRIG
#define ECHO_PIN    12    // Ultrasonic sensor ECHO
#define SERVO_PIN   14    // Servo motor

#define BUZZER_PIN  25    // Buzzer
#define LED_PIN     26    // Status LED

MFRC522 mfrc522(SS_PIN, RST_PIN);
Servo gateServo;

// Update this with your actual authorized UID from Serial Monitor
byte authorizedUID[] = {0xC3, 0x87, 0x3F, 0x02};

void setup() {
    Serial.begin(115200);
    SPI.begin();
    mfrc522.PCD_Init();

    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);

    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(LED_PIN, LOW);

    gateServo.attach(SERVO_PIN);
    gateServo.write(0);  // Gate closed

    Serial.println("Scan RFID to open gate...");
}

void loop() {
    if (!mfrc522.PICC_IsNewCardPresent() ||
        !mfrc522.PICC_ReadCardSerial()) {
        return;
    }

    Serial.print("Card UID: ");
    bool authorized = true;

    // Print and compare the scanned UID
    for (byte i = 0; i < mfrc522.uid.size; i++) {
        Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
        Serial.print(mfrc522.uid.uidByte[i], HEX);

        if (i >= sizeof(authorizedUID) ||
            mfrc522.uid.uidByte[i] != authorizedUID[i]) {
            authorized = false;
        }
    }
    Serial.println();

    // Ensure UID length also matches
    if (mfrc522.uid.size != sizeof(authorizedUID)) {
        authorized = false;
    }

    if (authorized) {
        Serial.println("Access Granted! Opening Gate...");

        // Green/status LED ON and short confirmation beep
        digitalWrite(LED_PIN, HIGH);
        digitalWrite(BUZZER_PIN, HIGH);
        delay(200);
        digitalWrite(BUZZER_PIN, LOW);

        // Open gate
        gateServo.write(90);
        delay(5000);

        Serial.println("Vehicle passage detection active...");

        // Wait while vehicle is detected
        while (getDistance() < 10) {
            delay(500);
        }

        Serial.println("Closing Gate...");

        // Close gate
        gateServo.write(0);

        // Turn status LED OFF
        digitalWrite(LED_PIN, LOW);

    } else {
        Serial.println("Access Denied!");

        // LED + buzzer alert for unauthorized card
        digitalWrite(LED_PIN, HIGH);

        for (int i = 0; i < 3; i++) {
            digitalWrite(BUZZER_PIN, HIGH);
            delay(150);
            digitalWrite(BUZZER_PIN, LOW);
            delay(150);
        }

        digitalWrite(LED_PIN, LOW);
    }

    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
}

long getDistance() {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);

    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);

    digitalWrite(TRIG_PIN, LOW);

    long duration = pulseIn(ECHO_PIN, HIGH, 30000);

    // If no echo is received, return a large distance
    if (duration == 0) {
        return 999;
    }

    return duration * 0.034 / 2;
}
