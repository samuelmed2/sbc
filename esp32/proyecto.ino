#include "ThingsBoard.h"
#include "WiFi.h"

#define WIFI_AP             "RODOLFO_Fmhh"
#define WIFI_PASSWORD       "uxsbhmwawf77"

// See https://thingsboard.io/docs/getting-started-guides/helloworld/
// to understand how to obtain an access token
#define TOKEN               "nlaEI1B3qtGoqOG8k4fM"
#define THINGSBOARD_SERVER  "demo.thingsboard.io"

// Baud rate for debug serial

// Initialize ThingsBoard client
WiFiClient espClient;
// Initialize ThingsBoard instance
ThingsBoard tb(espClient);
// the Wifi radio's status
int status = WL_IDLE_STATUS;

//SENSORES
#define SENSOR_HUMEDAD_TIERRA 4
#define LDR 27
#define SENSOR_AGUA 13
#define PESO 12

//ACTUADORES
#define ROJO 18
#define AZUL 23
#define VERDE 19
#define RELE 15


int V;
int ilum;
int temperatura;
float peso;
int humedad;

void setup() {
   pinMode(SENSOR_AGUA, INPUT);
   pinMode(PESO, INPUT);
   
   pinMode(LDR, INPUT);
   pinMode(ROJO, OUTPUT);
   pinMode(AZUL, OUTPUT);
   pinMode(VERDE, OUTPUT);
   pinMode(RELE, OUTPUT);
   
   Serial.begin(9600);
  // initialize serial for debugging
  //WiFi.begin(WIFI_AP, WIFI_PASSWORD);

   


}


void sensores(){
  
  humedad = analogRead(SENSOR_HUMEDAD_TIERRA);

  peso = analogRead(PESO);
  peso = 8*(peso - 228);
  Serial.print(peso);
  Serial.println("gramos");


  //Serial.print("Humedad del suelo: ");
  //Serial.println(analogRead(SENSOR_AGUA));  

  if(analogRead(SENSOR_AGUA) < 300)
  {
    Serial.println("Peligro desboramiento");
    digitalWrite(RELE, LOW);   
  }
  else 
    digitalWrite(RELE, HIGH);   

    
  ilum = analogRead(LDR);    
 
   
}

void actuadores(){


  if(ilum < 500)
  {
    digitalWrite(ROJO, HIGH); 
    digitalWrite(VERDE, HIGH);
  }
  else
  {
    digitalWrite(ROJO, LOW); 
    digitalWrite(VERDE, LOW);   
  }

   Serial.print("Iluminación LDR: ");   
   Serial.println(ilum);  

   
   Serial.print("Humedad del suelo: ");
   Serial.println(humedad);
  
  if(humedad > 2500 || analogRead(SENSOR_AGUA) > 300)
   {
    Serial.println("Regando");  
    digitalWrite(5, HIGH);   // turn the LED on (HIGH is the voltage level)
   }
   else
   {
    Serial.println("Cortando riego");  
    digitalWrite(5, LOW);   // turn the LED on (HIGH is the voltage level)
   }
  
}

void loop() {
 
  sensores();
  actuadores();

  delay(10000);



  Serial.println("Sending data...");

  // Uploads new telemetry to ThingsBoard using MQTT.
  // See https://thingsboard.io/docs/reference/mqtt-api/#telemetry-upload-api
  // for more details

  tb.sendTelemetryInt("ldr", 22);

  tb.loop();


  


  

}


void InitWiFi()
{
  Serial.println("Connecting to AP ...");
  // attempt to connect to WiFi network

  WiFi.begin(WIFI_AP, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to AP");
}

void reconnect() {
  // Loop until we're reconnected
  status = WiFi.status();
  if ( status != WL_CONNECTED) {
    WiFi.begin(WIFI_AP, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("Connected to AP");
  }
}
  