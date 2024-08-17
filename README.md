# esphome_c14_protocol

ESPHome C14Protocol component to communicate via RS485 - C14 protocol developed by Compit from Poland, air conditioning, ventilation and heating controllers producer.

C14Nano1 component dedicated to communicate strictly with Compit Nano1 controller.

Currently C14nano1 component supports reading values from temperature sensors. It also support reading and setting operation mode of connected ventilation box managed by the controller.

I've used a @syssi/esphome-jk-bms as a scuffolding .

It based on backward engineering, cause i do not have any documentation.

If someone knows more details of the protocol, such as registers addresses, functions, possible data values and so on, please contact me or try to add new functionality.

Very early stage of development. Use for your own risk.