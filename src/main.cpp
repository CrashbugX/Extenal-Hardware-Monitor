#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include <Wifi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>


#define x_limit 239
#define y_limit 134

const char* ssid = "Souler";
const char* password = "familiaceballos";

String payload = "{}";

TFT_eSPI tft = TFT_eSPI();
int xpos = 230;
int ypos = 120;
bool dir = 0; // 0 down, 1 up

const char* CPU_VCore;
const char* Chasis1_Fan;
const char* CPU_Fan;
const char* Chasis2_Fan;
const char* M2_Fan;
const char* CPU_Model;
const char* CPUCore1_Speed;
const char* CPUCore2_Speed;
const char* CPUCore3_Speed;
const char* CPUCore4_Speed;
const char* CPUCore5_Speed;
const char* CPUCore6_Speed;
const char* CPUCore7_Speed;
const char* CPUCore8_Speed;
const char* CPUPackage_Temp;
const char* CPUTotal_Load;
const char* CPUPower;
const char* GPU_Model;
const char* GPUTemp;
const char* GPUCore;
const char* GPUMem;
const char* GPUPower;

float cputemp;
float cpuper;
float gputemp;

int cpuplot[10] = {0,0,0,0,0,0,0,0,0,0};
int gpuplot[10] = {0,0,0,0,0,0,0,0,0,0};

float floatValue(const char* value){
  float res;
  if(value[2] == '.'){
    res = 10*((float)value[0]-48) + ((float)value[1]-48) + ((float)value[3]-48)/10;
  } else if (value[1] == '.'){
    res = ((float)value[0]-48) + ((float)value[2]-48)/10;
  } else {
    res = -404;
  }
  return res;
}

void tft_init(){
  tft.begin();
  tft.setRotation(3);
  
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setFreeFont(&Orbitron_Light_24);
  tft.fillScreen(TFT_BLACK);
  tft.drawLine(0,0,239,134,TFT_RED);
  tft.drawLine(0,y_limit, x_limit,0,TFT_RED);
  tft.setCursor(240/2 - 24*6/2, 135/2);
  tft.print("SOULER");
  delay(1500);
}

void setTextColor(){
  if(cputemp > 75 || gputemp > 75){
    tft.fillScreen(TFT_RED);
    tft.setTextColor(TFT_BLACK, TFT_RED);
  } else if (cputemp > 70 || gputemp > 70){
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_RED, TFT_BLACK);
  } else if (cputemp > 60 || gputemp > 60){
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_BROWN, TFT_BLACK);
  } else {
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_DARKGREEN, TFT_BLACK);
  }
}
void plotTemps(){
  //tft.drawLine(105, 0, 105, 135, TFT_BLUE);

  tft.drawLine(120, 10-1, 120, 60+1, TFT_WHITE);
  tft.drawLine(120, 60+1, 220+1, 60+1, TFT_WHITE);

  tft.drawLine(120, 15-2+60, 120, 65+60, TFT_WHITE);
  tft.drawLine(120, 60+66, 220+1, 60+66, TFT_WHITE);

  for(int i = 0; i < 9; i++){
    tft.drawLine(121+i*10, 60-cpuplot[i], 121+(i+1)*10, 60-cpuplot[i+1], TFT_BLUE);
    tft.drawLine(121+i*10, 125-gpuplot[i], 121+(i+1)*10, 125-gpuplot[i+1], TFT_BLUE);
    //delay(20);
  }
}

void tft_template(){
  setTextColor();
  tft.setFreeFont(&Orbitron_Light_24);
  tft.setCursor(20, 32*1);  //tft.setCursor(240/2 - 24*1.5, 32*1);
  tft.print("CPU");
  tft.setCursor(20, 32*3);  //tft.setCursor(240/2 - 24*3/2, 32*3);
  tft.print("GPU");

  tft.setFreeFont(&Orbitron_Light_24);
  tft.setCursor(10,32*2);
  tft.print(CPUPackage_Temp);
  //tft.print("      ");
  //tft.print(CPUTotal_Load);
  tft.setCursor(10,32*4);
  tft.print(GPUTemp);

  plotTemps();
}

void getServerData(){
  if ((WiFi.status() == WL_CONNECTED)){
    HTTPClient http;
    http.begin("http://192.168.0.194:8085/data.json");
    int httpCode = http.GET();
    if(httpCode > 0){
      payload = http.getString();
      Serial.println(httpCode);
      //Serial.println(payload);
    }
    else {
      Serial.println("Error on HTTP request");
    }
    http.end();
  }
}

