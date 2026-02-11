/****************************************************
 SISTEMA INTELIGENTE DE MONITOREO ELÉCTRICO IoT
 ESP32 + PZEM + Dashboard + Bluetooth + ThingSpeak
****************************************************/

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <PZEM004Tv30.h>
#include "BluetoothSerial.h"
#include <ThingSpeak.h>

//================ WIFI =================
const char* ssid = "TU_WIFI";
const char* password = "TU_PASSWORD";

//================ THINGSPEAK =================
WiFiClient client;
unsigned long channelID = TU_CHANNEL_ID;
const char* writeAPI = "TU_WRITE_API";

//================ BLUETOOTH =================
BluetoothSerial SerialBT;

//================ PZEM SENSOR =================
#define PZEM_RX_PIN 16
#define PZEM_TX_PIN 17

PZEM004Tv30 pzem(Serial2, PZEM_RX_PIN, PZEM_TX_PIN, 0x20);

//================ DATOS =================
struct Sensor {
  float v,c,p,e,f,fp;
};

Sensor data;

//================ WEB SERVER =================
AsyncWebServer server(80);

//================ LECTURA SENSOR =================
void leerPZEM(){

  data.v = pzem.voltage();
  data.c = pzem.current();
  data.p = pzem.power();
  data.e = pzem.energy();
  data.f = pzem.frequency();
  data.fp = pzem.pf();

  if(isnan(data.v)) data.v=-1;
  if(isnan(data.c)) data.c=-1;
  if(isnan(data.p)) data.p=-1;
  if(isnan(data.e)) data.e=-1;
  if(isnan(data.f)) data.f=-1;
  if(isnan(data.fp)) data.fp=-1;

  // Bluetooth envío
  SerialBT.printf("V: %.2f | I: %.2f | P: %.2f\n",
                   data.v,data.c,data.p);

  Serial.println("Datos leídos del sensor");
}

//================ ENVÍO A THINGSPEAK =================
void subirThingSpeak(){

  ThingSpeak.setField(1,data.v);
  ThingSpeak.setField(2,data.c);
  ThingSpeak.setField(3,data.p);

  ThingSpeak.writeFields(channelID,writeAPI);

  Serial.println("Datos enviados a ThingSpeak");
}

//================ DASHBOARD WEB =================
const char* html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<title>Dashboard Energético</title>
<style>
body{font-family:Arial;background:#0f172a;color:white;text-align:center;}
.grid{display:grid;grid-template-columns:repeat(3,1fr);gap:15px;padding:20px;}
.card{background:#1e293b;padding:20px;border-radius:12px;}
.value{font-size:28px;color:#22d3ee;}
</style>
<script>
function upd(){
fetch("/data").then(r=>r.json()).then(d=>{
v.innerText=d.v;
c.innerText=d.c;
p.innerText=d.p;
e.innerText=d.e;
f.innerText=d.f;
fp.innerText=d.fp;
});
}
setInterval(upd,1000);
</script>
</head>

<body onload="upd()">
<h2>⚡ Dashboard Energético IoT</h2>

<div class="grid">
<div class="card">Voltaje<div id="v" class="value">0</div></div>
<div class="card">Corriente<div id="c" class="value">0</div></div>
<div class="card">Potencia<div id="p" class="value">0</div></div>
<div class="card">Energía<div id="e" class="value">0</div></div>
<div class="card">Frecuencia<div id="f" class="value">0</div></div>
<div class="card">Factor<div id="fp" class="value">0</div></div>
</div>

</body>
</html>
)rawliteral";

//================ SETUP =================
void setup(){

Serial.begin(115200);
delay(1000);

Serial.println("=== INICIO SISTEMA IoT ===");

// Bluetooth
SerialBT.begin("ESP32_ENERGIA");

// UART sensor
Serial2.begin(9600,SERIAL_8N1,PZEM_RX_PIN,PZEM_TX_PIN);
delay(1000);

// WiFi
WiFi.mode(WIFI_STA);
WiFi.begin(ssid,password);

Serial.print("Conectando WiFi");

while(WiFi.status()!=WL_CONNECTED){
delay(500);
Serial.print(".");
}

Serial.println("\nWiFi conectado");
Serial.println(WiFi.localIP());

// ThingSpeak
ThingSpeak.begin(client);

// Web server
server.on("/",HTTP_GET,[](AsyncWebServerRequest *r){
r->send(200,"text/html",html);
});

server.on("/data",HTTP_GET,[](AsyncWebServerRequest *r){

leerPZEM();

String json="{";
json+="\"v\":"+String(data.v,2)+",";
json+="\"c\":"+String(data.c,2)+",";
json+="\"p\":"+String(data.p,2)+",";
json+="\"e\":"+String(data.e,2)+",";
json+="\"f\":"+String(data.f,2)+",";
json+="\"fp\":"+String(data.fp,2);
json+="}";

r->send(200,"application/json",json);
});

server.begin();

Serial.println("Dashboard activo");
}

//================ LOOP =================
void loop(){

static unsigned long timer=0;

if(millis()-timer>15000){

leerPZEM();
subirThingSpeak();

timer=millis();
}

// Bluetooth lectura
if(SerialBT.available()){
Serial.print("BT: ");
Serial.println((char)SerialBT.read());
}

delay(200);
}
