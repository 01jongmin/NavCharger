#include <EEPROM.h>

int xcoordinate;
int ycoordinate;
int ydirpin = 2;
int ysteppin = 3;
int xdirpin = 4;
int xsteppin = 5;
static int x = 0;
static int y = 0;
uint8_t xaddress = 0;
uint8_t yaddress = 1;

//max x: 13975
void setup() 
{
  Serial.begin(9600);
  Serial.println("<Arduino is ready>");
  pinMode(xdirpin, OUTPUT);
  pinMode(xsteppin, OUTPUT);
  pinMode(ydirpin, OUTPUT);
  pinMode(ysteppin, OUTPUT);
}

void loop()
{
  if (Serial.available()){
    
    int lastx = EEPROM.read(xaddress);
    int lasty = EEPROM.read(yaddress);
    
    Serial.print("LAST X: ");
    Serial.println(String(lastx)); 
    Serial.print("LAST Y: ");
    Serial.println(String(lasty));
    
    String input = Serial.readString();
    Serial.println(input);
  
    int length = input.length();
    char s[length+1];
    
    input.toCharArray(s, length+1);
  
    int xcoordinate, ycoordinate;
  
    sscanf(s, "%i,%i", &xcoordinate, &ycoordinate);
    
    int xstepperCoordinateCurrent = int(map(lastx, 0, 100, 0, 17300));
    int xstepperCoordinateTarget = int(map(xcoordinate, 0, 100, 0, 17300));
    int xdifference = abs(xstepperCoordinateCurrent - xstepperCoordinateTarget);
    
    if (xcoordinate > 100 || xcoordinate < 0){
      Serial.println("ERROR");
      xdifference = 0;
      xcoordinate = lastx;
    } else if (xcoordinate >= lastx) {
      digitalWrite(xdirpin, HIGH);
    } else if (xcoordinate < lastx) {
      digitalWrite(xdirpin, LOW);
    }
  
    EEPROM.write(xaddress, xcoordinate); 
  
    delay(100);
  
    int i;
    
    for (i = 0; i<xdifference; i++)       // Iterate for 4000 microsteps.
      {
        digitalWrite(xsteppin, LOW);  // This LOW to HIGH change is what creates the
        digitalWrite(xsteppin, HIGH); // "Rising Edge" so the easydriver knows to when to step.
        delayMicroseconds(500);      // This delay time is close to top speed for this
      }                              // particular motor. Any faster the motor stalls.
  
    int ystepperCoordinateCurrent = int(map(lasty, 0, 100, 0, 16600));
    int ystepperCoordinateTarget = int(map(ycoordinate, 0, 100, 0, 16600));
    int ydifference = abs(ystepperCoordinateCurrent - ystepperCoordinateTarget);
    
    if (ycoordinate > 100 || ycoordinate < 0){
      Serial.println("ERROR");
      ydifference = 0;
      ycoordinate = lasty;
    } else if (ycoordinate >= lasty) {
      digitalWrite(ydirpin, HIGH);
    } else if (ycoordinate < lasty) {
      digitalWrite(ydirpin, LOW);
    }
  
    EEPROM.write(yaddress, ycoordinate); 
  
    delay(100);
  
    int j;
    
    for (j = 0; j<ydifference; j++)       // Iterate for 4000 microsteps.
      {
        digitalWrite(ysteppin, LOW);  // This LOW to HIGH change is what creates the
        digitalWrite(ysteppin, HIGH); // "Rising Edge" so the easydriver knows to when to step.
        delayMicroseconds(500);      // This delay time is close to top speed for this
      }                              // particular motor. Any faster the motor stalls.
    }
}                              
