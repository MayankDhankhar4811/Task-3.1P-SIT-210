#include <DHT.h>  // included library to work with DHT11 temperature sensor
#include <WiFiNINA.h> // included wifi library to establish wireless connection
// WiFi credentials
char ssid[] = "Redmi";
char pass[] = "mayank2005";
// ThingSpeak channel settings
char server[] = "api.thingspeak.com";
String apiKey = "1UIDG4MJ7IAN241B"; // channel API ID for sending data to thingspeak server
// Pin configuration for DHT11 sensor
#define DHTPIN 2  // defining a pin for dht sensor
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);//This command initializes a DHT sensor object named 'dht' with specified pin and sensor type for communication in an Arduino program.
// Initialize the WiFi client
WiFiClient client;
void setup() {
  // Start serial communication
  Serial.begin(9600); // Initialising serial communication at baud rate of 9600 bits per second b/w serial monitor and sensor 
  // Connect to WiFi
  while (WiFi.status() != WL_CONNECTED) {   // Checking WiFi status 
    Serial.print("Establishing connection with WiFi...");
    WiFi.begin(ssid, pass);//initiates a connection attempt to a Wi-Fi network using the provided 'ssid' (network name) and 'pass' (password) 
    delay(10000); // Delay for 10 seconds
  }
  Serial.println("Successfully established connection with WiFi");
  dht.begin();
}
void loop() {
  // Read temperature and humidity from DHT11 sensor
  float temperature = dht.readTemperature();
  // Check if any reads failed and exit early (to try again)
  if (isnan(temperature)) {
    Serial.println("DHT sensor initialisation failed");
    delay(2000);
    return;
  }
  // Prepare the data to send to ThingSpeak
  String data = "api_key=" + apiKey;
  data += "&field1=" + String(temperature);
  // Make an HTTP POST request to ThingSpeak
  if (client.connect(server, 80)) {
    client.print("POST /update HTTP/1.1\r\n");
    client.print("Host: " + String(server) + "\r\n");
    client.print("Content-Type: application/x-www-form-urlencoded\r\n");
    client.print("Content-Length: " + String(data.length()) + "\r\n");
    client.print("\r\n");
    client.print(data);
    Serial.println("Connection established to ThingSpeak");
    Serial.println("Data sent to ThingSpeak!");
  } else {
    Serial.println("Connection to ThingSpeak failed!");
    Serial.println("Data transfer failed....");
  }
  // Wait for response and close the connection
  while (client.connected()) {  // Checking if client/user is available for transfer of data
    if (client.available()) {
      char c = client.read();
      Serial.print(c);
    }
  }
  client.stop();
  // Wait 15 seconds before sending the next set of data
  delay(15000); 