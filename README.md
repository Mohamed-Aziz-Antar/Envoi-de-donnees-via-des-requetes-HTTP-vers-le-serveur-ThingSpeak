# 🌡️ IoT Project – ESP32 + DHT11 + ThingSpeak (HTTP)

Ce projet utilise une **ESP32**, un capteur **DHT11**, et le protocole **HTTP** pour envoyer les données de température et d’humidité vers **ThingSpeak** afin de les visualiser en temps réel 📈.

---

## 📷 Résultat sur ThingSpeak

Voici un aperçu du graphique obtenu sur ThingSpeak :

![ThingSpeak](https://github.com/user-attachments/assets/0a7f237e-cf65-4604-9a4a-892921b0dbc4)

---

## ⚙️ Matériel utilisé

| Composant | Description |
|-----------|-------------|
| ESP32 | Microcontrôleur Wi-Fi |
| DHT11 | Capteur Température & Humidité |
| Câbles Dupont | Connexions |
| ThingSpeak | Plateforme IoT Cloud |

---

## 🧠 Fonctionnement

1. L’ESP32 lit la température et humidité via le **DHT11**.
2. Connexion au Wi-Fi (mode station).
3. Envoi des données vers **ThingSpeak via HTTP GET**.
4. Visualisation en temps réel sur dashboard.

---

## 🌐 Architecture Réseau

ESP32 → Wi-Fi → TCP/IP → HTTP GET → ThingSpeak Cloud


---

## 🔌 Connexions du DHT11

| DHT11 Pin | ESP32 GPIO |
|-----------|-----------|
| VCC | 3.3V |
| GND | GND |
| DATA | GPIO 4 |

---

## 🧾 Code source principal (`main.c`)

Le code complet se trouve dans le dossier :  

/main/main.c


---

## 📡 Fonction HTTP utilisée

```c
esp_http_client_config_t config = {
    .url = url,
    .method = HTTP_METHOD_GET,
};
esp_http_client_perform(client);

➡️ Cela prouve que le protocole HTTP est bien utilisé pour l'envoi des données.

🚀 Résultat sur ThingSpeak

Graphique affichant :

Température (°C)

Humidité (%)

📌 Intervalle d’envoi : 1 seconde

