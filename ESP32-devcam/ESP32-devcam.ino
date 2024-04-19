#include "OV2640.h"
#include <WiFi.h>
#include "SimStreamer.h"
#include "OV2640Streamer.h"
#include "CRtspSession.h"

OV2640 cam;
WiFiServer rtspServer(8554);
CStreamer *streamer;

void setup() {
    Serial.begin(115200);
    while (!Serial);

    cam.init(esp32cam_config);

    const char *hostname = "devcam";
    WiFi.mode(WIFI_AP);
    bool result = WiFi.softAP(hostname, "12345678", 1, 0);
    if (!result) {
        Serial.println("AP Config failed.");
        return;
    } else {
        Serial.println("AP Config Success.");
        Serial.print("AP MAC: ");
        Serial.println(WiFi.softAPmacAddress());
    }

    rtspServer.begin();
    streamer = new OV2640Streamer(cam);
}

void loop() {
    WiFiClient rtspClient = rtspServer.accept();
    if (rtspClient) {
        Serial.print("client: ");
        Serial.print(rtspClient.remoteIP());
        Serial.println();
        streamer->addSession(rtspClient);
    }

    // Add any additional functionality or tasks here
}
