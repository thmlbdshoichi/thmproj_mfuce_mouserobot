//List of Library Include
//License ID : 6131501016 6131501026 6131501054 6131501066
#include <NewPing.h> //Sonar
//-----------------------
//List of Pin Number
int pinButton = 12; // Declare Number of PIN that is Connected to Yellow Button at OUTPUT
int pinBuzzer_1 = 13; // Declare Number of PIN that is Connected to Buzzer at IN
int pinIRSensor = 2; // Declare Number of PIN that is Connected to Infrared Sensor D0

#define TRIGGER_PIN  11  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     10  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 400 // Maximum distance we want to ping for (in centimeters). 
//Maximum sensor distance is rated at 400-500cm.
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

int DRIVE_EN1 = 6; // Declare Number of PIN that is Connected to DC Motor Driver L298N IN1
int DRIVE_EN2 = 7; // Declare Number of PIN that is Connected to DC Motor Driver L298N IN2
int DRIVE_EN3 = 8; // Declare Number of PIN that is Connected to DC Motor Driver L298N IN3
int DRIVE_EN4 = 9; // Declare Number of PIN that is Connected to DC Motor Driver L298N IN4
//-----------------------
//List of variable used in Program
//Main Variable
boolean isCounter = false;
//-----------------------
//INT Variable for counting

//-----------------------

void setup() 
{
  Serial.begin(9600); //Connected to Serial Monitor
  //Select Pin MODE
  //INPUT
  pinMode(pinButton, INPUT);
  pinMode(pinIRSensor, INPUT);
  attachInterrupt(0, doCounter, FALLING);
  //OUTPUT
  pinMode(pinBuzzer_1, OUTPUT);
  pinMode(DRIVE_EN1, OUTPUT);
  pinMode(DRIVE_EN2, OUTPUT);
  pinMode(DRIVE_EN3, OUTPUT);
  pinMode(DRIVE_EN4, OUTPUT);
  
  digitalWrite(pinBuzzer_1, HIGH); // SET DEFAULT TURN OFF THE BUZZER 
}

void loop() 
{
  int input = digitalRead(pinButton);  // INPUT FROM YELLOW BUTTON IF ACTIVE HIGH THEN PROGRAM START (DEFUALT LOW STATUS)
  if (input == 1)
  {
    Serial.print("Initialize the Program | ");
    Serial.print("Status Button = ");
    Serial.println(input);
    delay(100);
    // 3 Seconds before Drive
    int LEDBlinkbegin = 0;
    int Countdown_1 = 3;
    while (LEDBlinkbegin < 3)
    {
      Serial.print("Begin with in ");
      Serial.print(Countdown_1);
      Serial.println(" Seconds ");
      digitalWrite(pinBuzzer_1, LOW);
      delay(1000);
      digitalWrite(pinBuzzer_1,HIGH);
      LEDBlinkbegin = LEDBlinkbegin +1;
      Countdown_1 --;
      delay(1000);
    }
    // Initialize Sonar
    delay(100);                     // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
    Serial.print("The Distance is : ");
    Serial.print(sonar.ping_cm()); // Send ping, get distance in cm and print result (0 = outside set distance range)
    Serial.println(" cm");
    
      // Drive Forward until closer than xx
      int IRCounter = 0;
      while (sonar.ping_cm() >= 20)
      {
        digitalWrite(DRIVE_EN2, HIGH);
        digitalWrite(DRIVE_EN4, HIGH);
        if (isCounter == true) 
        {
          isCounter = false;
          Serial.println(IRCounter);
          Serial.print("The Distance is : ");
          Serial.print(sonar.ping_cm()); // Send ping, get distance in cm and print result (0 = outside set distance range)
          Serial.println(" cm");
          IRCounter++;
        }
      }
      // STOP After closer than xx stop and delay 2 secs
      digitalWrite(DRIVE_EN2, LOW);
      digitalWrite(DRIVE_EN4, LOW);
      digitalWrite(pinBuzzer_1, LOW);
      delay(3000);
      digitalWrite(pinBuzzer_1, HIGH);
      
      // TURN 180 Degrees
      Serial.println("");
      Serial.println("TURN 180 Degrees STARTED");
      int IRCounterTurn = 0;
      while (IRCounterTurn < 16  ) // TURN 180 Degree
      {
        digitalWrite(DRIVE_EN2, HIGH);
        digitalWrite(DRIVE_EN3, HIGH);
        if (isCounter == true) 
        {
          isCounter = false;
          Serial.println(IRCounterTurn);
          Serial.print("The Distance is : ");
          Serial.print(sonar.ping_cm()); // Send ping, get distance in cm and print result (0 = outside set distance range)
          Serial.println(" cm");
          IRCounterTurn++;
        }
      }
      Serial.println("TURN 180 Degrees FINISHED");
  
      // AFTER TURN 180 STOP AND DELAY FOR 2 SEC
      digitalWrite(DRIVE_EN2, LOW);
      digitalWrite(DRIVE_EN3, LOW);
      digitalWrite(pinBuzzer_1, LOW);
      delay(3000);
      digitalWrite(pinBuzzer_1, HIGH);
  
      // Drive back to where it came from
      int IRCounterBack = 0;
      while (IRCounterBack < IRCounter)
      {
        int IRSensorBack = digitalRead(pinIRSensor);
        digitalWrite(DRIVE_EN2, HIGH);
        digitalWrite(DRIVE_EN4, HIGH);
        if (isCounter == true)
        {
          isCounter = false;
          Serial.println(IRCounterBack);
          Serial.print("The Distance is : ");
          Serial.print(sonar.ping_cm()); // Send ping, get distance in cm and print result (0 = outside set distance range)
          Serial.println(" cm");
          IRCounterBack++;
        }
      }
      digitalWrite(DRIVE_EN2, LOW);
      digitalWrite(DRIVE_EN4, LOW);
      //AFTER DRIVE TO START POSITION STOP AND BLINK
  
      Serial.println("");
      Serial.print("Checking IR Count from Last IRCounter = ");
      Serial.println(IRCounter);
      Serial.print("Checking IR Count from Last IRCounterBack = ");
      Serial.println(IRCounterBack);
      Serial.println("Note that IRCounter must equal to IRCounterBack");
      Serial.println("");
      
      //DO FINAL BLINK
      int LEDBlinkEnd = 0;
      while (LEDBlinkEnd < 6)
      {
        digitalWrite(pinBuzzer_1, LOW);
        delay(100);
        digitalWrite(pinBuzzer_1,HIGH);
        LEDBlinkEnd = LEDBlinkEnd +1;
        delay(100);
      }
    //OUTSIDE INPUT WHILE LOOP
    Serial.println(" ");
    Serial.println("The Program has finished Process Please Push Yellow the button to Start [End Program]");
  }
  else
  {
    Serial.println("Program Robot has not been started yet | Status button = 0");
    delay(2000);
  }
  
}

void doCounter() 
{
  isCounter = true;
}
