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
 * https://github.com/earlephilhower/bearssl-esp8266/tree/master/src
 * Sketch Modified by Stephen Borsay for www.udemy.com/course/exploring-aws-iot/
 * https://github.com/sborsay
 * Add in EOF certificate delimiter
 * Add in Char buffer utilizing sprintf to dispatch JSON data to AWS IoT Core
 * First 9 chars of certs obfusicated, use your own, but you can share root CA / x.509 until revoked
 */

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "<Your-WiFi-Network-Here>";
const char* password = "<Your-Password-Here>";

const char* awsEndpoint = "<YOURACCOUNTID-ats.iot.REGION.amazonaws.com>"; //AWS IoT Core--> Settings (Device data endpoint)

// For the two certificate strings below paste in the text of your AWS 
// device certificate and private key:

// xxxxxxxxxx-certificate.pem.crt
static const char certificatePemCrt[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g
SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTIyMDIxMjIzMDQy
NVoXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0
ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAOhy3rpzRWRrnNwoRRSZ
WQu74bD26FDBNTp1K3cejOK7A+HoTixdxYxJ0hv6Cv/9SEbfoBasP59oiTCUhgtX
NYWfi5iYOoGJQbDycKhmWvcBOZqi6m4l+2w5LjOUOqjoo1e3+iEmz+kZb7QFBN2j
zain1RWgNwA+AjcrclsnY7g4zy/Zn9NBEqD8YDkTqDoZF03gPb7y/USPKxZy3rI8
kyU0Gk1rS8acqXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX6ME0AzrdqhN/pcN3OZ/a
Iy/vxCiRAkVYCQhj/qWPz+qV9KK2teesfeJCiqUzHMfFaWq/919GzfdZeopAzAkL
WS8CAwEAAaNgMF4wHwYDVR0jBBgwFoAUfoVNZfc8zl7iSbOrFGqHVtNfB7UwHQYD
VR0OBBYEFXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXR0P
AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQCVspP/8JhHk3Tn7bhT2uojxsmS
idIjCYQwOR2ESYAMyvYnyKDdzJ11G+mRk0htSV75U0fLu1Nz7UYv3znrTXCdbPN7
/YOEK+O1B05ldvWshGI4SUmTBeLEuSOcyKnlW6O5BVZlpeb8IqkF/gWo67Hn0miJ
TKkWaN0sRO83ybMbI3HbVXMIfymn2FOZ806oa010HwRwaFccfz/s7gXfFtd4JCWd
tMhwrYshIylUdFPOZ/T+7rsMpd3jWNWRht5/XbC3Ai6ZdGHBqXMEW3cOUeOHpkBX
VHv+/MIcw7KNf60NQFzWG5wMkCgmvmeQkBjSSOOxFDIxB0MeiLuSL/+8OCgx
-----END CERTIFICATE-----
)EOF";

