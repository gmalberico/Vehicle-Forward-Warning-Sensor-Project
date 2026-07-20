#include <SPI.h>
#include <RF24.h>

// CE = D7, CSN = D8
RF24 radio(7, 8, 1000000);

const byte address[6] = "ABCDE";
const int ackLedPin = 3;

unsigned long counter = 0;

void setup() {
  Serial.begin(9600);
  delay(1000);

  pinMode(ackLedPin, OUTPUT);
  digitalWrite(ackLedPin, LOW);

  // LED boot test
  digitalWrite(ackLedPin, HIGH);
  delay(300);
  digitalWrite(ackLedPin, LOW);

  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);

  Serial.println("TX close-mode starting...");

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
  radio.setRetries(15, 15);

  radio.openWritingPipe(address);
  radio.stopListening();

  radio.flush_tx();
  radio.flush_rx();

  Serial.println("TX ready.");
}

void loop() {
  char msg[16];
  snprintf(msg, sizeof(msg), "TEST_%lu", counter);

  bool ok = radio.write(msg, sizeof(msg));

  Serial.print("Sent: ");
  Serial.print(msg);
  Serial.print("   Result: ");

  if (ok) {
    Serial.println("ACK RECEIVED");

    // Blink transmitter LED when ACK comes back
    digitalWrite(ackLedPin, HIGH);
    delay(150);
    digitalWrite(ackLedPin, LOW);
  } else {
    Serial.println("NO ACK");

    // LED stays off when no ACK
    digitalWrite(ackLedPin, LOW);
  }

  counter++;
  delay(5);
}