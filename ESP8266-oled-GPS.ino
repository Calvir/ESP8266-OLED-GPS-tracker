/*
 * GPS Tracker with OLED Display and Web Interface
 * 
 * This project creates a portable GPS tracker using an ESP8266 and NEO-6M GPS module.
 * It displays location data on an OLED screen and provides a web interface accessible
 * through a WiFi access point. Features include:
 * - Real-time GPS tracking
 * - OLED display showing coordinates, time, satellites, and speed
 * - Web interface with Google Maps integration
 * - Automatic daylight saving time adjustment for France
 * 
 * Hardware:
 * - NodeMCU ESP8266 V3 with 0.96" OLED display
 * - GY-NEO6MV2 GPS module
 * 
 * License: CC-BY-NC-SA
 * This work is licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0
 * You are free to:
 * - Share: copy and redistribute the material in any medium or format
 * - Adapt: remix, transform, and build upon the material
 * 
 * Under the following terms:
 * - Attribution: You must give appropriate credit
 * - NonCommercial: You may not use the material for commercial purposes
 * - ShareAlike: If you remix, transform, or build upon the material, you must distribute
 *   your contributions under the same license as the original
 * 
 * Author: Virgile Calvert
 * Date: December 2024
 */

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// OLED screen definitions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_SDA D5
#define OLED_SCL D6

// GPS definitions
#define GPS_RX D1
#define GPS_TX D4

// WiFi configuration
const char* ssid = "GPS_Tracker";      // WiFi network name
const char* password = "12345678";     // WiFi password

#define MAX_SRV_CLIENTS 2  // Maximum number of simultaneous clients

// Create objects
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
TinyGPSPlus gps;
SoftwareSerial gpsSerial(GPS_RX, GPS_TX);
ESP8266WebServer server(80);

// Function to determine if we are in daylight saving time
bool isDST(int day, int month, int dow) {
    // In France, DST starts on the last Sunday of March (at 2am)
    // and ends on the last Sunday of October (at 3am)
    if (month < 3 || month > 10) return false;
    if (month > 3 && month < 10) return true;
    int lastSunday = 31 - dow;  // dow = day of week (0 = Sunday)
    if (month == 3) return day >= lastSunday;
    if (month == 10) return day < lastSunday;
    return false;
}

// Function to get current UTC offset
int getUTCOffset(int day, int month, int dow) {
    return isDST(day, month, dow) ? 2 : 1;  // 2h in summer, 1h in winter
}

void setup() {
  Serial.begin(115200);
  Wire.begin(OLED_SDA, OLED_SCL);
  gpsSerial.begin(9600);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Failed to initialize SSD1306 display"));
    for(;;);
  }

  // WiFi access point configuration
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(IPAddress(192,168,4,1), IPAddress(192,168,4,1), IPAddress(255,255,255,0));
  WiFi.softAP(ssid, password, 1, 0, MAX_SRV_CLIENTS);  // Channel 1, no hidden SSID, max 2 clients
  
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("SSID: ");
  Serial.println(ssid);
  Serial.print("Access Point IP: ");
  Serial.println(myIP);

  // Web server routes configuration
  server.on("/", HTTP_GET, handleRoot);
  server.on("/data", HTTP_GET, handleData);
  server.on("/css", HTTP_GET, handleCSS);
  server.on("/ping", HTTP_GET, []() {
    server.send(200, "text/plain", "pong");
  });
  
  server.begin();
  Serial.println("HTTP server started");
  
  // Initial connection information display
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("GPS Tracker");
  display.println("------------");
  display.println("WIFI: GPS_Tracker");
  display.println("PASS: 12345678");
  display.println("------------");
  display.print("IP: ");
  display.println(myIP);
  display.display();
  delay(5000);
}

void loop() {
  static unsigned long lastCheck = 0;
  
  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());
  }
  
  static unsigned long lastDisplayUpdate = 0;
  if (millis() - lastDisplayUpdate >= 100) {
    updateDisplay();
    lastDisplayUpdate = millis();
  }
  
  server.handleClient();
  
  if (millis() - lastCheck >= 10000) {
    Serial.printf("Clients connectés: %d\n", WiFi.softAPgetStationNum());
    lastCheck = millis();
  }
  
  delay(1);
}

void handleRoot() {
  String html = R"html(
    <!DOCTYPE html>
    <html>
    <head>
        <title>GPS Tracker</title>
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <link rel="stylesheet" href="/css">
        <script>
            let retryCount = 0;
            const maxRetries = 3;
            
            async function checkConnection() {
                try {
                    const response = await fetch('/ping');
                    if (response.ok) {
                        retryCount = 0;
                        return true;
                    }
                } catch (e) {
                    console.error('Erreur de connexion:', e);
                }
                return false;
            }
            
            async function updateData() {
                try {
                    const response = await fetch('/data');
                    if (!response.ok) throw new Error('Erreur réseau');
                    const data = await response.json();
                    
                    document.getElementById('datetime').textContent = data.datetime;
                    document.getElementById('satellites').textContent = data.satellites;
                    document.getElementById('latitude').textContent = data.latitude;
                    document.getElementById('longitude').textContent = data.longitude;
                    document.getElementById('speed').textContent = data.speed + ' km/h';
                    
                    const mapsLink = document.getElementById('maps-link');
                    if (data.mapsLink) {
                        mapsLink.href = data.mapsLink;
                        mapsLink.style.display = 'inline';
                    } else {
                        mapsLink.style.display = 'none';
                    }
                    
                    retryCount = 0;
                } catch (error) {
                    console.error('Erreur:', error);
                    retryCount++;
                    if (retryCount >= maxRetries) {
                        document.getElementById('error-message').style.display = 'block';
                    }
                }
            }
            
            checkConnection().then(connected => {
                if (connected) {
                    updateData();
                    setInterval(updateData, 1000);
                }
            });
        </script>
    </head>
    <body>
        <h1>GPS Tracker</h1>
        <div id="error-message" style="display:none; color:red; text-align:center; margin:10px;">
            Problème de connexion. Essayez de recharger la page.
        </div>
        <div class="data-container">
            <p>Date/Heure: <span id="datetime" class="value">--</span></p>
            <p>Satellites: <span id="satellites" class="value">--</span></p>
            <p>Latitude: <span id="latitude" class="value">--</span></p>
            <p>Longitude: <span id="longitude" class="value">--</span></p>
            <p>Vitesse: <span id="speed" class="value">--</span></p>
            <p style="text-align: center; border-bottom: none;">
                <a id="maps-link" href="#" target="_blank">Voir sur Google Maps</a>
            </p>
        </div>
    </body>
    </html>
  )html";
  server.send(200, "text/html", html);
}

