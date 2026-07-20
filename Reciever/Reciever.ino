#include <SPI.h>
#include <RF24.h>

// CE = D7, CSN = D8
RF24 radio(7, 8, 1000000);

const byte address[6] = "NODE1";
const int ledPin = 3;

void setup() {
  Serial.begin(9600);
  delay(1000);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);

  Serial.println("Starting receiver...");

  bool beginOK = radio.begin();
  bool chipOK = radio.isChipConnected();

  Serial.print("radio.begin() = ");
  Serial.println(beginOK ? "OK" : "FAILED");

  Serial.print("isChipConnected() = ");
  Serial.println(chipOK ? "YES" : "NO");

  if (!chipOK) {
    Serial.println("Chip not connected. Stopping.");
    while (1) {
      digitalWrite(ledPin, LOW);
    }
  }

  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_1MBPS);
  radio.setChannel(76);
  radio.setAddressWidth(5);
  radio.setCRCLength(RF24_CRC_16);

  radio.openReadingPipe(0, address);

  // Clear any old garbage sitting in the receive buffer
  radio.flush_rx();

  radio.startListening();

  Serial.println("Receiver running.");
}

void loop() {
  if (radio.available()) {
    char msg[32] = "";
    radio.read(&msg, sizeof(msg));

    Serial.print("Received: ");
    Serial.println(msg);

    if (strcmp(msg, "NRF_TEST_OK") == 0) {
      Serial.println("VALID MESSAGE");

      digitalWrite(ledPin, HIGH);
      delay(150);
      digitalWrite(ledPin, LOW);
    } else {
      Serial.println("Garbage/stale packet ignored.");
      digitalWrite(ledPin, LOW);

      // clear RX junk
      radio.flush_rx();
    }
  } else {
    digitalWrite(ledPin, LOW);
  }
}