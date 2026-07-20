#include <SPI.h>


const int CE_PIN  = 7;
const int CSN_PIN = 8;

#define R_REGISTER 0x00
#define W_REGISTER 0x20
#define NOP        0xFF

byte readReg(byte reg) {
  digitalWrite(CSN_PIN, LOW);
  byte status1 = SPI.transfer(R_REGISTER | (reg & 0x1F));
  byte val = SPI.transfer(NOP);
  digitalWrite(CSN_PIN, HIGH);

  Serial.print("READ reg 0x");
  Serial.print(reg, HEX);
  Serial.print(" status 0x");
  Serial.print(status1, HEX);
  Serial.print(" value 0x");
  Serial.println(val, HEX);

  return val;
}

void writeRegDebug(byte reg, byte val) {
  digitalWrite(CSN_PIN, LOW);
  byte status1 = SPI.transfer(W_REGISTER | (reg & 0x1F));
  byte status2 = SPI.transfer(val);
  digitalWrite(CSN_PIN, HIGH);

  Serial.print("WRITE reg 0x");
  Serial.print(reg, HEX);
  Serial.print(" val 0x");
  Serial.print(val, HEX);
  Serial.print(" status1 0x");
  Serial.print(status1, HEX);
  Serial.print(" status2 0x");
  Serial.println(status2, HEX);

  delay(10);

  byte check = readReg(reg);

  Serial.print("VERIFY reg 0x");
  Serial.print(reg, HEX);
  Serial.print(" should be 0x");
  Serial.print(val, HEX);
  Serial.print(" actually 0x");
  Serial.println(check, HEX);
  Serial.println();
}

void setup() {
  Serial.begin(9600);
  delay(1000);

  pinMode(CE_PIN, OUTPUT);
  pinMode(CSN_PIN, OUTPUT);
  pinMode(10, OUTPUT);

  digitalWrite(CE_PIN, LOW);
  digitalWrite(CSN_PIN, HIGH);
  digitalWrite(10, HIGH);

  SPI.begin();
  SPI.beginTransaction(SPISettings(125000, MSBFIRST, SPI_MODE0));

  Serial.println("nRF write status debug");

  Serial.println("Initial:");
  readReg(0x00); // CONFIG
  readReg(0x05); // RF_CH
  readReg(0x06); // RF_SETUP
  readReg(0x03); // SETUP_AW

  Serial.println("Writing test values:");
  writeRegDebug(0x05, 76);   // RF_CH
  writeRegDebug(0x06, 0x06); // RF_SETUP: 1Mbps, 0dBm-ish safer test than 250kbps
  writeRegDebug(0x00, 0x0F); // CONFIG: RX, PWR_UP, CRC
  writeRegDebug(0x03, 0x03); // SETUP_AW: 5-byte address
}

void loop() {
}