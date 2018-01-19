#include <Adafruit_NeoPixel.h> //RGB

//-- ULTRASOUND SENSOR VARIABLES --
const int trigPin = 7;
const int echoPin = 8;
int n = 10;

//-- LED STRIP VARIABLES --
const int ledPin =  2;
const int ledNumber = 5;
int luminosity = 10;

// Define strip type
Adafruit_NeoPixel strip = Adafruit_NeoPixel(ledNumber, ledPin, NEO_GRB + NEO_KHZ800);



void setup() 
{
  Serial.begin(9600);

  pinMode(ledPin,  OUTPUT);
  strip.begin();

  //delay(500);            // TEST

  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin, LOW);

  
// Blink 2 times blue  
  for(int i=0; i<2; i++)
  {
    setColorAll(0, 0, 255);
    delay(500);
    setColorAll(0, 0, 0);
    delay(500);
  }
  
}

////////////////////////////////////
//           LOOP                //
///////////////////////////////////
void loop() 
{

  int distance = getDistance();
  Serial.println("Distance is "+String(distance)+" cm nigga!");
  if (distance <= 200)
  {
    sunRise();
    Serial.println("smaller");
  }
  setColorAll(0,0,0);
  
  
//  sunRise();

  delay(200);
}


////////////////////////////////////
//           FUNCTIONS           //
///////////////////////////////////
// ===================== GET DISTANCE ====================================
int getDistance()
{
  unsigned long t1;
  unsigned long t2;
  unsigned long pulse_width;
  float cm;
  float cmTot;
  float distance;

  cmTot = 0;
  for (int i=1; i<=n; i++)
  {
    // Send wave
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    // Listen to echo
    while(digitalRead(echoPin) == 0);
    t1 = micros();
    while(digitalRead(echoPin) == 1);
    t2 = micros();
    pulse_width = t2 - t1;
    // Calculate distance in cm
    cm = pulse_width / 58.0;
    cmTot += cm;
  }
  distance = cmTot/n;
  int distanceInt = (int) distance;
//  Serial.println("Distance is "+String(distance)+" cm nigga!"); 
  return(distance);
}

// ================ SET COLOR TO ALL FUNCTION =============================
void setColorAll(int red, int green, int blue)
{
  // Apply luminosity
  float color[] = {red, green, blue};
  for (int c=0; c<3; c++)
  {
    color[c] = color[c] * luminosity / 255;
  }

  // Set color for all LEDs
  for(int ledNr=0; ledNr<ledNumber; ledNr++)
  {
    strip.setPixelColor(ledNr, strip.Color(color[0], color[1], color[2]));
  }

  strip.show(); 
}

// =============== SUNRISE FUNCTION ===========================
void sunRise()
{
  uint32_t srTime = 1; // Sunrise time (minutes)
  
  int srPatternRed[] = {0,255,255,255};
  int srPatternGreen[] = {0,0,255,255};
  int srPatternBlue[] = {0,0,0,255};
  int patternSize = sizeof(srPatternRed) / sizeof(int);

  // For all sunrise points
  for(int i=0; i<(patternSize-1); i++)
  {
    // Interpolate color points
    int colorSteps = 500;
    for(uint32_t j=0; j<colorSteps; j++)
    {
      int red = srPatternRed[i] + (srPatternRed[i+1] - srPatternRed[i])*j/colorSteps;
      int green = srPatternGreen[i] + (srPatternGreen[i+1] - srPatternGreen[i])*j/colorSteps;
      int blue = srPatternBlue[i] + (srPatternBlue[i+1] - srPatternBlue[i])*j/colorSteps;

      setColorAll(red, green, blue);
      delay(5);
     // delay(srTime * 60 * 1000 / ((patternSize-1) * colorSteps));
    }    
  }
  
  
}







