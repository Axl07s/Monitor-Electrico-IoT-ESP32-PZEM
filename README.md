Sistema Inteligente de Monitoreo Eléctrico IoT (ESP32 + PZEM-004T)

👥 Integrantes
Axel Molineros, 
Jeremy Novoa,
Harold Vinueza

📝 Descripción del Proyecto
Este sistema embebido permite la supervisión de redes eléctricas en tiempo real. Utiliza un microcontrolador ESP32 y un sensor industrial PZEM-004T para capturar variables críticas como voltaje, corriente, potencia y factor de potencia. Los datos se visualizan simultáneamente en un Dashboard web local, una App móvil vía Bluetooth y en la nube a través de ThingSpeak.



📂 Estructura del Repositorio
Para garantizar el cumplimiento de la metodología, el proyecto se organiza de la siguiente manera:



/Código: Contiene el firmware .ino desarrollado para el ESP32.




/Documentación: Incluye el informe técnico final en formato Word/PDF.


/Esquemas: Diagrama de conexiones detallado realizado en Fritzing.


/Media: Fotografías del prototipo físico y capturas del funcionamiento de las plataformas.


🛠️ Requisitos de Hardware

Microcontrolador ESP32 DevKit V1 (Núcleo de procesamiento).




Sensor PZEM-004T V3.0 (Medidor industrial de alta precisión).




Transformador de Corriente (CT) (Medición no invasiva).



Fuente de alimentación 5V/3.3V.

💻 Configuración de Software
Es necesario instalar las siguientes librerías en el IDE de Arduino para la correcta compilación del código:


PZEM004Tv30: Comunicación Modbus-RTU con el sensor.


ThingSpeak: Sincronización con la plataforma en la nube.


ESPAsyncWebServer y AsyncTCP: Para el Dashboard web local asíncrono.

🌐 Visualización de Datos

Dashboard Local: Accesible mediante la IP local del ESP32 http://10.160.250.216



ThingSpeak: https://thingspeak.mathworks.com/channels/3222083
Video Demostrativo: https://youtu.be/HvyWxxEdrPc
