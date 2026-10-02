#include <SPI.h>
#include <MFRC522.h>
#include <ESP32Servo.h>

#define SS_PIN  5
#define RST_PIN 22

#define TRIG_PIN 13
#define ECHO_PIN 12
#define SERVO_PIN 14

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

    gateServo.attach(SERVO_PIN);
    gateServo.write(0);

    Serial.println("Scan RFID to open gate...");
}

void loop() {
    if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
        return;
    }

    Serial.print("Card UID: ");
    bool authorized = true;

    for (byte i = 0; i < mfrc522.uid.size; i++) {
        Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
        Serial.print(mfrc522.uid.uidByte[i], HEX);

        if (i >= sizeof(authorizedUID) || mfrc522.uid.uidByte[i] != authorizedUID[i]) {
            authorized = false;
        }
    }
    Serial.println();

    if (mfrc522.uid.size != sizeof(authorizedUID)) {
        authorized = false;
    }

    if (authorized) {
        Serial.println("Access Granted! Opening Gate...");
        gateServo.write(90);
        delay(5000);

        while (getDistance() < 10) {
            delay(500);
        }

        Serial.println("Closing Gate...");
        gateServo.write(0);
    } else {
        Serial.println("Access Denied!");
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

    long duration = pulseIn(ECHO_PIN, HIGH);
    return duration * 0.034 / 2;
}