void handleData() {
  String datetime = "Non disponible";
  if (gps.date.isValid() && gps.time.isValid()) {
    int hour = gps.time.hour();
    int minute = gps.time.minute();
    int second = gps.time.second();
    
    int currentOffset = getUTCOffset(gps.date.day(), gps.date.month(), gps.date.day() % 7);
    hour = (hour + currentOffset) % 24;
    
    char buffer[30];
    sprintf(buffer, "%02d/%02d/%02d %02d:%02d:%02d", 
            gps.date.day(), gps.date.month(), gps.date.year() % 100,
            hour, minute, second);
    datetime = String(buffer);
  }

  String mapsLink = "";
  if (gps.location.isValid()) {
    mapsLink = "https://www.google.com/maps?q=" + 
               String(gps.location.lat(), 6) + "," + 
               String(gps.location.lng(), 6);
  }

  String json = "{";
  json += "\"datetime\":\"" + datetime + "\",";
  json += "\"satellites\":" + String(gps.satellites.value()) + ",";
  json += "\"latitude\":" + String(gps.location.isValid() ? gps.location.lat() : 0, 6) + ",";
  json += "\"longitude\":" + String(gps.location.isValid() ? gps.location.lng() : 0, 6) + ",";
  json += "\"speed\":" + String(gps.speed.isValid() ? gps.speed.kmph() : 0, 1) + ",";
  json += "\"mapsLink\":\"" + mapsLink + "\"";
  json += "}";
  
  server.send(200, "application/json", json);
}

void handleCSS() {
  String css = R"css(
    body { 
        font-family: Arial, sans-serif; 
        margin: 20px;
        background-color: #f5f5f5;
        color: #333;
    }
    
    h1 {
        color: #2c3e50;
        text-align: center;
        margin-bottom: 30px;
    }
    
    .data-container { 
        background: white; 
        padding: 20px;
        border-radius: 10px;
        box-shadow: 0 2px 5px rgba(0,0,0,0.1);
        max-width: 600px;
        margin: 0 auto;
    }
    
    p {
        margin: 15px 0;
        padding: 10px;
        border-bottom: 1px solid #eee;
    }
    
    .value { 
        font-weight: bold;
        color: #2980b9;
        float: right;
    }

    #maps-link {
        float: none;
        color: #2980b9;
        text-decoration: none;
        padding: 5px 10px;
        border: 1px solid #2980b9;
        border-radius: 5px;
    }
    
    #maps-link:hover {
        background-color: #2980b9;
        color: white;
    }
    
    @media (max-width: 480px) {
        body { 
            margin: 10px;
        }
        .data-container {
            padding: 15px;
        }
    }
  )css";
  
  server.send(200, "text/css", css);
}

void updateDisplay() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  
  // Display Date and Time with offset
  display.setCursor(0,2);
  if (gps.date.isValid() && gps.time.isValid()) {
    int hour = gps.time.hour();
    int minute = gps.time.minute();
    int second = gps.time.second();
    
    int currentOffset = getUTCOffset(gps.date.day(), gps.date.month(), gps.date.day() % 7);
    hour = (hour + currentOffset) % 24;
    
    char dateStr[11];
    sprintf(dateStr, "%02d/%02d/%02d", gps.date.day(), gps.date.month(), gps.date.year() % 100);
    display.print(dateStr);
    display.print(" ");
    char timeStr[9];
    sprintf(timeStr, "%02d:%02d:%02d", hour, minute, second);
    display.println(timeStr);
  } else {
    display.println("Waiting signal...");
  }
  
  // Separation line
  display.drawLine(0, 12, SCREEN_WIDTH, 12, WHITE);
  
  // Display Satellites and IP on the same line
  display.setCursor(0,16);
  display.print("SAT: ");
  display.print(gps.satellites.value());
  
  // IP on the right
  String ip = WiFi.softAPIP().toString();
  int16_t ipWidth = ip.length() * 6;
  display.setCursor(SCREEN_WIDTH - ipWidth, 16);
  display.print(ip);
  
  // Display Coordinates
  display.setCursor(0,28);
  if (gps.location.isValid()) {
    display.print("LAT: ");
    display.println(gps.location.lat(), 4);
    display.setCursor(0,38);
    display.print("LON: ");
    display.println(gps.location.lng(), 4);
  } else {
    display.println("Position: Searching");
  }
  
  // Separation line before speed
  display.drawLine(0, 46, SCREEN_WIDTH, 46, WHITE);
  
  // Display Speed
  display.setCursor(0,49);
  display.print("SPD: ");
  if (gps.speed.isValid()) {
    display.print(gps.speed.kmph(), 1);
    display.print(" km/h");
  } else {
    display.print("--.- km/h");
  }
  
  display.display();
} 