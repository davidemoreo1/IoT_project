#include<Wire.h>
#include <Adafruit_AM2315.h>
#include "LowPower.h"
#include "SIGFOX.h"
//created by IoT_Group_2
Adafruit_AM2315 am2315;
unsigned int soc_battery,temp,humid;
int batt;
int a;
int b;
unsigned int sleepCounter;

// Sigfox declaration======================================================
static const String device = "417D73";  //  Set this to your device name if you're using UnaBiz Emulator.
static const bool useEmulator = false;  //  Set to true if using UnaBiz Emulator.
static const bool echo = true;  //  Set to true if the SIGFOX library should display the executed commands.
static const Country country = COUNTRY_SG;  //  Set this to your country to configure the SIGFOX transmission frequencies.
static UnaShieldV2S transceiver(country, useEmulator, device, echo);  //  Uncomment this for UnaBiz UnaShield V2S Dev Kit
//static UnaShieldV1 transceiver(country, useEmulator, device, echo);  //  Uncomment this for UnaBiz UnaShield V1 Dev Kit
//==========================================
void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  am2315.begin();
  delay(500);
    if (! am2315.begin()) {
     Serial.println("Sensor not found, check wiring & pullups!"); 
      delay(500);
    }
  Serial.begin(9600); 
  setup_sigfox();
  delay(500); 
}

void loop() {
for (sleepCounter = 3; sleepCounter > 0; sleepCounter--)
{
  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF); 
   
}

b=read_Humidity();
delay(2500);
a=read_Temp();
delay(2500);
Serial.print("Temp_a *C: "); Serial.println(a);
Serial.print("Hum_b %: "); Serial.println(b);
Message msg(transceiver);  //  Will contain the structured sensor data.
msg.addField("abc", a);   //  4 bytes
msg.addField("hmd", b);   //  4 bytes
batt=battery_check();
delay(500); 
 Serial.print(F("\n>> Device sending message ")); Serial.print(msg.getEncodedMessage() + "...");
  transceiver.echoOn();
  if (msg.send()) {
    Serial.println(F("Message sent"));
  } else {
    Serial.println(F("Message not sent"));
  }
}

int battery_check(){
float battery_voltage;
  battery_voltage = (analogRead(A0)/1024*5);
  battery_voltage = battery_voltage *100;
  soc_battery=(int) battery_voltage;
  Serial.print("SoC(%): "); Serial.println(battery_voltage);
  delay(500);
  return  soc_battery ;
  }
  
int read_Humidity (){
  float humidity;
   float temperature;
  if (! am2315.readTemperatureAndHumidity(&temperature, &humidity)) {
    Serial.println("Failed to read data from AM2315");
    delay(500);
  }
 else{
  
  humid=  (humidity);
  temp=  (temperature);
  Serial.print("Temp *C: "); Serial.println(temp);
  Serial.print("Hum %: "); Serial.println(humid);
  delay(1000);
  return humid;
 }
}

  int read_Temp (){
  float temperature;
   float humidity;
  if (! am2315.readTemperatureAndHumidity(&temperature, &humidity)) {
    Serial.println("Failed to read data from AM2315");
    delay(500);
  }
 else{
  temp=  (temperature);
  humid=  (humidity);
  Serial.print("Temp *C: "); Serial.println(temp);
  Serial.print("Hum %: "); Serial.println(humid);

  delay(1000);
  return temp;
 }
  }

  
void setup_sigfox(){
 //  Begin SIGFOX Module Setup

  String result = "";
  //  Enter command mode.
  Serial.println(F("\nEntering command mode..."));
  transceiver.enterCommandMode();

  if (useEmulator) {
    //  Emulation mode.
    transceiver.enableEmulator(result);
  } else {
    //  Disable emulation mode.
    Serial.println(F("\nDisabling emulation mode..."));
    transceiver.disableEmulator(result);

    //  Check whether emulator is used for transmission.
    Serial.println(F("\nChecking emulation mode (expecting 0)...")); int emulator = 0;
    transceiver.getEmulator(emulator);
  }

  //  Set the frequency of SIGFOX module to SG/TW.
  Serial.println(F("\nSetting frequency..."));  result = "";
  transceiver.setFrequencySG(result);
  Serial.print(F("Set frequency result = "));  Serial.println(result);

  //  Get and display the frequency used by the SIGFOX module.  Should return 3 for RCZ4 (SG/TW).
  Serial.println(F("\nGetting frequency (expecting 3)..."));  String frequency = "";
  transceiver.getFrequency(frequency);
  Serial.print(F("Frequency (expecting 3) = "));  Serial.println(frequency);

  //  Read SIGFOX ID and PAC from module.
  Serial.println(F("\nGetting SIGFOX ID..."));  String id = "", pac = "";
  if (transceiver.getID(id, pac)) {
    Serial.print(F("SIGFOX ID = "));  Serial.println(id);
    Serial.print(F("PAC = "));  Serial.println(pac);
  } else {
    Serial.println(F("ID KO"));
  }

  //  Exit command mode and prepare to send message.
  transceiver.exitCommandMode();

  //  End SIGFOX Module Setup
  ////////////////////////////////////////////////////////////   
    
    }
