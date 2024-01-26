#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPDash.h>
#include <AsyncElegantOTA.h>
#include <ArtnetWifi.h>
#include <PCA9685.h>

#define tilt_1 0
#define pan_1 1
#define tilt_2 2
#define pan_2 3
#define tilt_3 4
#define pan_3 5
#define tilt_4 6
#define pan_4 7

#define fogger 15

const int numLeds = 4;                     // CHANGE FOR YOUR SETUP
const int numberOfChannels = numLeds * 3;  // Total number of channels you want to receive (1 led = 3 channels)
ArtnetWifi artnet;
const int startUniverse = 0;  // CHANGE FOR YOUR SETUP most software this is 1, some software send out artnet first universe as 0.
const int maxUniverses = numberOfChannels / 512 + ((numberOfChannels % 512) ? 1 : 0);
bool universesReceived[maxUniverses];
bool sendFrame = 1;

const int freq = 5000;
const int resolution = 8;

const int pulse_duration_min = 500;//min 300
const int pulse_duration_max = 2400;//max 2600

bool power = false;
bool artnet_on = false;
int effect = 0;

const char* ssid = "Adi Home Network";
const char* password = "62826282"; 


PCA9685 pca9685;
AsyncWebServer server(80);
ESPDash dashboard(&server); 
TaskHandle_t Task1;

Card power_toggle(&dashboard, BUTTON_CARD, "Main Power");
Card artnet_toggle(&dashboard, BUTTON_CARD, "ArtNet");
Card fog(&dashboard, BUTTON_CARD, "Fogger");

Card effect0(&dashboard, BUTTON_CARD, "Clear All Effects");
Card effect1(&dashboard, BUTTON_CARD, "Back and Fourth 1");
Card effect2(&dashboard, BUTTON_CARD, "Back and Fourth 2");
Card effect3(&dashboard, BUTTON_CARD, "Left and Right 1");
Card effect4(&dashboard, BUTTON_CARD, "Left and Right 2");
Card effect5(&dashboard, BUTTON_CARD, "Left and Right 3");
Card effect6(&dashboard, BUTTON_CARD, "One by One");
Card effect7(&dashboard, BUTTON_CARD, "Cross");
Card effect8(&dashboard, BUTTON_CARD, "Fast Rotate");
Card effect9(&dashboard, BUTTON_CARD, "Idle Position");

Card all_pan(&dashboard, SLIDER_CARD, "PAN ALL", "", pulse_duration_min, pulse_duration_max);
Card all_tilt(&dashboard, SLIDER_CARD, "TILT ALL", "", pulse_duration_min, pulse_duration_max);

Card S1_pan(&dashboard, SLIDER_CARD, "SR1 PAN", "", pulse_duration_min, pulse_duration_max);
Card S1_tilt(&dashboard, SLIDER_CARD, "SR1 TILT", "", pulse_duration_min, pulse_duration_max);

Card S2_pan(&dashboard, SLIDER_CARD, "SR2 PAN", "", pulse_duration_min, pulse_duration_max);
Card S2_tilt(&dashboard, SLIDER_CARD, "SR2 TILT", "", pulse_duration_min, pulse_duration_max);

Card S3_pan(&dashboard, SLIDER_CARD, "SR3 PAN", "", pulse_duration_min, pulse_duration_max);
Card S3_tilt(&dashboard, SLIDER_CARD, "SR3 TILT", "", pulse_duration_min, pulse_duration_max);

Card S4_pan(&dashboard, SLIDER_CARD, "SR4 PAN", "", pulse_duration_min, pulse_duration_max);
Card S4_tilt(&dashboard, SLIDER_CARD, "SR4 TILT", "", pulse_duration_min, pulse_duration_max);

void set_color(int led, int red, int green, int blue)
{
  if(led == 1)
  {
    ledcWrite(0, red);
    ledcWrite(1, green);
    ledcWrite(2, blue);
  }
  else if(led == 2)
  {
    ledcWrite(3, red);
    ledcWrite(4, green);
    ledcWrite(5, blue);
  }
  else if(led == 3)
  {
    ledcWrite(6, red);
    ledcWrite(7, green);
    ledcWrite(8, blue);
  }
  else if(led == 4)
  {
    ledcWrite(9, red);
    ledcWrite(10, green);
    ledcWrite(11, blue);
  }
}

#include "dances.h"

