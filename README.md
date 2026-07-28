## Overview

This project implements a **Modbus RTU Master** on the **STM32F103CBT6** microcontroller. The firmware periodically sends a Modbus RTU request every **10 seconds** over an RS-485 interface to an **ES35-SW** sensor to retrieve measurement data. The received data is validated, processed, and displayed in real time on a **128×64 OLED display** driven by the **SSD1306** controller.

### Main Features

- STM32F103CBT6-based Modbus RTU Master
- Polls the ES35-SW sensor every **10 seconds**
- RS-485 communication using the Modbus RTU protocol
- Real-time sensor data display on an SSD1306 OLED

## Low-Power Design

The primary design goal of this firmware is to maximize MCU sleep time and minimize unnecessary CPU activity.

Instead of continuously polling peripherals inside the main loop, the firmware relies entirely on hardware interrupts. The main loop only executes the `__WFI()` instruction, allowing the Cortex-M3 core to remain in sleep mode until an interrupt is generated.

```c
while (1)
{
    ...
    __WFI();
    ...
}
```

Periodic communication is initiated by the TIM1 interrupt every 10 seconds, while UART DMA interrupts handle the reception and processing of Modbus responses. This interrupt-driven architecture reduces power consumption and keeps CPU utilization to a minimum.

## Hardware

The project is built using the following hardware components:

| Component | Description |
|----------|-------------|
| **STM32 Blue Pill** | Development board based on the **STM32F103CBT6** microcontroller |
| **MAX485 Module** | UART-to-RS485 transceiver for Modbus RTU communication |
| **ES35-SW Sensor** | Modbus RTU slave device that provides sensor measurements |
| **0.96" OLED Display** | 128×64 OLED display using the **SSD1306** controller over I²C |

### Peripheral Configuration

| Peripheral | Purpose |
|------------|---------|
| **USART1** | Modbus RTU communication with the ES35-SW sensor via the MAX485 module |
| **I2C1** | Communication with the SSD1306 OLED display |

## Hardware Connection Diagram

```text
                        +----------------------+
                        | STM32F103CBT6        |
                        | (Blue Pill)          |
                        |                      |
                        | USART1 TX ----------+------------------+
                        | USART1 RX <---------+                  |
                        |                                         |
                        | I2C1 SCL --------------------------+     |
                        | I2C1 SDA --------------------------|-----+
                        +------------------------------------|------+
                                                             |      |
                                                             |      |
                                      +----------------------+      |
                                      |                             |
                         +------------v-----------+      +----------v-----------+
                         | Module UART to RS485   |      | SSD1306 OLED Display |
                         |                        |      |      (I²C)           |
                         | TX  <----- USART1 TX   |      | SCL <----- I2C1 SCL |
                         | RX  -----> USART1 RX   |      | SDA <----- I2C1 SDA |
                         | A/B -------------------+------>                      |
                         +------------+-----------+      +----------------------+
                                      |
                                      | RS485 Bus
                                      |
                         +------------v-----------+
                         | ES35-SW Sensor         |
                         | (Modbus RTU Slave)     |
                         +------------------------+
```

### Communication Topology

```text
                Modbus RTU
+-----------+    UART    +-----------+    RS485    +-------------+
| STM32     |----------->|  MAX485   |===========>|  ES35-SW     |
| USART1    |<-----------|           |<===========|   Sensor     |
+-----------+            +-----------+            +-------------+

                  I²C
+-----------+------------------------------+--------------------+
| STM32     |                              | SSD1306 OLED       |
| I2C1      |----------------------------->| Display            |
+-----------+                              +--------------------+
```

## Software Architecture

The firmware is organized into separate layers to improve readability, maintainability, and code reusability.

| Folder | Description |
|--------|-------------|
| **Application/** | Contains the main application logic and coordinates the overall firmware workflow. |
| **Components/** | Contains reusable drivers and protocol implementations, including the Modbus RTU stack, sensor driver, and OLED display driver. |
| **Core/** | Auto-generated startup code, interrupt handlers, and system initialization files generated by STM32CubeMX. |
| **Drivers/** | STM32 HAL and CMSIS libraries generated by STM32CubeMX for hardware abstraction and peripheral access. |

### Software Structure

```text
Application
    │
    ├── Application Logic
    │
    ▼
Components
    ├── Modbus RTU
    ├── ES35-SW Driver
    └── SSD1306 Driver
    │
    ▼
Drivers (HAL / CMSIS)
    │
    ▼
STM32F103CBT6 Hardware
```

## Communication Flow

The firmware periodically polls the ES35-SW sensor every **10 seconds** using **TIM1**. The communication process is fully interrupt-driven and follows the sequence below:

```text
TIM1 Update Interrupt (10 s)
            │
            ▼
Timer Callback
            │
            ▼
Build & Transmit Modbus RTU Request
            │
            ▼
Receive Response via UART DMA
            │
            ▼
DMA Reception Complete Interrupt
            │
            ▼
Parse Modbus Frame
            │
            ▼
Extract Sensor Data
            │
            ▼
Update SSD1306 OLED Display
```

### Communication Sequence

1. **TIM1** generates an update interrupt every **10 seconds**.
2. The timer callback builds and transmits a Modbus RTU request to the ES35-SW sensor through **USART1**.
3. The response frame is received using **UART DMA**, minimizing CPU involvement during data reception.
4. Once the expected frame has been received, the **DMA reception complete interrupt** is triggered.
5. The received Modbus frame is validated and parsed to extract the sensor measurement.
6. The latest sensor value is displayed on the **SSD1306 OLED** via **I²C1**.



