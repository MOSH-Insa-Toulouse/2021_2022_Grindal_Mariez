#include <SoftwareSerial.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>                // pins for I2C are defined by Wire library

// digital potentiometer
int pos = 255;
const byte csBarPin = 10;       // MCP42050 chip select pin
const int  maxPositions = 256;  // wiper can move from 0 to 255 = 256 positions
const long rAB = 46300;         // 46300 Ohm with multimeter between 5 and 6
const byte rWiper = 320;        // 320 ohm measures when in position 255
const byte pot0 = 0x11;         // pot0 addr // B 0001 0001
const byte pot0Shutdown = 0x21; // pot0 shutdown // B 0010 0001 
#define csBarPin 10
#define siPin 11

// signal amplifier
const int flexPin = A0;         // pin connected to voltage divider output
const float VCC = 5.0;          // voltage at Ardunio 5V line
float Rs = 0.0;
const float R5 = 10000.0;
const float R1 = 100000.0;
const float R3 = 100000.0;
float Rcal=rWiper+468;
float gain = 1+R3/Rcal;

unsigned long previousMillis = 0; // count milliseconds from last measurement
const long interval = 700;        // time between two measurements

// OLED screen
#define SCREEN_WIDTH 128         // OLED display width, in pixels
#define SCREEN_HEIGHT 32         // OLED display height, in pixels
#define OLED_RESET -1            // reset pin
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 OLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// rotary encoder KY-040
#define encoderPinA 2                 // right
#define encoderPinB 3                 // left
#define  switchButton 4               // switch button
volatile unsigned int encoderPos = 0; // a counter for the dial
unsigned int lastReportedPos = 1;     // change management
static boolean rotating = false;      // debounce management
int menu=0; int submenu=0;
int setGain=0; // disables menu rotation when 1
int autoCalibrate=0; // to prevent the calibration to run in a loop
int maxRot=65335; float Mohms = 0.;
int ADCin = 0; float VADC = 0.;

// interrupt service routine
boolean A_set = false;
boolean B_set = false;

// bluetooth and dig. pot. serial ports
#define rxPin 8   // connect rxPin to TX on HC-05
#define txPin 9   // // connect txPin to RX on HC-05
SoftwareSerial mySerialBT(rxPin ,txPin);
SoftwareSerial mySerialDP(csBarPin ,siPin);
byte incomingByte = 0;   // for incoming serial data
byte serialRX;    // stock the received data
volatile byte RX = 0; 
byte mode = 0; // send or receive data from the BT application

