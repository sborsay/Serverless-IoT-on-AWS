/* ESP8266 AWS IoT
 *  
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

const char* ssid = "<YOUR-WIFI-NETWORK>";
const char* password = "<YOUR-WIFI-PASSWORD>";

// Find this awsEndpoint in the AWS Console: Manage - Things, choose your thing
// choose Interact, its the HTTPS Rest endpoint.  Endpoints ARE regional
const char* awsEndpoint = "YOUR-ACCOUNT-ID-ats.iot.YOUR-CERTIFICATE-REGION.amazonaws.com";

// For the two certificate strings below paste in the text of your AWS 
// device certificate and private key, comment out the BEGIN and END 
// lines, add a quote character at the start of each line and a quote 
// and backslash at the end of each line:

// xxxxxxxxxx-certificate.pem.crt
const String certificatePemCrt = \
//-----BEGIN CERTIFICATE-----
R"EOF(

XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXMA0GCSqGSIb3DQEB
CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t
IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0xOTExMDYxODI1
NDFaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh
dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDRSWA/2xSg/OYOOM6d
2smJIEn3VIgmqCEmrg1+6vdoKBLxgMh194z2Ns83siRG9GPf7+v5oWk0Bu9kDKNl
vquAnBO3+eW1Sgg3JE9SP5utExYGDXud1im6dlG/YbnN8gWCG3W0Abx1vfsiJMP7
yhez8Xp4lb+/fSDH/vbi5IqYLsCsUgSB7hrg0a8zqXa16lq5FjTgUk9CxFDb5V+z
ipXLfcXboisLIAjJMhNXnef+CpM4rQJulf1eZxCV3P9Du8eFpGKx1VFm8/D1pwrh
GJ1N9kFuSWfQHhj+gA383OL7andGE9h2097O4KaqXW9coCZVKv3AjX2WWqczL3uV
6AOdAgMBAAGjYDBeMB8GA1UdIwQYMBaAFOwXufzDBIb6LGYzX9hsuuTkGBggMB0G
A1UdDgQWBBQRfgU2R8TDTFz6lgUra5M/9pFn6jAMBgNVHRMBAf8EAjAAMA4GA1Ud
DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEAB4tmHWDNBu74BDIn+f32c1ED
r+HJjZAVWBwG9v7ubFu/uA+TaHYT+KBaAfB4NfAlOmTpMZN6egLkKfRNNDIA+tAH
PV/QBDZWjCR3YS5sAUIrnSSRzFGRtAFtscYJhJ73Ahhob8H6zXyH2XQRft4663dI
fqMlXsofXS7QH/mhy1zv13su8EaS/UNJRMvB/+ESyEwkQ2BhzpE8TjaBXTkyZuKw
wSBoHO71UqJGhmLkDLp99hKc90KWh08v9jmNBfbTZvIUcCsIz6EhcbfUAg8oBSE0
yuzWG1P4ZwUk70SDcHhDty8GZ9oajrCsLTcv7zrw4aUtfuIFOqXLybqVOCaGOw==

)EOF"; 
//-----END CERTIFICATE-----

// xxxxxxxxxx-private.pem.key
const String privatePemKey = \
//-----BEGIN RSA PRIVATE KEY-----
R"EOF(

XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXq4Nfur3aCgS8YDI
dfeM9jbPN7IkRvRj3+/r+aFpNAbvZAyjZb6rgJwTt/nltUoINyRPUj+brRMWBg17
ndYpunZRv2G5zfIFght1tAG8db37IiTD+8oXs/F6eJW/v30gx/724uSKmC7ArFIE
ge4a4NGvM6l2tepauRY04FJPQsRQ2+Vfs4qVy33F26IrCyAIyTITV53n/gqTOK0C
bpX9XmcQldz/Q7vHhaRisdVRZvPw9acK4RidTfZBbkln0B4Y/oAN/Nzi+2p3RhPY
dtPezuCmql1vXKAmVSr9wI19llqnMy97legDnQIDAQABAoIBAAKo/RkyrqtxK3do
z2+ANWmRyH7lSym6n1k/gxpm4CgpwjvhmCqvr9H9Vlrt37orJw3Undo5a/3mTKqn
4nfLmaBz22hOO9Y3D62rv3pbJp5njLMc/sI905/0GwvksRjmB53kvXVBRGT2ujdv
4jHUGsMFwbnApYRIkd8Y5YhywTQ39gQeC30suTikIlqSbXnT5KJkKcKvsfnAbsx0
bp+7grEYZjkmki7f7KJgJpVWOQBg8JhbRHBFMiIsMQORPDI+NfaHtgUKxBKykCju
gkkf7ljslfMrfLJvB5gGJetoc/ilLS+19O6VZVN3bChpL4OKE+WnsOdjMLPzmfWL
fIoYiAECgYEA9TpqOwUbWuFjAXIU+lTjn/jJCmnkvX3S0tJ9AXxkZBzfN0BlDlMC
XkEfaARLqj05YbvwfikmnXbER9hE3TjHh8uq+Pv8ZzK432X9/kTLuNKksan3sDgP
e94sQmKrNm5cTw2K9fMsk04W9UdkQGG2duZvOINW8iPqtfNy0jj6rZ0CgYEA2nrM
jmoC4MTuhtEDaishocVH5Q3MVCrZWh1QELgu+d7XUFuvpByT1Dw68PAdlcUjQa8t
+1jopKHGEaZ3781dSkFL6fCcpOn4bGBvNKzURt6avj9r4WoK6tzLqymZbr+32eNc
vXgui3fz6V7fZDtE03ixw62qEgLJF+08yhFNzgECgYB7WW+30kDJPNe1EXI13N9G
RziwsUUqf5C9FL1mMvC4XsF0pEJwqxZk3LL0ejypG/SyEXvNqdtPlz7xuHojIH1U
9ABDD1UNf7j4PfA9ptMmW2YWK514GSrIrp9qoQDn9ykdZn2Aa1n/mmb353oo3D2Q
nyZQsdfZInHcJeGalqiZDQKBgQDZ6N0ZtlbEhOc7hEEZpYdX6IL6zLZdxJchMFdp
Nat1AXRT6/7VesNfTeuj4HpBpWyy2NzN8zGm8suxUw4RGg3QQCxNWvWB7vMedVi1
eyQGw4Qn+O3K2I+nDS3+u/ES6xmesw0O1U3nQW7/9uZs/Eh7e8gh2Zble+3CdZbK
HFQ0AQKBgQCQWrO4PaQQIZ/ZPi9/lKZLRHAb30gx1Qpgpv2pbpxVyIKRfOdDvpHg
Fp+icoIMvHcPoJPz13QYujADzmKpLviKYXDDojJ6sCrPmieV0wAEsF19zbOkMZgr
guGf0uCCv7CpEoRvaVUAX12/Kp9iA03b5hc3hTqBTMSKirdRlouugA==

)EOF";
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

WiFiClientSecure wiFiClient;
void msgReceived(char* topic, byte* payload, unsigned int len);
PubSubClient pubSubClient(awsEndpoint, 8883, msgReceived, wiFiClient); 

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
  wiFiClient.setCertificate(binaryCert, len);
  
  uint8_t binaryPrivate[privatePemKey.length() * 3 / 4];
  len = b64decode(privatePemKey, binaryPrivate);
  wiFiClient.setPrivateKey(binaryPrivate, len);

  uint8_t binaryCA[caPemCrt.length() * 3 / 4];
  len = b64decode(caPemCrt, binaryCA);
  wiFiClient.setCACert(binaryCA, len);
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
    String msg = String("Hello from ESP8266: ") + ++msgCount;

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
  configTime(3 * 3600, 0,"pool.ntp.org", "time.nist.gov");

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
