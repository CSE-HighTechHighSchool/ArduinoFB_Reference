#include <SPI.h>
#include <WiFiNINA.h>
#include <ArduinoJson.h>

#define echoPin 3 // attach pin D3 Arduino to pin Echo of HC-SR04
#define trigPin 2 //attach pin D2 Arduino to pin Trig of HC-SR04

long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement


char ssid[] = "****";        // your network SSID (name)
char pass[] = "****";    // your network password 
int keyIndex = 0;

int status = WL_IDLE_STATUS;

// Initialize the Wifi client library

WiFiClient client;

// server address:
//char server[] = "jsonplaceholder.typicode.com"; // for public domain server
IPAddress server(***,***,**,***); // for localhost server (server IP address can be found with ipconfig or ifconfig)

unsigned long lastConnectionTime = 0;

const unsigned long postingInterval = 10L * 50L; // delay between updates, in milliseconds (10L * 50L is around 1 second between requests)

void setup() {
  
  Serial.begin(9600); // Start serial monitor

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT

  while (!Serial) {

    ; // wait for serial port to connect. Needed for native USB port only

  }

  // check for the WiFi module:

  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!"); // don't continue
    while (true);
  }

  // check if firmware is outdated
  String fv = WiFi.firmwareVersion(); 
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid); // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    delay(1000); // wait 1 second for connection
  }

  printWifiStatus(); // you're connected now, so print out the status
}

void loop() {

  StaticJsonDocument<200> doc;

  // if there's incoming data from the net connection, append each character to a variable
  String response = "";
  while (client.available()) {
    char c = client.read();
    response += (c);
  }

  // print out non-empty responses to serial monitor
  if (response != "") {
    Serial.println(response);
  }
  

  // repeat request after around 1 second
  if (millis() - lastConnectionTime > postingInterval) {
    httpRequest();
  }

}

// this method makes a HTTP connection to the server:
void httpRequest() {

  // close any connection before send a new request to free the socket
  client.stop();

  // get ultrasonic sensor distance
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2; // convert echo time to centimeters

  // if there's a successful connection:
  if (client.connect(server, 5000)) {
    Serial.println("connecting...");

    // send the HTTP GET request with the distance as a parameter:
    String request = "GET /?distance=" + String(distance) + " HTTP/1.1";
    client.println(request);

    // set the host as server IP address
    client.println("Host: ****");

    // other request properties
    client.println("User-Agent: ArduinoWiFi/1.1");
    client.println("Connection: close");
    client.println();

    // note the time that the connection was made:
    lastConnectionTime = millis();
  } else {
    Serial.println("connection failed"); // couldn't make a connection
  }
}

// connect to wifi network and display status
void printWifiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  IPAddress ip = WiFi.localIP(); // your board's IP on the network
  Serial.print("IP Address: ");
  Serial.println(ip);
  long rssi = WiFi.RSSI(); // received signal strength
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
