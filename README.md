# Wireless Forward-Obstacle Warning System

A compact vehicle-mounted prototype that uses LiDAR ranging and wireless communication to warn a driver when an object is detected ahead.

The system is split into two units:

- A **sensor/transmitter unit** mounted toward the front of the vehicle
- A **receiver/warning unit** located near the driver

The sensor unit measures distance using a TF-Luna LiDAR, processes the reading with an Arduino Nano, and transmits warning data over an nRF24L01 radio link. The receiver listens for the message and activates an indicator when an obstacle is inside the configured warning range.

> **Status:** Active hardware and firmware development

---

## System Overview

```text
TF-Luna LiDAR
      |
      | I2C
      v
Arduino Nano
      |
      | SPI
      v
nRF24L01 Transmitter
      |
      | 2.4 GHz wireless link
      v
nRF24L01 Receiver
      |
      | SPI
      v
Arduino Nano
      |
      v
Warning Indicator
```

The sensor node is intended to operate from a rechargeable battery charged by a small solar panel.

---

## Features

- Forward distance measurement using a TF-Luna LiDAR
- Wireless warning transmission using nRF24L01 modules
- Arduino Nano-based control on both ends
- Packet acknowledgement support
- Configurable distance threshold
- Receiver-side warning LED
- Solar-assisted battery charging
- Custom PCB development in KiCad
- Separate radio, LiDAR, power, and firmware test configurations

---

## Hardware

### Sensor / Transmitter Unit

- Arduino Nano V3
- DFRobot TF-Luna ToF Micro Single-Point Ranging LiDAR
- nRF24L01 2.4 GHz transceiver
- TI BQ25306 battery charger
- Two-cell rechargeable battery system
- DFRobot FIT0330 9 V, 600 mA solar panel
- 3.3 V regulation for the radio
- External LiDAR connector
- Charging-status LED
- Custom PCB

### Receiver Unit

- Arduino Nano V3
- nRF24L01 2.4 GHz transceiver
- Warning LED or other indicator
- Regulated power source

---

## Electrical Interfaces

### TF-Luna LiDAR

The TF-Luna is used as an off-board sensor head.

| Signal | Connection |
|---|---|
| Power | 5 V |
| Ground | GND |
| SDA | Arduino Nano A4 |
| SCL | Arduino Nano A5 |

The I2C bus uses approximately **3 kΩ pull-up resistors to 3.3 V**.

### nRF24L01

The nRF24L01 communicates with the Arduino Nano over SPI.

| nRF24L01 Pin | Arduino Nano |
|---|---|
| VCC | Regulated 3.3 V |
| GND | GND |
| CE | Firmware-defined GPIO |
| CSN | Firmware-defined GPIO |
| SCK | D13 |
| MOSI | D11 |
| MISO | D12 |
| IRQ | Optional |

The nRF24L01 must be powered from a stable 3.3 V rail. A local bypass capacitor should be placed close to the module.

**Do not connect 5 V directly to the nRF24L01 VCC pin.**

---

## Battery Charging System

The sensor node uses a TI BQ25306 charger configured for a two-cell Li-ion battery.

Current design values include:

| Parameter | Value |
|---|---:|
| Charge voltage | Approximately 8.4 V |
| Feedback upper resistor | 1.33 MΩ |
| Feedback lower resistor | 200 kΩ |
| BAT-to-FB capacitor | 470 pF |
| Inductor | 2.2 µH |
| BAT capacitor | 10 µF |
| PMID capacitor | 10 µF |
| VBUS capacitor | 2.2 µF |
| REGN capacitor | 2.2 µF |
| Charge-current resistor | 78.7 kΩ |

The selected 9 V, 600 mA solar panel is electrically compatible with the charger, but available charging current depends strongly on lighting conditions and panel orientation.

---

## Firmware Operation

### Transmitter

The transmitter firmware is intended to:

1. Initialize the TF-Luna and nRF24L01.
2. Read the measured distance.
3. Compare the distance with the warning threshold.
4. Transmit the distance and warning state.
5. Check for a returned acknowledgement.
6. Indicate successful or failed transmission when required.

### Receiver

The receiver firmware is intended to:

1. Initialize the nRF24L01.
2. Listen for incoming packets.
3. Validate received data.
4. Activate the warning indicator when the warning condition is true.
5. Turn the indicator off when the path is clear.
6. Enter a safe state when received data becomes stale.

---

## Repository Structure

```text
.
├── README.md
├── hardware/
│   ├── schematic/
│   ├── pcb/
│   ├── footprints/
│   └── datasheets/
├── firmware/
│   ├── transmitter/
│   ├── receiver/
│   └── radio-tests/
├── documentation/
│   ├── wiring/
│   ├── design-notes/
│   └── test-results/
└── images/
```

The current repository may not yet follow this exact structure. It can be reorganized as the project develops.

---

## Current Development Priorities

- Improve packet and acknowledgement reliability
- Confirm stable 3.3 V radio power during transmission
- Finalize CE and CSN pin assignments
- Validate TF-Luna communication on the completed PCB
- Add packet timeout and fail-safe behavior
- Measure total current consumption
- Test solar charging under realistic conditions
- Confirm connector and 30 AWG wire compatibility
- Complete enclosure and weather-resistance design
- Perform controlled vehicle-mounted testing

---

## KiCad Files

The hardware design is developed in KiCad. Important project files include:

```text
*.kicad_pro
*.kicad_sch
*.kicad_pcb
*.kicad_sym
*.kicad_mod
*.pretty/
```

A suggested `.gitignore` for KiCad is:

```gitignore
# KiCad backups and temporary files
*-backups/
_autosave-*
*.lck
*.bak
*.tmp
fp-info-cache

# Operating-system files
.DS_Store
Thumbs.db
```

---

## Uploading Changes

After editing the project locally:

```bash
git status
git add .
git commit -m "Describe the changes"
git push
```

Before starting work on another computer:

```bash
git pull
```

---

## Safety Notice

This project is an experimental driver-assistance prototype.

It is not a certified automotive safety device and must not be relied upon as the sole method of obstacle detection, collision avoidance, or driver warning. Testing should be performed in a controlled environment before any vehicle-mounted use.

---

## Author

Gianni Alberico