void setup() {

  pinMode(flexPin, INPUT);
  // OLED screen
  if(!OLED.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
    while(1);
  OLEDstartup();

  // rotary encoder KY-040
  pinMode(encoderPinA, INPUT);
  pinMode(encoderPinB, INPUT);
  pinMode(switchButton, INPUT);
  // turn on pullup resistors
  digitalWrite(encoderPinA, HIGH);
  digitalWrite(encoderPinB, HIGH);
  digitalWrite(switchButton, HIGH);
  // encoder pin on interrupt 0 (pin 2)
  attachInterrupt(0, doEncoderA, CHANGE);
  // encoder pin on interrupt 1 (pin 3)
  attachInterrupt(1, doEncoderB, CHANGE);

  // dig. pot.
  SPI.begin();
  digitalWrite(csBarPin, HIGH);   // chip select default to de-selected
  pinMode(csBarPin, OUTPUT);      // configure chip select as output
  setPot(pot0, pos);

  //bluetooth
  pinMode(rxPin,INPUT);
  pinMode(txPin,OUTPUT);
  Serial.begin(9600);
  mySerialBT.begin(9600);
  mySerialDP.begin(9600);
  mySerialBT.listen();
  mySerialBT.flush();
}


void loop(){
  OLED.setTextSize(1); // measure R and V changes it to 2
  int sent = 1;        // send only 1 byte when mode == 2
  rotating = true;     // reset the debouncer

  // detect button
  if (digitalRead(switchButton) == LOW){
    menu = 1;}
  
  // BT
  if (mySerialBT.available() > 0){
    incomingByte = mySerialBT.read();
    if (incomingByte==1){
      mode = 1;}
    else if (incomingByte==2){
      sent = 0;
      mode = 2;}
    else {mode = incomingByte;}}
  
  // measurement
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {     
    previousMillis = currentMillis;   
    ADCin = analogRead(flexPin);
    // conversion from 10 to 8 bits
    ADCin = map(analogRead(flexPin),0,1024,0,255);  
    VADC = 5.*ADCin/256. ;
    float Rs = (1.0+R3/Rcal)*R1*(VCC/VADC)-R1-R5;
    Mohms = Rs/1000000.;  
    if (mode == 1){
      // send measurement to app
      mySerialBT.write(ADCin);}}
  
  // send dig. pot. pos to recalculate Rcal in app
  if ((mode == 2) && (sent != 1)){
    mySerialBT.write(pos);
    sent=1;}
  
  // rotary encoder and menu shown on screen
  if (lastReportedPos != encoderPos){ 
    if (submenu == 1){
      maxRot = 3;
      lastReportedPos = encoderPos;
      OLED.clearDisplay();
      OLED.setCursor(0, 0);
      switch (lastReportedPos) {
        case 0:
          OLED.println("-> MEASURE R <-");
          OLED.println("MEASURE V");
          OLED.println("AUTO CALIBRATE");
          OLED.println("SET GAIN");
          break;
        case 1:
          OLED.println("MEASURE R");
          OLED.println("-> MEASURE V <-");
          OLED.println("AUTO CALIBRATE");
          OLED.println("SET GAIN");
          break;
        case 2:
          OLED.println("MEASURE R");
          OLED.println("MEASURE V");
          OLED.println("-> AUTO CALIBRATE <-");
          OLED.println("SET GAIN");
          break;
        case 3:
          OLED.println("MEASURE R");
          OLED.println("MEASURE V");
          OLED.println("AUTO CALIBRATE");
          OLED.println("-> SET GAIN <-");
          break; }
          OLED.display();} 
    else if ((submenu == 2) && (setGain == 1)){
      maxRot = 255;
      lastReportedPos = encoderPos;
      setPot(pot0, encoderPos);
      pos = encoderPos;
      OLED.clearDisplay();
      OLED.setCursor(0, 0);
      OLED.println("Dig, pot. position: ");     
      OLED.println(encoderPos, DEC);
      OLED.print("Gain:");
      OLED.setCursor(64, 16);
      OLED.println(gain);
      OLED.print("Voltage:");
      OLED.setCursor(64, 24);    
      OLED.print(readSensor());
      OLED.display();}}
    
  // button has been pressed
  if ((menu == 1) && (digitalRead(switchButton) != LOW)){
    if (setGain == 1){setGain = 0;} 
    switch (submenu){ 
      case 0:
        OLED.clearDisplay();
        encoderPos = 0;
        submenu = 1;
        break;
      case 1:
        OLED.clearDisplay();
        submenu = 2;
        break;
      case 2:
        // problem with screen not updating after
        // exiting the R measure
        OLED.clearDisplay();
        OLED.setCursor(0,0);
        OLED.println("-> MEASURE R <-");
        OLED.println("MEASURE V");
        OLED.println("AUTO CALIBRATE");
        OLED.println("SET GAIN");
        OLED.display();
        submenu = 1;
        encoderPos = 0;
        autoCalibrate = 0;
        break;} 
    menu= 0 ;} 
  
  // menu management
  if (digitalRead(switchButton) != LOW){ 
    if ((submenu == 2) && (lastReportedPos == 3)){
      if (setGain != 1){
        OLED.clearDisplay();
        OLED.setCursor(0,0); 
        OLED.println("Please rotate to ");
        OLED.println("adjust the gain. ");
        OLED.display();
        delay(500);
        OLED.clearDisplay();
        pos=encoderPos;
        setGain = 1;}}
    else if ((submenu == 2) && (setGain != 1)){
      switch (lastReportedPos){ 
        case 0:
          OLED.clearDisplay();
          OLED.setCursor(0,0);
          OLED.setTextSize(2); 
          OLED.println("R (MOhm): ");  
          OLED.print(Mohms);
          OLED.display();
          delay(100); 
          break;
        case 1:
          OLED.clearDisplay();
          OLED.setCursor(0,0);
          OLED.setTextSize(2);
          OLED.println("VADC (V): ");
          OLED.print(VADC);
          OLED.display(); 
          delay(100);
          break;
        case 2:        
          int showPos = -1;
          if (autoCalibrate != 1){
            OLED.clearDisplay();
            OLED.setCursor(0,0);
            OLED.println("Please wait...");
            OLED.display();
            delay(100);
            showPos = potSwiper(1.1);
            OLED.clearDisplay();
            OLED.setCursor(0,0);  
            OLED.println("Dig. pot. position:");
            OLED.println(showPos);
            OLED.println("Gain: ");
            OLED.setCursor(64, 16);
            OLED.println(gain);
            OLED.print("Voltage:");
            OLED.setCursor(64, 24);
            OLED.print(readSensor());
            OLED.display();
            autoCalibrate = 1; }          
            break;}}}
} 

// --end of loop--

float readSensor(){
  int ADCin = analogRead(flexPin);
  // conversion from 10 to 8 bits
  ADCin = map(analogRead(flexPin),0,1024,0,255);  
  float VADC = 5.*ADCin/256.;
  return VADC;
}

// changes dig. pot. position and updates Rcal and gain
void setPot(int addr, int pos){
  pos = constrain(pos, 0, 255);
  digitalWrite(csBarPin, LOW);    // select chip
  //// configure target pot with wiper position
  SPI.transfer(addr);             
  SPI.transfer(pos);
  digitalWrite(csBarPin, HIGH);   // de-select chip
  Rcal = 320+468+(rAB/255)*(255-pos);
  gain = 1+R3/Rcal;          
}

// changes dig. pot. to land on approx.
// the initial voltage
int potSwiper(float init_voltage) {
  setPot(pot0, 255);
  delay(1000);
  float VADC_old = 0.;
  int pos_init=0;
  for(int i = 255; i >= 0; i--){
    pos_init = i;
    setPot(pot0, pos_init);
    // small delay to stabilize the voltage
    // after changing the dig. pot.
    delay(200);
    float VADC_new = readSensor();
    if (VADC_new <= init_voltage){
      break;}
    if (VADC_new > init_voltage){
      if ((VADC_old > VADC_new) && (VADC_old != 0.0)){
        pos_init = i-1;  
        
        setPot(pot0, pos);
        break;}}}
    pos = pos_init;
  return pos_init; 
}

void OLEDstartup(){ 
  OLED.clearDisplay();
  OLED.setTextSize(1); 
  OLED.setTextColor(SSD1306_WHITE);
  OLED.setCursor(30, 10);
  OLED.println("LOW-TECH");
  OLED.setCursor(30, 20);
  OLED.println("FLEX SENSOR");
  OLED.display();
  delay(1000);
  OLED.invertDisplay(true);
  delay(100);
  OLED.invertDisplay(false);
  delay(100);
  OLED.setTextSize(2);  
  OLED.setCursor(30, 0);  
  OLED.clearDisplay();
  OLED.print(F("Ready!"));
  OLED.setCursor(20, 25);
  OLED.setTextSize(1);
  OLED.print(F("Press to start.")); 
  OLED.display();
}

// interrupt on A changing state and debouncing
void doEncoderA(){
  // debounce
  if (rotating){delay(1);} // wait a little until the bouncing is done
  // test transition
  if (digitalRead(encoderPinA) != A_set ){ // debounce once more
    A_set = !A_set;
    // update counter + if A leads B
    if (A_set && !B_set){
      if (lastReportedPos == maxRot){
        encoderPos = 0;}
      else {encoderPos += 1;}}    
    // no more debouncing until loop() hits again
    rotating = false;}}

// interrupt on B changing state
void doEncoderB(){
  if (rotating){delay(1);}
  if (digitalRead(encoderPinB) != B_set){
    B_set = !B_set;
    //  update counter - 1 if B leads A
    if (B_set && !A_set){
      if (lastReportedPos == 0){
        encoderPos = maxRot;}
      else {encoderPos -= 1;}}
    rotating = false;}}
