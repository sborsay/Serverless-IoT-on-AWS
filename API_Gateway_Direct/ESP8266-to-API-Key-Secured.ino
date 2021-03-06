/*
    HTTP over TLS (HTTPS) example sketch
    This example demonstrates how to use
    WiFiClientSecure class to access HTTPS API.
    We fetch and display the status of
    esp8266/Arduino project continuous integration
    build.
    Limitations:
      only RSA certificates
      no support of Perfect Forward Secrecy (PFS)
      TLSv1.2 is supported since version 2.4.0-rc1
    Created by Ivan Grokhotkov, 2015.
    This example is in public domain.
    * This example modified by Stephen Borsay for AWS Serverless course on Udemy
    * to Connect your device directly to AWS API Gateway
    * modified for sending fake data buffer, connect any sensor as desired
    * 
*/

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

#ifndef STASSID
#define STASSID "<YOUR-WIFI-NETWORK>"
#define STAPSK  "<YOUR-NETWORK-PASSWORD>"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;
const char* host = "<YOUR-API-GATEWAY-ENDPOINT>.execute-api.<YOUR-REGION>.amazonaws.com"; //do not include "https://"
String url = "<YOUR-API-GATEWAY-DEPLOYMENT-NAME>";

const char* API_KEY = "<YOUR-API-GATEWAY_API-KEY-HERE>";

const int httpsPort = 443;
unsigned long uptime;

// Use web browser to view and copy  SHA1 fingerprint of the certificate 
//to acquire the thumbprint for this webpage, go to the breadcrumbs in the upper right corner of your browser.
//Then go to Tools-->developer tools-->security-->view certificate-->details(tab)-->thumbprint
//const char fingerprint[] PROGMEM = "98 f8 5e fc 87 65 43 5f 0f c1 1e fe e9 81 c9 9c c2 43 27 4c"; //example thumbprint with proper formatting

const char fingerprint[] PROGMEM = "<YOUR-SHA-THUMBPRINT>";

 WiFiClientSecure client;
 
void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Use WiFiClientSecure class to create TLS connection
 
  Serial.print("connecting to ");
  Serial.println(host);

  Serial.printf("Using fingerprint '%s'\n", fingerprint);
  client.setFingerprint(fingerprint);

  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }
  //String url = "/dep1";
  
  Serial.print("requesting URL: ");
  Serial.println(url);

}


void loop() {
  int t =  random(30,110); //fake number range, adjust as you like
  int h =  random(50,100);


  Serial.print("uptime: ");
  uptime = millis()/1000;
  Serial.println(uptime); //prints time since program started
  

  client.print(String("GET ") + url + "/?uptime=" + (String) uptime
       + "&temperature=" + (String) t + "&humidity=" + (String) h + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "x-api-key: " + API_KEY + "\r\n" +
               "User-Agent: 14 ESP8266\r\n" +
               "Connection: close\r\n\r\n");

  Serial.println("request sent");
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }
  String line = client.readStringUntil('\n');
  if (line.startsWith("{\"state\":\"success\"")) {
    Serial.println("esp8266/Arduino CI successfull!");
  } else {
    Serial.println("esp8266/Arduino CI has failed");
  }
  Serial.println("reply was:");
  Serial.println("==========");
  Serial.println(line);
  Serial.println("==========");
  Serial.println("closing connection");
  delay(1000);

//unlike MQTT, HTTP/HTTPS has to be reconstructed every time a request is processed
// so reconnect after GET request is completed and key/value URL payload is dispatched
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }
   delay(1000);
}
