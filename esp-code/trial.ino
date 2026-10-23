#define ENABLE_USER_AUTH
#define ENABLE_DATABASE


#include <WiFiClientSecure.h>
#include <WiFi.h>
#include <DHT.h>
#include <FirebaseClient.h>

// ---------- Wi-Fi ----------
#define WIFI_SSID "MOTO-G35"
#define WIFI_PASSWORD "alifmoto"

// ---------- Firebase ----------
#define API_KEY "AIzaSyCriW12_vEx_dwGAj6D99XUg_w1ikdGur4"
#define USER_EMAIL "EMAIL"
#define USER_PASSWORD "PASSWAORD"
#define DATABASE_URL "https://smart-home-c853b-default-rtdb.firebaseio.com/"

// ---------- DHT11 ----------
#define DHTPIN 4
#define DHTTYPE DHT11
#define MQ 34
#define BUZZER 18

DHT dht(DHTPIN, DHTTYPE);

// ---------- Firebase ----------
UserAuth user_auth(API_KEY, USER_EMAIL, USER_PASSWORD);

FirebaseApp app;

WiFiClientSecure ssl_client;

using AsyncClient = AsyncClientClass;
AsyncClient aClient(ssl_client);

RealtimeDatabase Database;

void auth_debug_print(AsyncResult &aResult)
{
  if (aResult.isEvent())
  {
    Firebase.printf(
      "Event task: %s, msg: %s, code: %d\n",
      aResult.uid().c_str(),
      aResult.eventLog().message().c_str(),
      aResult.eventLog().code()
    );
  }

  if (aResult.isDebug())
  {
    Firebase.printf(
      "Debug task: %s, msg: %s\n",
      aResult.uid().c_str(),
      aResult.debug().c_str()
    );
  }

  if (aResult.isError())
  {
    Firebase.printf(
      "Error task: %s, msg: %s, code: %d\n",
      aResult.uid().c_str(),
      aResult.error().message().c_str(),
      aResult.error().code()
    );
  }
}

void setup()
{
  Serial.begin(115200);
  delay(1000);

  dht.begin();
  pinMode(MQ, INPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(2, OUTPUT);

  Serial.println();
  Serial.println("===== RENO =====");

  // Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED)
  {
    digitalWrite(2, HIGH);
    Serial.print(".");
    delay(500);
  }

  digitalWrite(2, LOW);
  Serial.println();
  Serial.println("Wi-Fi connected!");

  // SSL
  ssl_client.setInsecure();

  // Firebase
  Serial.println("Connecting to Firebase...");

 ssl_client.setInsecure();

Serial.println("Initializing Firebase...");

initializeApp(
  aClient,
  app,
  getAuth(user_auth),
  120000,
  auth_debug_print
);

Serial.println("initializeApp called");
  
  app.getApp<RealtimeDatabase>(Database);

  Database.url(DATABASE_URL);

  Serial.println("Firebase setup complete!");
}

void loop()
{
  app.loop();

  Serial.println("Loop running...");

  if (app.ready())
  {
    Serial.println("Firebase ready!");

    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    int airQuality = analogRead(MQ);

    if (airQuality > 750 )
    {
      Serial.println("Alert Gas Value reached threshold");
      digitalWrite(BUZZER, LOW);
    }
    else 
    {
      digitalWrite(BUZZER, HIGH);
    }

    if (isnan(temperature) || isnan(humidity))
    {
      Serial.println("DHT11 reading failed!");
    }
    else
    {
      Serial.print("Temperature: ");
      Serial.print(temperature);
      Serial.println(" C");

      Serial.print("Humidity: ");
      Serial.print(humidity);
      Serial.println(" %");

      Serial.print("Air Quality: ");
      Serial.print(airQuality);

      Database.set<float>(
        aClient,
        "/reno/temperature",
        temperature
      );

      Database.set<float>(
        aClient,
        "/reno/humidity",
        humidity
      );

      Database.set<int>(
        aClient,
        "/reno/airQuality",
        airQuality
      );

      Database.set<bool>(
        aClient,
        "/reno/connected",
        true
      );

      Serial.print("Data sent!");
    }
  }
  else
  {
    Serial.println("Waiting for Firebase...");
  }
}

  
