# ESP8266-OLED-GPS-tracker
# GPS Tracker with ESP8266 and OLED Screen

## Description
This project is a portable GPS tracking system using an ESP8266 and a NEO-6M GPS module. It displays position information on an OLED screen and offers a web interface accessible via WiFi.

## Required Materials
- NodeMCU ESP8266 V3 with 0.96-inch OLED screen (CH340)
- GY-NEO6MV2 GPS Module
- Connection wires
- USB Power Supply (a power bank is recommended for portability)

## Connections
### GPS Module
- TX → S1 (GPIO5) on the ESP8266
- RX → D4 (GPIO2) on the ESP8266
- VCC → 3.3V
- GND → GND

### OLED Screen (integrated in the NodeMCU)
- SDA → D5
- SCL → D6

## Software Installation
1. **Arduino IDE Setup**
   - Download the Arduino IDE from [arduino.cc](https://www.arduino.cc/en/software)
   - Add ESP8266 support:
     - In Preferences, add the URL: `https://arduino.esp8266.com/stable/package_esp8266com_index.json`
     - Install "ESP8266" via the Board Manager

2. **Required Libraries**
   - Install via the Library Manager:
     - `Adafruit GFX Library`
     - `Adafruit SSD1306`
     - `TinyGPSPlus`

3. **Board Configuration**
   - Select "NodeMCU 1.0 (ESP-12E Module)"
   - Speed: 115200 baud

## Features
### OLED Display
- Date and time (automatic daylight saving time adjustment)
- Number of satellites
- GPS coordinates (latitude/longitude)
- Speed in km/h
- WiFi access point IP address

### Web Interface
- Accessible via WiFi access point
- Real-time GPS data display
- Direct link to Google Maps
- Responsive interface (mobile/desktop)

## Usage
1. **Startup**
   - Power the ESP8266 via USB
   - Wait for initialization (startup screen)
   - Place it outdoors or near a window for GPS reception

2. **WiFi Connection**
   - SSID: `GPS_Tracker`
   - Password: `12345678`
   - Web Address: `http://192.168.4.1`

3. **Initial GPS Acquisition**
   - Cold start may take 1-5 minutes
   - Wait until at least 3 satellites are locked for a valid position

## Important Notes
- GPS accuracy depends on the number of visible satellites
- The web interface is limited to 2 simultaneous connections
- On mobile, use the URL `http://192.168.4.1` directly
- The ESP8266 battery lasts approximately X hours (to be tested based on usage)

## Troubleshooting
1. **No GPS Signal**
   - Ensure you are outdoors or near a window
   - Wait a few minutes for initial acquisition
   - Check TX/RX connections

2. **WiFi Issues**
   - Ensure you are within range of the signal
   - Try reconnecting to WiFi
   - Restart the ESP8266 if necessary

3. **Display Errors**
   - Check power supply
   - Restart the system

## Possible Improvements
- Add a LiPo battery
- 3D printed case
- Route recording
- Power-saving mode
- Customizable WiFi settings

## License
This project is licensed under Creative Commons CC-BY-NC-SA. This means:
- **Attribution (BY)**: You must credit the original author.
- **Non Commercial (NC)**: Use is restricted to non-commercial purposes.
- **Share Alike (SA)**: If you modify or transform this project, you must distribute it under the same license.

## Auteur
Virgile Calvert - Décembre 2024

## Version
1.0.0 - December 2024

---

# GPS Tracker avec ESP8266 et Écran OLED

## Description
Ce projet est un système de tracking GPS portable utilisant un ESP8266 et un module GPS NEO-6M. Il affiche les informations de position sur un écran OLED et propose une interface web accessible via WiFi.

## Matériel Nécessaire
- NodeMCU ESP8266 V3 avec écran OLED 0.96 pouces (CH340)
- Module GPS GY-NEO6MV2
- Câbles de connexion
- Alimentation USB (power bank recommandé pour la portabilité)

## Connexions
### Module GPS
- TX → S1 (GPIO5) de l'ESP8266
- RX → D4 (GPIO2) de l'ESP8266
- VCC → 3.3V
- GND → GND

### Écran OLED (intégré au NodeMCU)
- SDA → D5
- SCL → D6

## Installation du Logiciel
1. **Configuration de l'IDE Arduino**
   - Installer l'IDE Arduino depuis [arduino.cc](https://www.arduino.cc/en/software)
   - Ajouter le support ESP8266 :
     - Dans Préférences, ajouter l'URL : `https://arduino.esp8266.com/stable/package_esp8266com_index.json`
     - Installer "ESP8266" via le Gestionnaire de cartes

2. **Bibliothèques Requises**
   - Installer via le Gestionnaire de Bibliothèques :
     - `Adafruit GFX Library`
     - `Adafruit SSD1306`
     - `TinyGPSPlus`

3. **Configuration de la Carte**
   - Sélectionner "NodeMCU 1.0 (ESP-12E Module)"
   - Vitesse : 115200 bauds

## Fonctionnalités
### Affichage OLED
- Date et heure (ajustement automatique heure été/hiver)
- Nombre de satellites
- Coordonnées GPS (latitude/longitude)
- Vitesse en km/h
- Adresse IP du point d'accès WiFi

### Interface Web
- Accessible via le point d'accès WiFi
- Affichage en temps réel des données GPS
- Lien direct vers Google Maps
- Interface responsive (mobile/desktop)

## Utilisation
1. **Démarrage**
   - Alimenter l'ESP8266 via USB
   - Attendre l'initialisation (écran de démarrage)
   - Placer à l'extérieur ou près d'une fenêtre pour la réception GPS

2. **Connexion WiFi**
   - SSID : `GPS_Tracker`
   - Mot de passe : `12345678`
   - Adresse web : `http://192.168.4.1`

3. **Première Acquisition GPS**
   - Le démarrage à froid peut prendre 1-5 minutes
   - Attendre d'avoir au moins 3 satellites pour une position valide

## Notes Importantes
- La précision GPS dépend du nombre de satellites visibles
- L'interface web est limitée à 2 connexions simultanées
- Sur mobile, utiliser directement l'URL `http://192.168.4.1`
- La batterie de l'ESP8266 dure environ X heures (à tester selon votre utilisation)

## Dépannage
1. **Pas de Signal GPS**
   - Vérifier que vous êtes à l'extérieur ou près d'une fenêtre
   - Attendre quelques minutes pour l'acquisition initiale
   - Vérifier les connexions TX/RX

2. **Problèmes de WiFi**
   - Vérifier que vous êtes à portée du signal
   - Essayer de reconnecter le WiFi
   - Redémarrer l'ESP8266 si nécessaire

3. **Erreurs d'Affichage**
   - Vérifier l'alimentation
   - Redémarrer le système

## Améliorations Possibles
- Ajout d'une batterie LiPo
- Boîtier imprimé en 3D
- Enregistrement du trajet
- Mode économie d'énergie
- Configuration WiFi personnalisable

## Licence
Ce projet est sous licence Creative Commons CC-BY-NC-SA. Cela signifie :
- **Attribution (BY)** : Vous devez créditer l'auteur d'origine.
- **Non Commercial (NC)** : L'utilisation est réservée à des fins non commerciales.
- **Share Alike (SA)** : Si vous modifiez ou transformez ce projet, vous devez le distribuer sous la même licence.

## Auteur
Virgile Calvert - Décembre 2024

## Version
1.0.0 - Décembre 2024
