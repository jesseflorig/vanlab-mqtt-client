# Vanlab MQTT Client

MQTT client written in C++ for the [Adafruit Feather RP2040](https://www.adafruit.com/product/4884) using the [Silicognition PoE FeatherWing](https://silicognition.com/Products/poe-featherwing/).

## Quick Start

1. Clone and flash the sketch to an RP2040 device using the Arduino IDE.

## Features

### MQTT
Features specifically for interacting with the MQTT broker:
- __Easy Broker Config__ - Easily set broker configuration
- __Basic Auth Support__ - connect to broker with username and password
- __Last Will and Testament (LWT)__ - preload client notification on dirty disconnect
- __Auto-Reconnect__ - automatic reconnection to broker after disconnect
- __Heartbeat__ - publishes a heartbeat every 10 seconds
- __Publish Topic Map__ - named topic map for publishing
- __PublishTo Function__ - publish payloads to named publish topics
- __Subscribe Topic Map__ - named topic map for subscribing with callback handlers
- __Message Handler Functions__ - easily set handler logic per topic message

### Utility
Miscellaneous features this project incorporates for an improved developer experience:
- __DEBUG Flag and Macros__ - Set `DEBUG` to `0` to disable and strip all `DEBUG` statements
- __Easy Ethernet Config__ - Easily set static IP for W5500 hardware

## Todo
- [ ] Sensor library
