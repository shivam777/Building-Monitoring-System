#include <ESP8266WiFi.h>
 
String apiKey = "YOUR API KEY";     //  Enter your Write API key from ThingSpeak

const char *ssid =  "YOUR WIFI NAME";     // replace with your wifi ssid and wpa2 key
const char *pass =  "YOUR PASSWORD";
const char* server = "api.thingspeak.com";

int EP =14;//Vibration D5
int isFlamePin = 5;  // This is our input pin D1
int isFlame = HIGH;  // HIGH MEANS NO FLAME

WiFiClient client;
 
void setup() 
{
       Serial.begin(115200);
       delay(10);
       Serial.println("Connecting to ");
       Serial.println(ssid);
       WiFi.begin(ssid, pass);
       while (WiFi.status() != WL_CONNECTED) 
         {
            delay(500);
            Serial.print(".");
         }
      Serial.println("");
      Serial.println("WiFi connected");
      Serial.println(ssid);
      pinMode(EP, INPUT); //set EP input for measurment Vibration
      pinMode (12 , INPUT);//raindrops
      pinMode(LED_BUILTIN, OUTPUT);
      pinMode(isFlamePin, INPUT);//Flame
}
 
void loop() 
{
      long measurement =TP_init();//Vibration
      isFlame = digitalRead(isFlamePin);//Flame
      int value = analogRead(A0);//Raindrops A0
      int digital = digitalRead(12);//Raindrops D6

      if (isnan(measurement)) 
        {
            Serial.println("Failed to read from Vibration sensor!");
            return;
        }
      if (isnan(isFlame)) 
        {
           Serial.println("Failed to read from Flame sensor!");
           return;
        }
      if (isnan(value || digital)) 
        {
           Serial.println("Failed to read from Raindrops sensor!");
           return;
        }
      if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
        {  
           String postStr = apiKey;
           postStr +="&field1=";
           postStr += String(measurement);
           postStr +="&field2=";
           postStr += String(isFlame);
           postStr +="&field3=";
           postStr += String(value);                             
           postStr +="&field4=";
           postStr += String(digital);                             
           postStr += "\r\n\r\n\r\n\r\n";
 
       client.print("POST /update HTTP/1.1\n");
       client.print("Host: api.thingspeak.com\n");
       client.print("Connection: close\n"); 
       client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
       client.print("Content-Type: application/x-www-form-urlencoded\n"); 
       client.print("Content-Length: ");
       client.print(postStr.length());
       client.print("\n\n");
       client.print(postStr);
       
       Serial.println(measurement);
       
       if (measurement > 1000)
         {
            digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
            Serial.println("ALERT BUILDING IS VIBRATING");
            delay(1000);                       // wait for a second
         }
       else
         {
            digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
            Serial.println("NO ALERT");
            delay(10);                       // wait for a second
        }
      if (isFlame== LOW)
        {
           Serial.println("No Flame");
           digitalWrite(LED_BUILTIN, HIGH);
        }
      else
         {
           Serial.println("Warning Flame!!!!");
           digitalWrite(LED_BUILTIN, LOW);
           //digitalWrite(LED, LOW);
         }
      if (digital == 0) 
         {
           Serial.println (value);
           Serial.println (digital);
           Serial.println ("Water");
         } 
      if (digital == 1)
         {
           Serial.println (value);
           Serial.println (digital);
           Serial.println ("No Water");
         }
      delay(2000);
       
       client.stop();
       Serial.println("Waiting...");
  
 
  delay(100);
}}
long TP_init(){
  delay(100);
  long measurement=pulseIn (EP, HIGH);  //wait for the pin to get HIGH and returns measurement
  return measurement;
}

