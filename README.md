# esphome_c14_protocol

ESPHome C14Protocol component to communicate via RS485 - C14 protocol developed by Compit from Poland, air conditioning, ventilation and heating controllers producer.

C14Nano1 component dedicated to communicate strictly with Compit Nano1 controller.

Currently C14nano1 component supports reading values from temperature sensors. It also support reading and setting operation mode of connected ventilation box managed by the controller.

I've used a [@syssi](https://github.com/syssi)/[esphome-jk-bms](https://github.com/syssi/esphome-jk-bms) as a scuffolding .

It based on backward engineering, cause i do not have any documentation.

If someone knows more details of the protocol, such as registers addresses, functions, possible data values and so on, please contact me or try to add new functionality.

Very early stage of development. Experimental. Use for your own risk.

## Requirements

* [ESPHome 2024.6.0 or higher](https://github.com/esphome/esphome/releases).
* Generic ESP32 or ESP8266 board

## Modules

<a href="https://raw.githubusercontent.com/geemark7/esphome_c14_protocol/main/images/modules.png" target="_blank">
<img src="https://raw.githubusercontent.com/geemark7/esphome_c14_protocol/main/images/modules.png" width="50%">
</a>

## Schematics

<a href="https://raw.githubusercontent.com/geemark7/esphome_c14_protocol/main/images/schema.png" target="_blank">
<img src="https://raw.githubusercontent.com/geemark7/esphome_c14_protocol/main/images/schema.png" width="50%">
</a>

## References

* https://www.elektroda.pl/rtvforum/viewtopic.php?p=19766444#19766444
* https://compit.pl/aktualnosci/3-protokol-komunikacyjny-c14-jak-to-ugryzc-czesc-2.html

