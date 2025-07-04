# Vanlab MQTT Client

MQTT client written for the [Adafruit Feather RP2040](https://www.adafruit.com/product/4884). using the [Silicognition PoE FeatherWing](https://silicognition.com/Products/poe-featherwing/).

## Quick Start

1. Clone and flash the sketch to an RP2040 device using the Arduino IDE.

## Features

### MQTT
- __Basic Auth Support__ - connect to broker with username and password
- __Last Will and Testament (LWT)__ - preload client notification on dirty disconnect

### Utility
- __DEBUG Flag and Macros__ - Set `DEBUG` to `0` to disable and strip all `DEBUG` statements

## Todo

- [ ] Modular topic mapping
- [ ] Modular topic handlers
