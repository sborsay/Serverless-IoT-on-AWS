/*********
Cloudboard by Stephen Borsay
AWS Connection
For AWS WebSockets Serverless Real-Time IoT Dashboarding
*********/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include "images.h" //bmp picture files

//begin AWS WiFi portion
#include <WiFiClientSecure.h>
#include <PubSubClient.h> // install with Library Manager, I used v2.6.0

const char* ssid = "<Your-WiFi-Network-Name>";
const char* password = "<Your-WiFi-PAssword>";

const char* awsEndpoint = "iknowkungfu-ats.iot.us-east-1.amazonaws.com"; //AWS IoT Core--> Settings (Device data endpoint)

// xxxxxxxxxx-certificate.pem.crt
static const char certificate_pem_crt[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g
SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTIyMDIxMjIzMDQy
NVoXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0
ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAOhy3rpzRWRrnNwoRRSZ
WQu74bD26FDBNTp1K3cejOK7A+HoTixdxYxJ0hv6Cv/9SEbfoBasP59oiTCUhgtX
NYWfi5iYOoGJQbDycKhmWvcBOZqi6m4l+2w5LjOUOqjoo1e3+iEmz+kZb7QFBN2j
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
kyU0Gk1rS8acqE3ZaG0vT7Z0+mAwQibizV8ExrP7JmmY6ME0AzrdqhN/pcN3OZ/a
Iy/vxCiRAkVYCQhj/qWPz+qV9KK2teesfeJCiqUzHMfFaWq/919GzfdZeopAzAkL
WS8CAwEAAaNgMF4wHwYDVR0jBBgwFoAUfoVNZfc8zl7iSbOrFGqHVtNfB7UwHQYD
VR0OBBYEFKwtijpUoUqzg3otRKbqCg068a6kMAwGA1UdEwEB/wQCMAAwDgYDVR0P
AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQCVspP/8JhHk3Tn7bhT2uojxsmS
idIjCYQwOR2ESYAMyvYnyKDdzJ11G+mRk0htSV75U0fLu1Nz7UYv3znrTXCdbPN7
/YOEK+O1B05ldvWshGI4SUmTBeLEuSOcyKnlW6O5BVZlpeb8IqkF/gWo67Hn0miJ
TKkWaN0sRO83ybMbI3HbVXMIfymn2FOZ806oa010HwRwaFccfz/s7gXfFtd4JCWd
tMhwrYshIylUdFPOZ/T+7rsMpd3jWNWRht5/XbC3Ai6ZdGHBqXMEW3cOUeOHpkBX
VHv+/MIcw7KNf60NQFzWG5wMkCgmvmeQkBjSSOOxFDIxB0MeiLuSL/+8OCgx
-----END CERTIFICATE-----
)KEY";

// xxxxxxxxxx-private.pem.key
static const char private_pem_key[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
LF3FjEnSG/oK//1IRt+gFqw/n2iJMJSGC1c1hZ+LmJg6gYlBsPJwqGZa9wE5mqLq
biX7bDkuM5Q6qOijV7f6ISbP6RlvtAUE3aPNqKfVFaA3AD4CNytyWydjuDjPL9mf
00ESoPxgOROoOhkXTeA9vvL9RI8rFnLesjyTJTQaTWtLxpyoTdlobS9PtnT6YDBC
JuLNXwTGs/smaZjowTQDOt2qE3+lw3c5n9ojL+/EKJECRVgJCGP+pY/P6pX0ora1
56x94kKKpTMcx8Vpar/3X0bN91l6ikDMCQtZLwIDAQABAoIBACedVAoQlRXtJRl8
H3HIRJ20Ul1XZvZERy8EQvkVEsudNactcQ0smfszpYQxE+jWoJe7isvyZRFkvmSy
GzoZjbyNGTxTTT1ASBrA5qQ8dWo9IjURaMSG4pdqE06lB24734KWjyFRPQOr2xah
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
M1uGcy6rv02X/0FC6wXTWgaELnuabHF7hLwDweEsDPBicVnWkDUoLvlBYYbNAEQ3
qH3fT9O6I68y3ZYX3piD5tRDFZ7Mjh4Z6NbA3b01Vs8YXfc2zumxJ1GGzCTTc1wa
3iCI5SkCgYEA9+m8M27nmAPfNlU7OQW4SmZvam5I9jjzj4YefWmg2EHSKQX6eNio
jElGybYiFTBKB0hSLEDg/f+MZnVCStrlzBHI4W8Gc/8ltOK6AXqrjN673Gu/y+C6
y9kwRA9JicMohe1a8FlyLHcTx/LCSQ0BQ9noXzbCU64/hS0rOjprr3UCgYEA8Af+
8+Fqf40kGUO8SGNEFti+lVS5b8dpnmyMx4nj+3+iQhy1uggl1zF3L/0derTnrTXk
9O0rgUi8DtI1EgKiSYwWwiN7GTIFeh8P2szrWFbL0Dx454Y0kVzk8WLpVXHYucIf
5431ov3H6lzUDINw61AopC0NmJ483LvyhMdtFZMCgYEAnnrt05dXN4dqHsqXhFlS
uUUIfmon/DbmBF8BiLyIlRZmqdDnkG84OA3z0ai72v22pTcnSKDk85p0f18AVOfg
NPc9ilc2MdrumNbDf4SSvAf64eqDfmqYHw4lW6zGHuoCMrNMCg+z/BbXL1/RsW89
2sknUg6oZWQjuRfp5VxJUK0CgYEAlfKYzef67NXSQEo1XA9PIOqkHIvCEFdF3uc3
BwxYNOvmBGEFEcM3SHipWXxJJzfOJ0Gs12k2LVyvxp6A8tBca4+tQ/iAVRwPmdu/
vtMXDqeiSWMNsK0BN/INP9Knq24kkt73qWLgmcLqWblF6s9OAmKgEPfoL3hi0w5C
kbo3pisCgYEA2IG2F6+geLdLPO8IQxnHL1bBs7TGQDru25Hk1o8fG72VSuZzViQ/
x4xwm2x8hI6HUjZ315P39emB5382XWnhsI9LkuJufna0KtxoiBocWDFHxA6GIOz/
6U0+bpP3rpPJlP07/LTQdEx/tbYBsKsTKhSZPtkuH4mLILuh93GBVvo=
-----END RSA PRIVATE KEY-----
)KEY";


