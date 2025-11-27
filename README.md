**Demo-Project** to show how to use KNX-Stack and LED library on a **ESP32** or **ESP8266**.  
Tested with VScode and **PlatformIO** v6.1.18  
using Platforms Espressif 32 v6.12.0, Espressif 8266 v4.2.1 and LibreTiny v1.9.2  
  
**Required libraries and folder structure:**  
...\Platformio\demo-esp-knx\knx-led-rgbcct  
...\Platformio\libraries\esp-knx-common (https://github.com/Phil1pp/esp-knx-common)  
...\Platformio\libraries\esp-knx-led (https://github.com/Phil1pp/esp-knx-led)  
...\Platformio\libraries\esp-knx-stack (https://github.com/thelsing/knx)  
...\Platformio\libraries\esp-knx-webserver (https://github.com/Phil1pp/esp-knx-webserver)  
  
Adjust settings.h and copy credentials.template.h to credentials.h and adjust as well