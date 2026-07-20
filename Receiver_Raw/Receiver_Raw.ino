#include <SPI.h>
#include <RF24.h>

// CE = D7, CSN = D8
RF24 radio(7, 8, 1000000);

const byte address[6] = "ABCDE";
const int ledPin = 3;

void setup() {
  Serial.begin(9600);
  delay(1000);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // LED boot test
  digitalWrite(ledPin, HIGH);
  delay(500);
  digitalWrite(ledPin, LOW);

  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);

  Serial.println("RX close-mode starting...");

  if (!radio.begin()) {
    Serial.println("radio.begin FAILED");
    while (1);
  }

  radio.powerDown();
  delay(100);
  radio.powerUp();
  delay(100);

  radio.setAutoAck(true);
  radio.disableDynamicPayloads();
  radio.setPayloadSize(16);

  // CLOSE MODE SETTINGS
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_1MBPS);
  radio.setChannel(91);
  radio.setAddressWidth(5);
  radio.setCRCLength(RF24_CRC_16);

  radio.openReadingPipe(1, address);

  radio.flush_rx();
  radio.flush_tx();
  radio.startListening();

  Serial.println("RX ready.");
}

void loop() {
  if (radio.available()) {
    char msg[16] = "";

    radio.read(msg, sizeof(msg));

    Serial.print("Received: ");
    Serial.println(msg);

    // Latch LED on after first valid receive
    digitalWrite(ledPin, HIGH);
  }
}