/* root CA found at:
  https://www.amazontrust.com/repository/AmazonRootCA1.pem
  fungible between regions.  Open cert, free for public consumption
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


unsigned long lastPublish;
int msgCount;
//end AWS wifi portion

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);  //taken out deplicate from helper file

const int sensorPin = 33; //GL5528 phtoresistor on GPIO23
int lightLevel;
const int potPin = 32;     //pin with 10k trimmer potentiometer
int potValue;

#define DHTPIN 15     // Digital pin connected to the DHT sensor

// Uncomment the type of sensor in use:
#define DHTTYPE    DHT11     // DHT 11
//#define DHTTYPE    DHT22     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);


void setup() {


    //enter picture portion
    //show picture for a few seconds
  // initialize with the I2C addr 0x3C
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  //cb_by_sdb

  // Clear the buffer.
  display.clearDisplay();

  // Display bitmap
  display.drawBitmap(0, 0,  cb_by_sdb, 128, 64, WHITE);
  delay(3000);
  display.display();
  //end picture portion 

  //end cb sdb

  // Clear the buffer.
  display.clearDisplay();

  // Display bitmap
  display.drawBitmap(0, 0,  AWS_Cloud_Logo, 128, 64, WHITE);
  delay(3000);
  display.display();
  //end picture portion 

  display.clearDisplay();

  
  //begin aws WiFI setup
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
  //end AWS WiFi setup

//begin DHT and OLED  setup
  dht.begin();


  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();
  display.setTextColor(WHITE);

  //end begin DHT and OLED  setup
}


void loop() {
  pubSubCheckConnect();
 
  delay(2000);
  

   lightLevel = analogRead(sensorPin);
   Serial.println((String)"The Photoresistor light level is: " + lightLevel);

   potValue = analogRead(potPin);
   Serial.println((String)"The Potentiometer Trimmer resistance is: " + potValue);

  //read temperature and humidity
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
  }

   Serial.println((String)"The Temperature value in ° Celcius is: " + t);
   Serial.println((String)"The humidity index % is: " + h);

  //clear display
  display.clearDisplay();

  // display temperature
  display.setTextSize(1);
  display.setCursor(0,0); //display.setCursor(x,y) – set the coordinates to start writing text
  display.print("Temp:");
  display.setTextSize(2);
  display.setCursor(0,10);
  display.print(((t * 9/5) + 32),1);  // (°C × 9/5) + 32 = °F
  //display.print(" ");
  //display.setTextSize(1);
  //display.cp437(true);
  //display.write(167);
  //display.setTextSize(1);
 // display.print("C");
  
  // display humidity
  display.setTextSize(1);
  display.setCursor(0, 35);
  display.print("Humidity:");
  display.setTextSize(2);
  display.setCursor(0, 45);
  display.print(h,1);
 //display.print("%"); 

//display resistence trimmer
  display.setTextSize(1);
  display.setCursor(70, 0);
  display.print("Resistance:");
  display.setTextSize(2);
  display.setCursor(70, 10);
  display.print(potValue);  //normalize 24 turn 10k trimmer to keep magigniture in line


  // display Photoresistor
  display.setTextSize(1);
  display.setCursor(70,35);
  display.print("Lux Level:");
  display.setTextSize(2);
  display.setCursor(70, 45);
  display.print(lightLevel/10); //normalize lux to same magnitude as enviro readings 
  
  display.display();   //end display portion
  
    //If you need to increase buffer size, change MQTT_MAX_PACKET_SIZE in PubSubClient.h
   char cloudboardData[128];
   sprintf(cloudboardData,  "{\"temperature\":%.0f,\"humidity\":%.0f, \"resistance\":%lu, \"lux\":%lu}", t, h, potValue, lightLevel/10);

 if (millis() - lastPublish > 5000) {
  boolean rc = pubSubClient.publish("iot/ESP32WROVER1", cloudboardData);
    Serial.print("Published, rc="); Serial.print( (rc ? "OK: " : "FAILED: ") );
    Serial.println(cloudboardData);
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
      pubSubClient.connect("ESPDeviceUniqueIdentifier"); //this is you clientID
      delay(1000);
    }
    Serial.println(" connected");
    pubSubClient.subscribe("inTopic");
  }
  pubSubClient.loop();
}
