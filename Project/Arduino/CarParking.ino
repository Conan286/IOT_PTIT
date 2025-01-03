#include <WiFi.h>
#include <FirebaseESP32.h>

#define SENSOR1 32
#define SENSOR2 33
#define SENSOR3 25
#define SENSOR4 26

// Thông tin Wi-Fi
const char *ssid = "Galaxy A047787";
const char *pass = "dung12345";

// Firebase credentials
#define FIREBASE_HOST "test-7dfa4-default-rtdb.asia-southeast1.firebasedatabase.app"
#define FIREBASE_AUTH "ChE5mR3x5FcY4Eab98BEddrimTgILxtPkWDJo2SQ"

FirebaseData firebaseData;
FirebaseConfig config;
FirebaseAuth auth;

void setup()
{
	Serial.begin(115200);

	// Kết nối Wi-Fi
	WiFi.begin(ssid, pass);
	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		Serial.print(".");
	}
	Serial.println("\nConnected to Wi-Fi");
	Serial.println(WiFi.localIP());

	config.host = FIREBASE_HOST;
  config.database_url = "https://" FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;

	Firebase.begin(&config, &auth);
	Firebase.reconnectWiFi(true);

	// Khởi tạo các cảm biến
	pinMode(SENSOR1, INPUT_PULLUP);
	pinMode(SENSOR2, INPUT_PULLUP);
	pinMode(SENSOR3, INPUT_PULLUP);
	pinMode(SENSOR4, INPUT_PULLUP);
}


void updateSensorStatus(int sensorNumber, int status)
{
	String path = "/parking/sensor" + String(sensorNumber);
	if (Firebase.setInt(firebaseData, path, status))
	{
		Serial.println("Firebase update successful: " + path + " = " + String(status));
	}
	else
	{
		Serial.println("Firebase update failed: " + firebaseData.errorReason());
	}
}

void updateSensorSoLanSuDung(int sensorNumber)
{
    String path = "/soLanSuDung/sensor" + String(sensorNumber);

    // Lấy giá trị hiện tại từ Firebase
    if (Firebase.getInt(firebaseData, path))
    {
        int currentCount = firebaseData.intData();
        Serial.println("Current usage count for sensor " + String(sensorNumber) + ": " + String(currentCount));

        // Tăng giá trị lên 1 và cập nhật Firebase
        if (Firebase.setInt(firebaseData, path, currentCount + 1))
        {
            Serial.println("Updated usage count for sensor " + String(sensorNumber) + ": " + String(currentCount + 1));
        }
        else
        {
            Serial.println("Failed to update usage count for sensor " + String(sensorNumber) + ": " + firebaseData.errorReason());
        }
    }
    else
    {
        Serial.println("Failed to get usage count for sensor " + String(sensorNumber) + ": " + firebaseData.errorReason());
    }
}


void sensor()
{
	int L1 = digitalRead(SENSOR1);
	int L2 = digitalRead(SENSOR2);
	int L3 = digitalRead(SENSOR3);
	int L4 = digitalRead(SENSOR4);

	if (L1 == 1)
	{
		Serial.println("IR SENSOR 1 detected");
		updateSensorStatus(1, 1);
   		updateSensorSoLanSuDung(1);
	}
	else
	{
		Serial.println("1 === All clear");
		updateSensorStatus(1, 0);
	}

	if (L2 == 1)
	{
		Serial.println("IR SENSOR 2 detected");
		updateSensorStatus(2, 1);
   		updateSensorSoLanSuDung(2);
	}
	else
	{
		Serial.println("2 === All clear");
		updateSensorStatus(2, 0);
	}

	if (L3 == 1)
	{
		Serial.println("IR SENSOR 3 detected");
		updateSensorStatus(3, 1);
    		updateSensorSoLanSuDung(3);
	}
	else
	{
		Serial.println("3 === All clear");
		updateSensorStatus(3, 0);
	}

	if (L4 == 1)
	{
		Serial.println("IR SENSOR 4 detected");
		updateSensorStatus(4, 1);
   		updateSensorSoLanSuDung(4);
	}
	else
	{
		Serial.println("4 === All clear");
		updateSensorStatus(4, 0);
	}
}

void loop()
{
	sensor();
	delay(1000);
}