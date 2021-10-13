/* ESP32 AWS IoT
 *  
 * Simplest possible example (that I could come up with) of using an ESP32 with AWS IoT.
 *  
 * Author: Anthony Elder 
 * License: Apache License v2
 * Sketch Modified by Stephen Borsay for www.udemy.com and AWS IoT Live Workshops
 * https://github.com/sborsay
 * Add in Char buffer utilizing sprintf to dispatch JSON data to AWS IoT Core
 * Use and replace your own SID, PW, AWS Account Endpoint, Client cert, private cert, x.509 CA root Cert
 */
#include <WiFiClientSecure.h>
#include <PubSubClient.h> // install with Library Manager, I used v2.6.0

const char* ssid = "YOUR NETWORK NAME";
const char* password = "YOUR NETWORK PASSWORD";

const char* awsEndpoint = "YOUR-AWS-IOT-ENDPOINT"; //AWS IoT Core--> Settings (Device data endpoint)

// xxxxxxxxxx-certificate.pem.crt
static const char certificate_pem_crt[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXAoccwSpJEkwDQYJKoZIhvcNAQEL
BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g
SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTIxMDYyMzAzNDk1
OFoXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0
ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAMlywFsajU0WY53cA2Dv
BdToiQzjBKG/6FKjIYB6D47nn1pvp03xkCewoF09uaKIHam2raL4XV71eeGFvFHj
cn49cfaYPfIsi2Keoxio+H6W+MJZHNt7XE4NiJydqAv+BjC6/aBPRrcP7X+HmDVV
mRbn8uLBnJKRlh6wK1/91q+2e9bpnoNz6IjyUEssQpB9Qtr99FSNl+PNloJr1Ki4
vs4IBJbwNv3kdblHVqd1V7GgYePbx+LqVQzf3IMecNqP1f8WYRMRuQ6RR9dYGFHU
cwmodbigKYd2IGcEOEfU0NYFlXJJ/7a8Nq8KKfDorZAQwJRkr8gOZR9TkpymiYxu
2SUCAwEAAaNgMF4wHwYDVR0jBBgwFoAUk5u9teDtHdsYtv8EPE/ZVGiubEYwHQYD
VR0OBBYEFFefBYP1bu1wgutG7/0Q3pSbYIYTMAwGA1UdEwEB/wQCMAAwDgYDVR0P
AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQCYFcP1pz4el9vBFO5AmTy4MtzM
wpQRyLC/rZpGkBmMxwyAwuweHYnMD2n3jSBcNNoZ69K3lVTpmlw26421P6qtf+Sf
b9e1vZzTtN2izGGNrR93y/3AxA0fCmkD0JNbfomd77uGnyiOOhrX/2b78Kb59vm4
oSjgR4/NU4dM1dJ8isMOaNQxGC1PvOYjFLJlOAEg5VMJUGgfRrqD5d4LVp9k26ok
ArYEEPnFK6wxXl273g+ZS3dED66/Wny6YYZqdGbyuL59DGNnlpjNVkOAYeh/9MZF
TXi8gPMGeExHgXq5oxbkekl58/pm4dkX8Foo0mY8Vfdq5kMqKhtlKQBtQLUp
-----END CERTIFICATE-----
)KEY";

