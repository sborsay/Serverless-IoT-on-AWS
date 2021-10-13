/* ESP8266 AWS IoT
 *  
 ----------------------------
 
 ##Compile this only if using ESP8266 version 3+, not backwards comparable
 
 #Tools--> Board--> Board Manager-->ESP8266 version 3+ by ESP8266 Community
 
 ##Older ESP8266 sketch is not forward compatible
 
 --------------------------------
 
 
 * Simplest possible example (that I could come up with) of using an ESP8266 with AWS IoT.
 * No messing with openssl or spiffs just regular pubsub and certificates in string constants
 *
 * This is working as at 3rd Aug 2019 with the current ESP8266 Arduino core release:
 * SDK:2.2.1(cfd48f3)/Core:2.5.2-56-g403001e3=20502056/lwIP:STABLE-2_1_2_RELEASE/glue:1.1-7-g82abda3/BearSSL:6b9587f
 *
 * Author: Anthony Elder
 * License: Apache License v2
 *
 * Sketch Modified by Stephen Borsay for www.udemy.com/course/exploring-aws-iot/
 * https://github.com/sborsay
 * Add in EOF certificate delimiter
 * Add in Char buffer utilizing sprintf to dispatch JSON data to AWS IoT Core
 * First 9 chars of certs obfusicated, use your own, but you can share root CA / x.509 until revoked
 */

 
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
extern "C" {
#include "libb64/cdecode.h"
}

 
const char* ssid = "your WiFi  network";
const char* password = "your wifi password";
 
// Find this awsEndpoint in the AWS Console: Manage - Things, choose your thing
// choose Interact, its the HTTPS Rest endpoint
const char* awsEndpoint = "a32iknowkungfu-ats.iot.us-east-1.amazonaws.com"; //your aws iot endpoint
 
// For the two certificate strings below paste in the text of your AWS
// device certificate and private key, comment out the BEGIN and END
// lines, add a quote character at the start of each line and a quote
// and backslash at the end of each line:
 
// xxxxxxxxxx-certificate.pem.crt
const String certificatePemCrt = \
//-----BEGIN CERTIFICATE-----
"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" \
"CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t" \
"IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0yMTA3MjQxNjU4" \
"MDBaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh" \
"dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDGqz8yFYKa9AdlAnbY" \
"Az7oGlWE4uHy5oowwl4LoclGiZVVikzFa/3AT7YhkoDKtmpFsaoxA/jy7fcxwVqq" \
"yhrRQqibSTy0aIH82nEn9trDAawNXFldniAzHS9BDa6EsBewfMYZUQplACBYu5QR" \
"i70heg+/IEppUfV9bM7JYUdQn5mCGWGDhf+Yf8Er+sFNufWjTpi6djAz4X1qDOnE" \
"EES8wt9r/GTvrLpmVndvhswnlMKaMfupcHyrftJELW3pYu4LH8ugsBVKgv8kA8GF" \
"jZyZWZYmuf3JEPrWPprOU0Kw8vwIOQ/VC1ZKA/VxSHeyo3Q5rJYnSxwruo1SdZF4" \
"XwUrAgMBAAGjYDBeMB8GA1UdIwQYMBaAFBnqGRCT6Kgp0DTTqr/ZZwttLH22MB0G" \
"A1UdDgQWBBTeZ+ZusU9cMJDfjiANujMjioeSRzAMBgNVHRMBAf8EAjAAMA4GA1Ud" \
"DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEAljcZHiWWxoagJtJNPN3wgUtM" \
"yQqJFGl8aU6LQTVW7WQSr5f28l7+6Nsx/4b/BvmiR8E9hwkRky4bJiIFEiVjkWGG" \
"Z1fq2T9q7OlvMF/OIOUCTENdIH+Ljp9YR5cbXHDaPYhBudS7CW+cprFaTGmFdk/f" \
"PMQLpk5iSC2n8qFhEGjlp3TR/mpxs8a4ZJVOqWFt+5rXqVb9JTW/diO7UgD6OmSX" \
"viaGV1lKh7Ju+x1Pj3TG79m4HIGmnjUmWFhZf+RrJVM6t92A20ydrQt1MN0PrY/4" \
"vjFh1BaO3FxCExxqWPzxfpqk3ciJzn0bjmacz2F1ZMCK9B+VgI5VlKjwJnfllw==";
//-----END CERTIFICATE-----
 
