#include <Arduino.h>
#include <BluetoothSerial.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run 'make menuconfig' to and enable it
#endif

BluetoothSerial bluetoothSerial;

void blueToothSerialCallback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param) {
    Serial.print("Event: ");
    Serial.println(event);

    if (event == ESP_SPP_SRV_OPEN_EVT) {
        Serial.println("Client Connected has address:");

        for (int i = 0; i < 6; i++) {
            Serial.printf("%02X", param->srv_open.rem_bda[i]);
            if (i < 5) {
                Serial.print(":");
            }
        }

        Serial.println();
    }

    if(event == ESP_SPP_CLOSE_EVT ){
        Serial.println("Client disconnected");
    }

}

void setup() {
    Serial.begin(115200);
    bluetoothSerial.register_callback(blueToothSerialCallback);
    if (bluetoothSerial.begin("ESP32test")) {
        Serial.println("The device started, now you can pair it with bluetooth!");
    } else {
        Serial.println("Failed to start bluetooth");
    }
}

void loop() {

    while (Serial.available()) {
        delay(20);
        bluetoothSerial.write(Serial.read());
        bluetoothSerial.println();
    }

    while (bluetoothSerial.available()) {
        delay(20);

        char character = bluetoothSerial.read();

        if (character != '\n') {
            Serial.write(character);
        } else {
            Serial.println();
        }
    }

    delay(20);
}