// xxxxxxxxxx-private.pem.key
static const char private_pem_key[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXoUqMhgHoPjuefWm+n
TfGQJ7CgXT25oogdqbatovhdXvV54YW8UeNyfj1x9pg98iyLYp6jGKj4fpb4wlkc
23tcTg2InJ2oC/4GMLr9oE9Gtw/tf4eYNVWZFufy4sGckpGWHrArX/3Wr7Z71ume
g3PoiPJQSyxCkH1C2v30VI2X482WgmvUqLi+zggElvA2/eR1uUdWp3VXsaBh49vH
4upVDN/cgx5w2o/V/xZhExG5DpFH11gYUdRzCah1uKAph3YgZwQ4R9TQ1gWVckn/
trw2rwop8OitkBDAlGSvyA5lH1OSnKaJjG7ZJQIDAQABAoIBAAakewVGBdzB3T0K
US40E3wmwsYjAjcXLeDzshobbEHpD08bGWgvi/JTF6abb3CVjmzXMjiKegG0lHVE
RKZqmo7hFc3szTYcE+iHlJcukysRbVhzOcle8vc76QJJDI6ArNGUiOJtWsaUht9K
qFEx5l9j/JVPjwsiM9IFOxjqNNcMXWAMgVvO1PCNxzI5YzUJADNRjccrFxdzl1b7
sbL3TmUnx/sOCQCg2e8UwNfPj8C4vcE/04IO2ew9zfkNZFy2tru3RXJaB+r5v+Mg
he26f9HuYbcAED4Z6/0zTS0EM7BwN7GTaUUaGezDw0ITGXCvqA8SJul7mCYQOdK1
fKWiJiECgYEA7wIhzfY83hewYHb6DJTIaA3HyxgdiCxXyWZmKtaPT/0flaH2k273
wtTyQfiBthpjKH64ksrSghhIJl0BMDgTcpiaSvVR7EL7deYrGqo/YnzX538oinD9
t2H2TfqFepTkac+QxEZ746ybj9z8EqOMbXgNPa6FeSnJnA9pxObcPpkCgYEA18UJ
/NHmwJgHU7GJHLNM7yO4GrKuGwZlAQwGh75gtm6P7MjnoCmnzgcynep9sW5t2zey
dW8AudSFcVakIi1Q+kWrc92akglYHeRVQwtlfug+pnbbk3V1cBzzGQ63YgZh9FZr
qCCZhjOhqDG8F7EJY7pY3omKn7WGL4pbhstjAm0CgYAbrwyJVbcxJl9InR9Ta7xW
yRnWPWHWTwSQEGtPFPEem7wX0d3Er/rEt3OkDou/swi/PQZWAmYlqXhZ8TXdBvHB
uUlN22J00kFcI5/kg8m5t6ouZz0j4/iPUCp+V8E0NKDMXL1w7pUVBv4xyK/t3MG8
QEaQWXGeWie/Cs3+f5sNKQKBgQCQ72QUFRLBT92sFFcblj0ZbY7fMCLPOG5kU6WX
TQQ9xuqajSE5boGvXRpdBQRFSP42AKW5SETejqjMRvydkH8SffX7MGIBspEkExpK
nL8ijfJ3cuNvm7i1BF0u+bmo+MyXie1q0+sMQGZYSoAkEeFSwfusEAuz2AU4reFp
G/l3yQKBgEMGm87PrO0rcNlcL99pHxT5QjydL9FWvq5eg4dC3yNJcqvd620eXt83
mYcLcO5Q+SWLNm/NjcQCIMZ/cuS1knTDZV3JIT44tCn4Q+Z6OFvy+KNFZOaQc3Ed
Cmr5ttndjNAEfVOUKPN+8wXx4o8Fbd9dkhwggvBaMQhQjaSzzMJs
-----END RSA PRIVATE KEY-----
)KEY";

/* root CA found at:
  https://www.amazontrust.com/repository/AmazonRootCA1.pem
  fungible between regions, I dont care if you use mine below or paste in your own.
*/
static const char rootCA[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)EOF";


WiFiClientSecure wiFiClient;
void msgReceived(char* topic, byte* payload, unsigned int len);
PubSubClient pubSubClient(awsEndpoint, 8883, msgReceived, wiFiClient); 

void setup() {
  Serial.begin(115200); delay(50); Serial.println();
  Serial.println("ESP32 AWS IoT Example");
  Serial.printf("SDK version: %s\n", ESP.getSdkVersion());

  Serial.print("Connecting to "); Serial.print(ssid);
  WiFi.begin(ssid, password);
  WiFi.waitForConnectResult();
  Serial.print(", WiFi connected, IP address: "); Serial.println(WiFi.localIP());

  wiFiClient.setCACert(rootCA);
  wiFiClient.setCertificate(certificate_pem_crt);
  wiFiClient.setPrivateKey(private_pem_key);
}

unsigned long lastPublish;
int msgCount;

void loop() {

  pubSubCheckConnect();

   //If you need to increase buffer size, you need to change MQTT_MAX_PACKET_SIZE in PubSubClient.h
   char fakeData[128];

  float var1 =  random(55,77); //fake number range, adjust as you like
  float var2 =  random(77,99);
  sprintf(fakeData,  "{\"uptime\":%lu,\"temp\":%f,\"humid\":%f}", millis() / 1000, var1, var2);


  if (millis() - lastPublish > 10000) {
  boolean rc = pubSubClient.publish("outTopic", fakeData);
    Serial.print("Published, rc="); Serial.print( (rc ? "OK: " : "FAILED: ") );
    Serial.println(fakeData);
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
      pubSubClient.connect("ESPthingXXXX");
      delay(1000);
    }
    Serial.println(" connected");
    pubSubClient.subscribe("inTopic");
  }
  pubSubClient.loop();
}