// xxxxxxxxxx-private.pem.key
const String privatePemKey = \
//-----BEGIN RSA PRIVATE KEY-----
"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" \
"xWv9wE+2IZKAyrZqRbGqMQP48u33McFaqsoa0UKom0k8tGiB/NpxJ/bawwGsDVxZ" \
"XZ4gMx0vQQ2uhLAXsHzGGVEKZQAgWLuUEYu9IXoPvyBKaVH1fWzOyWFHUJ+Zghlh" \
"g4X/mH/BK/rBTbn1o06YunYwM+F9agzpxBBEvMLfa/xk76y6ZlZ3b4bMJ5TCmjH7" \
"qXB8q37SRC1t6WLuCx/LoLAVSoL/JAPBhY2cmVmWJrn9yRD61j6azlNCsPL8CDkP" \
"1QtWSgP1cUh3sqN0OayWJ0scK7qNUnWReF8FKwIDAQABAoIBAHj/lXbAiRIs7FAF" \
"5zH7EBY2670Kngjdhm7aAoNdHwRaCVzW5ZkcA6lcIxZhbxWHckROR9SgkcALVRG1" \
"rK32d8zZp20kdtd+CyVgOajYfTZlz0833qnZA819yej6Fz9zLIpuBA0pQYZwREMm" \
"w7DIBgHcoLiMRyXMKBMi8PLPz6p25zZYggPoMJ+DfaF1lS2ZVo3F2t5WV1E8Q541" \
"L9ZgOAzks3vhETd1ZbFegrAllSuM5AvoKELdg11QKX89pcis8ir34Z7nzyE8riAt" \
"+fKke6raGMfKmbHo2s37gSlJI9SAAwmQdXkY0vbCXenRPT0mUi/VNFhfJ1Z+6UBd" \
"8oG8DHkCgYEA5pCww2lS2R0qYnH7L2H0pXbI8KdgJ7GF3iIzRE+oSlLKz2vSVKN8" \
"ZUDcEbm6ewjzmNVNsK5dX+eazxaIqnOKwkTsoW3CEN3cdd9uI1HimLgZCX1rVOnZ" \
"vWGgEViXdeaZ3QndU16eDTAEi79CNLckl2EAxsLK2/jDX/HXUIQrwj0CgYEA3JXJ" \
"DPPtqWLGY9uXbyoTatj00D3r3afza8AjmFxjAuRco3wXYVFPMarufXL99NH8fUnn" \
"GDXYHKafNU38koWzXTryY8txbqwbsrNq7ziixYsuyTt9nFfE7OUeZ5CxzomIc0CM" \
"26EYSQvno8AZ9SGbsJW+DkZG2cTuhT4KgPbGY4cCgYBWLd7kK4n7/RrKkTACFesL" \
"rcqNqQIX1BLlZIMlFWEh5rBHt1V2rWThs8yOE43QTxg+F/xh/cqI/liu7w1GKMnE" \
"s15/ODdBdqvgoxLXwNOhoa46DdcAdigc0VCzKxnxx6/bHsTLVKJe64PHeEWWzp2u" \
"t2NC56xLbEAarJp+TejNQQKBgARqezvnJjrd31rgaoevL/RTo5MHqlElq5ncntnN" \
"/61vJOug8FbiadN0uJGKW8sq6QMZF6dEbK8mJsnKZeQJCS40n+WfjQXKLEPHk7x2" \
"RcvunWEp77nRh/+2FoW8P9MCrdig0jETaIkN0/VTQzHFGnjOhp+CiKiTaLMCKvOp" \
"/Q7BAoGAb4U+1aIoZG8pkuH7iorVjj3mzn3QGL5SUGQ/mPUM+n4g/eNc8pfPg7zO" \
"1OwucCY8x2wArFE4LG6G+pOVAmGlbqPMl7lUhOahw5BVETrNYw2L2EWf32/GwhRL" \
"WvMotZjt5nJJXZiMo0y4h3unOvqTNQD/Yp2OdGPwhEp0J7XSxOM=";
//-----END RSA PRIVATE KEY-----
 
