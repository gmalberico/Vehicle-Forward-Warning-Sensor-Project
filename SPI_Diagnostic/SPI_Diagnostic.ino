#include <SPI.h>

const int CE_PIN  = 7;
const int CSN_PIN = 8;

byte readRegister(byte reg) {
  digitalWrite(CSN_PIN, LOW);
  byte status = SPI.transfer(reg & 0x1F);
  byte value  = SPI.transfer(0xFF);
  digitalWrite(CSN_PIN, HIGH);

  Serial.print("STATUS = 0x");
  if (status < 0x10) Serial.print("0");
  Serial.print(status, HEX);

  Serial.print("   REG 0x");
  if (reg < 0x10) Serial.print("0");
  Serial.print(reg, HEX);

  Serial.print(" = 0x");
  if (value < 0x10) Serial.print("0");
  Serial.println(value, HEX);

  return value;
}

void setup() {
  Serial.begin(9600);
  delay(1000);

  pinMode(CE_PIN, OUTPUT);
  pinMode(CSN_PIN, OUTPUT);
  pinMode(10, OUTPUT);   // keep Nano as SPI master

  digitalWrite(CE_PIN, LOW);
  digitalWrite(CSN_PIN, HIGH);
  digitalWrite(10, HIGH);

  SPI.begin();
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));

  Serial.println("nRF register check:");
}

void loop() {
  readRegister(0x00); // CONFIG, expected around 0x08
  readRegister(0x01); // EN_AA, expected around 0x3F
  readRegister(0x03); // SETUP_AW, expected 0x03
  readRegister(0x05); // RF_CH
  Serial.println("---");
  delay(1000);
}