void parseJSON(){
  const size_t capacity = 97*JSON_ARRAY_SIZE(0) + 11*JSON_ARRAY_SIZE(1) + 4*JSON_ARRAY_SIZE(2) + 2*JSON_ARRAY_SIZE(3) + 3*JSON_ARRAY_SIZE(4) + JSON_ARRAY_SIZE(5) + JSON_ARRAY_SIZE(6) + JSON_ARRAY_SIZE(7) + JSON_ARRAY_SIZE(8) + 2*JSON_ARRAY_SIZE(9) + 3*JSON_ARRAY_SIZE(10) + JSON_ARRAY_SIZE(15) + 127*JSON_OBJECT_SIZE(7) + 7470;
  DynamicJsonDocument doc(capacity);
  const char* json = payload.c_str();
  deserializeJson(doc, json, DeserializationOption::NestingLimit(12));
  // Unnesting


  // CPU VCore
  JsonObject Children_0 = doc["Children"][0];
  JsonArray Children_0_Children = Children_0["Children"];
  JsonObject Children_0_Children_0 = Children_0_Children[0];
  JsonObject Children_0_Children_0_Children_0 = Children_0_Children_0["Children"][0];
  JsonArray Children_0_Children_0_Children_0_Children = Children_0_Children_0_Children_0["Children"];
  JsonObject Children_0_Children_0_Children_0_Children_0 = Children_0_Children_0_Children_0_Children[0];
  JsonArray Children_0_Children_0_Children_0_Children_0_Children = Children_0_Children_0_Children_0_Children_0["Children"];
  JsonObject Children_0_Children_0_Children_0_Children_0_Children_0 = Children_0_Children_0_Children_0_Children_0_Children[0];
  CPU_VCore = Children_0_Children_0_Children_0_Children_0_Children_0["Value"]; 
  // Chasis 1 Fan
  JsonObject Children_0_Children_0_Children_0_Children_2 = Children_0_Children_0_Children_0_Children[2];
  JsonArray Children_0_Children_0_Children_0_Children_2_Children = Children_0_Children_0_Children_0_Children_2["Children"];
  JsonObject Children_0_Children_0_Children_0_Children_2_Children_0 = Children_0_Children_0_Children_0_Children_2_Children[0];
  Chasis1_Fan = Children_0_Children_0_Children_0_Children_2_Children_0["Value"];
  // CPU Fan
  JsonObject Children_0_Children_0_Children_0_Children_2_Children_1 = Children_0_Children_0_Children_0_Children_2_Children[1];
  CPU_Fan = Children_0_Children_0_Children_0_Children_2_Children_1["Value"];
  // Chasis 2 Fan
  JsonObject Children_0_Children_0_Children_0_Children_2_Children_2 = Children_0_Children_0_Children_0_Children_2_Children[2];
  Chasis2_Fan = Children_0_Children_0_Children_0_Children_2_Children_2["Value"];
  // M.2 Fan
  JsonObject Children_0_Children_0_Children_0_Children_2_Children_3 = Children_0_Children_0_Children_0_Children_2_Children[3];
  M2_Fan = Children_0_Children_0_Children_0_Children_2_Children_3["Value"];
  // CPU Model
  JsonObject Children_0_Children_1 = Children_0_Children[1];
  CPU_Model = Children_0_Children_1["Text"];
  // CPU Core 1 Speed
  JsonArray Children_0_Children_1_Children = Children_0_Children_1["Children"];
  JsonObject Children_0_Children_1_Children_0 = Children_0_Children_1_Children[0];
  JsonArray Children_0_Children_1_Children_0_Children = Children_0_Children_1_Children_0["Children"];
  JsonObject Children_0_Children_1_Children_0_Children_1 = Children_0_Children_1_Children_0_Children[1];
  CPUCore1_Speed = Children_0_Children_1_Children_0_Children_1["Value"];
  // CPU Core 2 Speed
  JsonObject Children_0_Children_1_Children_0_Children_2 = Children_0_Children_1_Children_0_Children[2];
  CPUCore2_Speed = Children_0_Children_1_Children_0_Children_2["Value"];
  // CPU Core 3 Speed
  JsonObject Children_0_Children_1_Children_0_Children_3 = Children_0_Children_1_Children_0_Children[3];
  CPUCore3_Speed = Children_0_Children_1_Children_0_Children_3["Value"];
  // CPU Core 4 Speed
  JsonObject Children_0_Children_1_Children_0_Children_4 = Children_0_Children_1_Children_0_Children[4];
  CPUCore4_Speed = Children_0_Children_1_Children_0_Children_4["Value"];
  // CPU Core 5 Speed
  JsonObject Children_0_Children_1_Children_0_Children_5 = Children_0_Children_1_Children_0_Children[5];
  CPUCore5_Speed = Children_0_Children_1_Children_0_Children_5["Value"];
  // CPU Core 6 Speed
  JsonObject Children_0_Children_1_Children_0_Children_6 = Children_0_Children_1_Children_0_Children[6];
  CPUCore6_Speed = Children_0_Children_1_Children_0_Children_6["Value"];
  // CPU Core 7 Speed
  JsonObject Children_0_Children_1_Children_0_Children_7 = Children_0_Children_1_Children_0_Children[7];
  CPUCore7_Speed = Children_0_Children_1_Children_0_Children_7["Value"];
  // CPU Core 8 Speed
  JsonObject Children_0_Children_1_Children_0_Children_8 = Children_0_Children_1_Children_0_Children[8];
  CPUCore8_Speed = Children_0_Children_1_Children_0_Children_8["Value"];
  // CPU Package Temp
  JsonObject Children_0_Children_1_Children_1 = Children_0_Children_1_Children[1];
  JsonArray Children_0_Children_1_Children_1_Children = Children_0_Children_1_Children_1["Children"];
  JsonObject Children_0_Children_1_Children_1_Children_0 = Children_0_Children_1_Children_1_Children[0];
  CPUPackage_Temp = Children_0_Children_1_Children_1_Children_0["Value"];
  // CPU Total Load
  JsonObject Children_0_Children_1_Children_2 = Children_0_Children_1_Children[2];
  JsonArray Children_0_Children_1_Children_2_Children = Children_0_Children_1_Children_2["Children"];
  JsonObject Children_0_Children_1_Children_2_Children_0 = Children_0_Children_1_Children_2_Children[0];
  CPUTotal_Load = Children_0_Children_1_Children_2_Children_0["Value"];
  // CPU Power
  JsonObject Children_0_Children_1_Children_3 = Children_0_Children_1_Children[3];
  JsonArray Children_0_Children_1_Children_3_Children = Children_0_Children_1_Children_3["Children"];
  JsonObject Children_0_Children_1_Children_3_Children_0 = Children_0_Children_1_Children_3_Children[0];
  CPUPower = Children_0_Children_1_Children_3_Children_0["Value"];
  // GPU Model
  JsonObject Children_0_Children_3 = Children_0_Children[3];
  GPU_Model = Children_0_Children_3["Text"];
  // GPU Temp
  JsonArray Children_0_Children_3_Children = Children_0_Children_3["Children"];
  JsonObject Children_0_Children_3_Children_1 = Children_0_Children_3_Children[1];
  JsonObject Children_0_Children_3_Children_1_Children_0 = Children_0_Children_3_Children_1["Children"][0];
  GPUTemp = Children_0_Children_3_Children_1_Children_0["Value"];
  // GPU Core %
  JsonObject Children_0_Children_3_Children_2 = Children_0_Children_3_Children[2];
  JsonArray Children_0_Children_3_Children_2_Children = Children_0_Children_3_Children_2["Children"];
  JsonObject Children_0_Children_3_Children_2_Children_0 = Children_0_Children_3_Children_2_Children[0];
  GPUCore = Children_0_Children_3_Children_2_Children_0["Value"];
  // GPU Memory %
  JsonObject Children_0_Children_3_Children_2_Children_4 = Children_0_Children_3_Children_2_Children[4];
  GPUMem = Children_0_Children_3_Children_2_Children_4["Value"];
  // GPU Power
  JsonObject Children_0_Children_3_Children_5 = Children_0_Children_3_Children[5];
  JsonObject Children_0_Children_3_Children_5_Children_0 = Children_0_Children_3_Children_5["Children"][0];
  GPUPower = Children_0_Children_3_Children_5_Children_0["Value"];

  //Serial.println(CPU_Model);

}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  tft_init();
  delay(4000);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to the WiFi Network");
  
}

void dataToFloats(){
  cputemp = floatValue(CPUPackage_Temp);
  for(int i = 0; i < 9; i++){
    cpuplot[i] = cpuplot[i+1];
  }
  cpuplot[9] = cputemp/2;
  Serial.print("CPU TEMP = ");
  Serial.println(cputemp, 1);

  /*
  cpuper = floatValue(CPUTotal_Load);
  Serial.print("CPU % = ");
  Serial.println(cpuper, 1);
  */

  gputemp = floatValue(GPUTemp);
  for(int i = 0; i < 9; i++){
    gpuplot[i] = gpuplot[i+1];
  }
  gpuplot[9] = gputemp/2;
  Serial.print("GPU TEMP = ");
  Serial.println(gputemp, 1);
}

void loop() {
  getServerData();
  parseJSON();
  dataToFloats();
  tft_template();
  delay(3000);
}