#include <SPI.h>
#include <RF24.h>

// CE = D7, CSN = D8
RF24 radio(7, 8, 1000000);

const byte address[6] = "NODE1";

void setup() {
  Serial.begin(9600);
  delay(1000);

  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);

  Serial.println("Starting transmitter...");

  bool beginOK = radio.begin();
  bool chipOK = radio.isChipConnected();

  Serial.print("radio.begin() = ");
  Serial.println(beginOK ? "OK" : "FAILED");

  Serial.print("isChipConnected() = ");
  Serial.println(chipOK ? "YES" : "NO");

  if (!chipOK) {
    Serial.println("Chip not connected. Stop.");
    while (1);
  }

  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_1MBPS);
  radio.setChannel(76);
  radio.setAddressWidth(5);
  radio.setRetries(5, 15);

  radio.openWritingPipe(address);
  radio.stopListening();

  Serial.println("Transmitter running.");
}

void loop() {
  char msg[32] = "NRF_TEST_OK";

  bool success = radio.write(&msg, sizeof(msg));

  if (success) {
    Serial.println("Sent: NRF_TEST_OK");
  } else {
    Serial.println("Send failed");
  }

  delay(1000);
}