void setup() {

  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
      return;
  }

  pinMode(fogger,OUTPUT);
  digitalWrite(fogger,HIGH);

  Wire.begin();

  //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
                    Task1code,   /* Task function. */
                    "Task1",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task1,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */                  
  delay(500);

  ledcSetup(0, freq, resolution);
  ledcSetup(1, freq, resolution);
  ledcSetup(2, freq, resolution);
  ledcSetup(3, freq, resolution);
  ledcSetup(4, freq, resolution);
  ledcSetup(5, freq, resolution);
  ledcSetup(6, freq, resolution);
  ledcSetup(7, freq, resolution);
  ledcSetup(8, freq, resolution);
  ledcSetup(9, freq, resolution);
  ledcSetup(10, freq, resolution);
  ledcSetup(11, freq, resolution);

  ledcAttachPin(17, 0);
  ledcAttachPin(2, 1);
  ledcAttachPin(4, 2);

  ledcAttachPin(18, 3);
  ledcAttachPin(19, 4);
  ledcAttachPin(23, 5);

  ledcAttachPin(13, 6);
  ledcAttachPin(26, 7);
  ledcAttachPin(27, 8);

  ledcAttachPin(32, 9);
  ledcAttachPin(33, 10);
  ledcAttachPin(25, 11);

  set_color(1, 0, 0, 0);
  set_color(2, 0, 0, 0);
  set_color(3, 0, 0, 0);
  set_color(4, 0, 0, 0);

  artnet.begin();
  memset(universesReceived, 0, maxUniverses);
  artnet.setArtDmxCallback(onDmxFrame);

  pca9685.setupSingleDevice(Wire, 0x41);
  pca9685.setupOutputEnablePin(16);
  pca9685.enableOutputs(16);
  pca9685.setToServoFrequency();

  power_toggle.attachCallback([&](bool value){
    power = value;
    effect = 0;
    power_toggle.update(value);
    pca9685.setChannelServoPulseDuration(pan_1, 2400);
    pca9685.setChannelServoPulseDuration(pan_2, 2400);
    pca9685.setChannelServoPulseDuration(pan_3, 2400);
    pca9685.setChannelServoPulseDuration(pan_4, 2400);

    pca9685.setChannelServoPulseDuration(tilt_1, 2400);
    pca9685.setChannelServoPulseDuration(tilt_2, 2400);
    pca9685.setChannelServoPulseDuration(tilt_3, 2400);
    pca9685.setChannelServoPulseDuration(tilt_4, 2400);

    set_color(1, 0, 0, 0);
    set_color(2, 0, 0, 0);
    set_color(3, 0, 0, 0);
    set_color(4, 0, 0, 0);

    update_buttons();
  });

  artnet_toggle.attachCallback([&](bool value){
    artnet_on = value;
    artnet_toggle.update(value);
    dashboard.sendUpdates();
  });

  fog.attachCallback([&](bool value){
    if(power) digitalWrite(fogger,!value);
    fog.update(value);
    dashboard.sendUpdates();
  });

  effect0.attachCallback([&](bool value){
    effect = 0;
    update_buttons();
  });

  effect1.attachCallback([&](bool value){
    effect = 1;
    update_buttons();
  });

  effect2.attachCallback([&](bool value){ 
    effect = 2;
    update_buttons();
  });

  effect3.attachCallback([&](bool value){
    effect = 3;
    update_buttons();
  });

  effect4.attachCallback([&](bool value){
    effect = 4;
    update_buttons();
  });

  effect5.attachCallback([&](bool value){
    effect = 5;
    update_buttons();
  });

  effect6.attachCallback([&](bool value){
    effect = 6;
    update_buttons();
  });

  effect7.attachCallback([&](bool value){
    effect = 7;
    update_buttons();
  });

  effect8.attachCallback([&](bool value){
    effect = 8;
    update_buttons();
  });

  effect9.attachCallback([&](bool value){
    effect = 9;
    update_buttons();

    if(power) {
      pca9685.setChannelServoPulseDuration(pan_1, 2400);
      pca9685.setChannelServoPulseDuration(pan_2, 2400);
      pca9685.setChannelServoPulseDuration(pan_3, 2400);
      pca9685.setChannelServoPulseDuration(pan_4, 2400);

      pca9685.setChannelServoPulseDuration(tilt_1, 2000);
      pca9685.setChannelServoPulseDuration(tilt_2, 2000);
      pca9685.setChannelServoPulseDuration(tilt_3, 2000);
      pca9685.setChannelServoPulseDuration(tilt_4, 2000);
    }
  });

  all_pan.attachCallback([&](int value){
    if(power){
      pca9685.setChannelServoPulseDuration(pan_1, value);
      pca9685.setChannelServoPulseDuration(pan_2, value);
      pca9685.setChannelServoPulseDuration(pan_3, value);
      pca9685.setChannelServoPulseDuration(pan_4, value);
    }
    all_pan.update(value);
    dashboard.sendUpdates();
  });

  all_tilt.attachCallback([&](int value){
    if(power){
      pca9685.setChannelServoPulseDuration(tilt_1, value);
      pca9685.setChannelServoPulseDuration(tilt_2, value);
      pca9685.setChannelServoPulseDuration(tilt_3, value);
      pca9685.setChannelServoPulseDuration(tilt_4, value);
    }
    all_tilt.update(value);
    dashboard.sendUpdates();
  });

  S1_pan.attachCallback([&](int value){
    if(power){
      pca9685.setChannelServoPulseDuration(pan_1, value);
    }
    S1_pan.update(value);
    dashboard.sendUpdates();
  });

  S1_tilt.attachCallback([&](int value){
    if(power){
      pca9685.setChannelServoPulseDuration(tilt_1, value);
    }
    S1_tilt.update(value);
    dashboard.sendUpdates();
  });

  S2_pan.attachCallback([&](int value){
    if(power){
      pca9685.setChannelServoPulseDuration(pan_2, value);
    }
    S2_pan.update(value);
    dashboard.sendUpdates();
  });

  S2_tilt.attachCallback([&](int value){
    if(power){
      pca9685.setChannelServoPulseDuration(tilt_2, value);
    }
    S2_tilt.update(value);
    dashboard.sendUpdates();
  });

  S3_pan.attachCallback([&](int value){
    if(power){
      pca9685.setChannelServoPulseDuration(pan_3, value);
    }
    S3_pan.update(value);
    dashboard.sendUpdates();
  });

  S3_tilt.attachCallback([&](int value){
    if(power){
      pca9685.setChannelServoPulseDuration(tilt_3, value);
    }
    S3_tilt.update(value);
    dashboard.sendUpdates();
  });

  S4_pan.attachCallback([&](int value){
    if(power){
      pca9685.setChannelServoPulseDuration(pan_4, value);
    }
    S4_pan.update(value);
    dashboard.sendUpdates();
  });

  S4_tilt.attachCallback([&](int value){
    if(power){
      pca9685.setChannelServoPulseDuration(tilt_4, value);
    }
    S4_tilt.update(value);
    dashboard.sendUpdates();
  });

  AsyncElegantOTA.begin(&server);
  server.begin();
}