// xxxxxxxxxx-private.pem.key
static const char privatePemKey[] PROGMEM = R"EOF(
-----BEGIN RSA PRIVATE KEY-----
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
LF3FjEnSG/oK//1IRt+gFqw/n2iJMJSGC1c1hZ+LmJg6gYlBsPJwqGZa9wE5mqLq
biX7bDkuM5Q6qOijV7f6ISbP6RlvtAUE3aPNqKfVFaA3AD4CNytyWydjuDjPL9mf
00ESoPxgOROoOhkXTeA9vvL9RI8rFnLesjyTJTQaTWtLxpyoTdlobS9PtnT6YDBC
JuLNXwTGs/smaZjowTQDOt2qE3+lw3c5n9ojL+/EKJECRVgJCGP+pY/P6pX0ora1
56x94kKKpTMcx8Vpar/3X0bN91l6ikDMCQtZLwIDAQABAoIBACedVAoQlRXtJRl8
H3HIRJ20Ul1XZvZERy8EQvkVEsudNactcQ0smfszpYQxE+jWoJe7isvyZRFkvmSy
GzoZjbyNGTxTTT1ASBrA5qQ8dWo9IjURaMSG4pdqE06lB24734KWjyFRPQOr2xah
+FdRkD4bBDygRl9bMgN2+xE/XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
qH3fT9O6I68y3ZYX3piD5tRDFZ7Mjh4Z6NbA3b01Vs8YXfc2zumxJ1GGzCTTc1wa
3iCI5SkCgYEA9+m8M27nmAPfNlU7OQW4SmZvam5I9jjzj4YefWmg2EHSKQX6eNio
jElGybYiFTBKB0hSLEDg/f+MZnVCStrlzBHI4W8Gc/8ltOK6AXqrjN673Gu/y+C6
y9kwRA9JicMohe1a8FlyLHcTx/LCSQ0BQ9noXzbCU64/hS0rOjprr3UCgYEA8Af+
8+Fqf40kGUO8SGNEFti+lVS5b8dpnmyMx4nj+3+iQhy1uggl1zF3L/0derTnrTXk
9O0rgUi8DtI1EgKiSYwWwiN7GTIFeh8P2szrWFbL0Dx454Y0kVzk8WLpVXHYucIf
5431ov3HXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXdqHsqXhFlS
uUUIfmon/DbmBF8BiLyIlRZmqdDnkG84OA3z0ai72v22pTcnSKDk85p0f18AVOfg
NPc9ilc2MdrumNbDf4SSvAf64eqDfmqYHw4lW6zGHuoCMrNMCg+z/BbXL1/RsW89
2sknUg6oZWQjuRfp5VxJUK0CgYEAlfKYzef67NXSQEo1XA9PIOqkHIvCEFdF3uc3
BwxYNOvmBGEFEcM3SHipWXxJJzfOJ0Gs12k2LVyvxp6A8tBca4+tQ/iAVRwPmdu/
vtMXDqeiSWMNsK0BN/INP9Knq24kkt73qWLgmcLqWblF6s9OAmKgEPfoL3hi0w5C
kbo3pisCgYEA2IG2F6+geLdLPO8IQxnHL1bBs7TGQDru25Hk1o8fG72VSuZzViQ/
x4xwm2x8hI6HUjZ315P39emB5382XWnhsI9LkuJufna0KtxoiBocWDFHxA6GIOz/
6U0+bpP3rpPJlP07/LTQdEx/tbYBsKsTKhSZPtkuH4mLILuh93GBVvo=
-----END RSA PRIVATE KEY-----
)EOF";

// This is the AWS IoT CA Certificate from: 
// https://docs.aws.amazon.com/iot/latest/developerguide/managing-device-certs.html#server-authentication
// This one in here is the 'RSA 2048 bit key: Amazon Root CA 1' which is valid 
// until January 16, 2038 so unless it gets revoked you can leave this as is:
static const char caPemCrt[] PROGMEM = R"EOF(
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

BearSSL::X509List client_crt(certificatePemCrt);
BearSSL::PrivateKey client_key(privatePemKey);
BearSSL::X509List rootCert(caPemCrt);

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

  wiFiClient.setClientRSACert(&client_crt, &client_key);
  wiFiClient.setTrustAnchors(&rootCert);
}

unsigned long lastPublish;
int msgCount;

void loop() {

  pubSubCheckConnect();

  //If you need to increase buffer size, change MQTT_MAX_PACKET_SIZE in PubSubClient.h
   char fakeData[128];

  float var1 =  random(0,120); //fake number range, adjust as you like
  float var2 =  random(0,100);
  sprintf(fakeData,  "{\"uptime\":%lu,\"temperature\":%f,\"humidity\":%f}", millis() / 1000, var1, var2);
  
  if (millis() - lastPublish > 5000) {
  boolean rc = pubSubClient.publish("outTopic", fakeData);
    Serial.print("Published, rc=");
    Serial.print( (rc ? "OK: " : "FAILED: ") );
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
      pubSubClient.connect("ESPthing");
    }
    Serial.println(" connected");
    pubSubClient.subscribe("inTopic");
  }
  pubSubClient.loop();
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