// This is the AWS IoT CA Certificate from:
// https://docs.aws.amazon.com/iot/latest/developerguide/managing-device-certs.html#server-authentication
// This one in here is the 'RSA 2048 bit key: Amazon Root CA 1' which is valid
// until January 16, 2038 so unless it gets revoked you can leave this as is:
const String caPemCrt = \
//-----BEGIN CERTIFICATE-----
"MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF" \
"ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6" \
"b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL" \
"MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv" \
"b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj" \
"ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM" \
"9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw" \
"IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6" \
"VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L" \
"93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm" \
"jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC" \
"AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA" \
"A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI" \
"U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs" \
"N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv" \
"o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU" \
"5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy" \
"rqXRfboQnoZsG4q5WTP468SQvvG5";
//-----END CERTIFICATE-----
 
WiFiClientSecure WiFiClient;

void msgReceived(char* topic, byte* payload, unsigned int len); //function prototype

PubSubClient pubSubClient(awsEndpoint, 8883, msgReceived, WiFiClient);
 
X509List *rootCert;
X509List *clientCert;
PrivateKey *clientKey;
 
 
void setup() {
  Serial.begin(115200); Serial.println();
  Serial.println("ESP8266 AWS IoT Example");
 
  Serial.print("Connecting to "); Serial.print(ssid);
  WiFi.begin(ssid, password);
  WiFi.waitForConnectResult();
  Serial.print(", WiFi connected, IP address: "); Serial.println(WiFi.localIP());
 
  // get current time, otherwise certificates are flagged as expired
  setCurrentTime();
 
  uint8_t binaryCert[certificatePemCrt.length() * 3 / 4];
  int len = b64decode(certificatePemCrt, binaryCert);
  clientCert = new BearSSL::X509List(binaryCert, len);
 
  uint8_t binaryPrivate[privatePemKey.length() * 3 / 4];
  len = b64decode(privatePemKey, binaryPrivate);
  clientKey = new BearSSL::PrivateKey(binaryPrivate, len);
 
  WiFiClient.setClientRSACert(clientCert, clientKey);
 
  uint8_t binaryCA[caPemCrt.length() * 3 / 4];
  len = b64decode(caPemCrt, binaryCA);
  rootCert = new BearSSL::X509List(binaryCA, len);
 
  WiFiClient.setTrustAnchors(rootCert);
}
 
unsigned long lastPublish;
int msgCount;
 
void loop() {
 
  pubSubCheckConnect();

  //Add a JSON package of fake data to deliver to AWS IoT
  //Uses snprintf but other viable options are: sprintf, strcpy, strncpy, or  
  //Use the ArduinoJson library for Efficient JSON serialization  
  //If you need to increase buffer size, then you need to change MQTT_MAX_PACKET_SIZE in PubSubClient.h
  char fakeData[128];
  float var1 =  random(55,77); //fake number range, adjust as you like  
  float var2 =  random(77,99);  
  sprintf(fakeData,  "{\"uptime\":%lu,\"temp\":%f,\"humid\":%f}", millis() / 1000, var1, var2);
 
  if (millis() - lastPublish > 10000) {
     pubSubClient.publish("outTopic", fakeData);  
     Serial.print("Published: "); Serial.println(fakeData);
     lastPublish = millis();
  }
}
 
void msgReceived(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received on "); Serial.print(topic); Serial.print(": ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}
 
void pubSubCheckConnect() {
  if ( ! pubSubClient.connected()) {
    Serial.print("PubSubClient connecting to: "); Serial.print(awsEndpoint);
    while ( ! pubSubClient.connected()) {
      Serial.print(".");
      pubSubClient.connect("ESPthing");
    }
    Serial.println(" connected");
    pubSubClient.subscribe("inTopic");
  }
  pubSubClient.loop();
}
 
int b64decode(String b64Text, uint8_t* output) {
  base64_decodestate s;
  base64_init_decodestate(&s);
  int cnt = base64_decode_block(b64Text.c_str(), b64Text.length(), (char*)output, &s);
  return cnt;
}
 
void setCurrentTime() {
  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");
 
  Serial.print("Waiting for NTP time sync: ");
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: "); Serial.print(asctime(&timeinfo));
}
