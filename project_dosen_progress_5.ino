#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include  <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3f, 16, 2);
//pin scl : A5
//pin sda : A4

#define echoPin 8       //pin 8 arduino untuk echo pin sensor ultrasonic 
#define trigPin 9       //pin 9 arduino untuk trig pin sensor ultrasonic

#define batasAtas 24    //nilai batas jarak dalam satuan cm
#define batasBawah 19   //nilai batas jarak dalam satuan cm

#define batasAtas1 18   //nilai batas jarak dalam satuan cm
#define batasBawah1 13  //nilai batas jarak dalam satuan cm

#define batasAtas2 12   //nilai batas jarak dalam satuan cm
#define batasBawah2 7   //nilai batas jarak dalam satuan cm
long duration, ultraSensor;
int distance;  //membuuat devinisi tipe data yang digunakan
float volumenull, volume, volumeL;
char voll[50];
String vol_str;
using namespace std;

// Update these with values suitable for your network.
byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };

IPAddress dnServer(10, 13, 10, 13);
IPAddress gateway(10, 33, 107, 254);
//IPAddress gateway(10,39,52,254);
IPAddress subnet(255, 255, 255, 0);
IPAddress server(10, 33, 109, 82);
IPAddress ip(10, 33, 107, 169);
//IPAddress ip(10,39,52,112);

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

EthernetClient ethClient;
PubSubClient client(ethClient);

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduinoClient")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void setup()
{
  lcd.begin();
  Serial.begin (4800);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  digitalWrite(7, LOW);

  client.setServer(server, 1883);
  client.setCallback(callback);

  Ethernet.begin(mac, ip, dnServer, gateway, subnet);
  //Ethernet.begin(mac, ip);
  // Allow the hardware to sort itself out
  delay(1500);
}

void loop()
{
  sensorUltrasonic(trigPin, echoPin);
  ultraSensor = distance; //memberikan nilai ultrasensor dengan nilai yang ada pada distance
  vol_str = String(volumeL);
  vol_str.toCharArray(voll, vol_str.length() + 1);
  cetakjarak();
  volumenull = 3.14 * distance * 38 * 38; //rumus menghitung volume ruang kosong tabung yang tidak terisi fluida dengan satuan cm3
  volume = 589440.8 - volumenull; // rumus menghitung nilai volume fluida dengan satuan cm3
  volumeL = volume / 1000; // rumus mengubah volume cm3 menjadi Liter
  cetakVolume();
  delay(300);
  //delay(100);
  if (ultraSensor >= 25 )
  {
    satubar();
  }
  else if (ultraSensor <= batasAtas && ultraSensor >= batasBawah)
  {
    duabar();
  }
  else if (ultraSensor <= batasAtas1 && ultraSensor >= batasBawah1)
  {
    tigabar();
  }
  else if (ultraSensor <= batasAtas2 && ultraSensor >= batasBawah2)
  {
    empatbar();
  }
  else if (ultraSensor <= 6 )
  {
    penuh();
  }

  if (!client.connected()) {
    reconnect();
  } else {
    client.publish("volume", voll);
  }
  client.loop();
}

void sensorUltrasonic(int pinTrigger, int pinEcho)
{
  digitalWrite(pinTrigger, LOW);
  delayMicroseconds(2);
  digitalWrite(pinTrigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinTrigger, LOW);
  duration = pulseIn(pinEcho, HIGH);
  distance = (duration / 2) / 29.1; //rumus untuk menghitung jarak dengan cara membagi dua pada bagian durasi untuk menghilangkan nilai patul kembali dari gelombang ultrasonic lalu dibagi lagi dengan nilai 29.1 yaitu nilai konversi rambat gelombang menjadi cm
  delay(500);
}