void Task1code( void * pvParameters ){
  for(;;){
    if(artnet_on && power)artnet.read();
    delay(1);
  } 
}

void loop() {
  AsyncElegantOTA.loop();

  if(power && effect == 1)
  {
    backandfourth();
  }
  else if(power && effect == 2)
  {
    backandfourth2();
  }
  else if(power && effect == 3)
  {
    leftandright();
  }
  else if(power && effect == 4)
  {
    leftandright2();
  }
  else if(power && effect == 5)
  {
    leftandright3();
  }
  else if(power && effect == 6)
  {
    onebyone();
  }
  else if(power && effect == 7)
  {
    cross();
  }
  else if(power && effect == 8)
  {
    fast_rotate();
  }
}
    

void onDmxFrame(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* data) {
  sendFrame = 1;

  // range check
  if (universe < startUniverse) {
    return;
  }
  uint8_t index = universe - startUniverse;
  if (index >= maxUniverses) {
    return;
  }

  // Store which universe has got in
  universesReceived[index] = true;

  for (int i = 0; i < maxUniverses; i++) {
    if (!universesReceived[i]) {
      sendFrame = 0;
      break;
    }
  }

  // read universe and put into the right part of the display buffer
  for (int i = 0; i < length / 3; i++) {

    int red;
    int green;
    int blue;

    red = data[i * 3];

    green = data[i * 3 + 1];

    blue = data[i * 3 + 2];

    if (i == 0) {
      set_color(1,red,green,blue);
    } else if (i == 1) {
      set_color(2,red,green,blue);
    } else if (i == 2) {
      set_color(3,red,green,blue);
    } else if (i == 3) {
      set_color(4,red,green,blue);
    }
  }
  if (sendFrame) {
    memset(universesReceived, 0, maxUniverses);
  }
}

void update_buttons() {
  if(effect == 0)
  {
    effect0.update(true);
  }
  else
  {
    effect0.update(false);
  }

  if(effect == 1)
  {
    effect1.update(true);
  }
  else
  {
    effect1.update(false);
  }

  if(effect == 2)
  {
    effect2.update(true);
  }
  else
  {
    effect2.update(false);
  }

  if(effect == 3)
  {
    effect3.update(true);
  }
  else
  {
    effect3.update(false);
  }

  if(effect == 4)
  {
    effect4.update(true);
  }
  else
  {
    effect4.update(false);
  }

  if(effect == 5)
  {
    effect5.update(true);
  }
  else
  {
    effect5.update(false);
  }

  if(effect == 6)
  {
    effect6.update(true);
  }
  else
  {
    effect6.update(false);
  }

  if(effect == 7)
  {
    effect7.update(true);
  }
  else
  {
    effect7.update(false);
  }

  if(effect == 8)
  {
    effect8.update(true);
  }
  else
  {
    effect8.update(false);
  }

  if(effect == 9)
  {
    effect9.update(true);
  }
  else
  {
    effect9.update(false);
  }

  dashboard.sendUpdates();
}