#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP32Servo.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const int PAN_PIN = 12;         
const int TILT_PIN = 13;        
const int SPEAKER_PIN = 25;     
const int TRIG_PIN = 14;        
const int ECHO_PIN = 27;        

Servo panServo;
Servo tiltServo;

float currentPan = 90.0;
float targetPan = 90.0;
float currentTilt = 90.0;
float targetTilt = 90.0;
float easingFactor = 0.08;      

enum RobotFace { FACE_IDLE, FACE_HAPPY, FACE_SAD, FACE_ANGRY, FACE_CRYING };
RobotFace MochiFace = FACE_IDLE;

unsigned long prevServoMillis = 0;
unsigned long prevBlinkMillis = 0;
unsigned long prevSonicMillis = 0;
unsigned long audioTimer = 0;
unsigned long tearAnimationMillis = 0; 

int currentDistance = 999;
int tearOffset = 0;                    
bool isBlinking = false;
bool soundPlayed = false;

void setup() {
  Serial.begin(9600); 

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { for(;;); }
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 25);
  display.print("SHIPPING SYSTEM v4.0");
  display.display();

  pinMode(SPEAKER_PIN, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);    
  pinMode(ECHO_PIN, INPUT);     

  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1); 
  
  panServo.setPeriodHertz(50);
  panServo.attach(PAN_PIN, 500, 2400);
  panServo.write((int)currentPan);

  tiltServo.setPeriodHertz(50);
  tiltServo.attach(TILT_PIN, 500, 2400);
  tiltServo.write((int)currentTilt);
  delay(500);
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - prevSonicMillis >= 120) {
    prevSonicMillis = currentMillis;
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    long duration = pulseIn(ECHO_PIN, HIGH, 20000); 
    if (duration > 0) currentDistance = duration * 0.034 / 2; 
    else currentDistance = 999; 

    Serial.print("Hand Distance: ");
    Serial.print(currentDistance);
    Serial.println(" cm");
  }

  if (currentDistance > 2 && currentDistance <= 12) {
    targetPan = 135.0; targetTilt = 85.0;
    if (MochiFace != FACE_ANGRY) { MochiFace = FACE_ANGRY; soundPlayed = false; }
  } 
  else if (currentDistance > 12 && currentDistance <= 25) {
    targetPan = 90.0; targetTilt = 95.0;
    if (MochiFace != FACE_HAPPY) { MochiFace = FACE_HAPPY; soundPlayed = false; }
  } 
  else if (currentDistance > 25 && currentDistance <= 40) {
    targetPan = 45.0; targetTilt = 80.0;
    if (MochiFace != FACE_SAD) { MochiFace = FACE_SAD; soundPlayed = false; }
  } 
  else {
    if (MochiFace != FACE_CRYING) { MochiFace = FACE_CRYING; soundPlayed = false; }
    static float wobble = 0; wobble += 0.5;
    targetPan = 90.0 + (sin(wobble) * 35.0);  
    targetTilt = 60.0 + (cos(wobble) * 15.0); 
  }

  if (MochiFace == FACE_IDLE) {
    if (currentMillis - audioTimer > 80) noTone(SPEAKER_PIN);
    if (!isBlinking && (currentMillis - prevBlinkMillis >= 3000)) {
      isBlinking = true; prevBlinkMillis = currentMillis;
      tone(SPEAKER_PIN, 1500); audioTimer = currentMillis;
    }
    if (isBlinking && (currentMillis - prevBlinkMillis >= 120)) isBlinking = false;
  } 
  else if (MochiFace == FACE_HAPPY && !soundPlayed) {
    soundPlayed = true; tone(SPEAKER_PIN, 900, 70); delay(70); tone(SPEAKER_PIN, 1400, 90);
  } 
  else if (MochiFace == FACE_SAD && !soundPlayed) {
    soundPlayed = true; tone(SPEAKER_PIN, 400, 200); delay(200); tone(SPEAKER_PIN, 200, 200);
  } 
  else if (MochiFace == FACE_ANGRY && !soundPlayed) {
    soundPlayed = true; tone(SPEAKER_PIN, 180, 150);
  }
  else if (MochiFace == FACE_CRYING) {
    static int sirenFreq = 300; static int sirenDirection = 15;
    sirenFreq += sirenDirection;
    if (sirenFreq >= 900 || sirenFreq <= 300) sirenDirection = -sirenDirection;
    tone(SPEAKER_PIN, sirenFreq);
  }

  if (currentMillis - prevServoMillis >= 15) {
    prevServoMillis = currentMillis;
    if (MochiFace != FACE_CRYING) {
      currentPan = currentPan + (targetPan - currentPan) * easingFactor;
      currentTilt = currentTilt + (targetTilt - currentTilt) * easingFactor;
    } else {
      currentPan = targetPan; currentTilt = targetTilt;
    }
    panServo.write((int)currentPan);
    tiltServo.write((int)currentTilt);
  }

  if (currentMillis - prevServoMillis >= 80) {
    tearOffset = (tearOffset + 2) % 16; 
  }

  display.clearDisplay();
  switch (MochiFace) {
    case FACE_IDLE:   if (isBlinking) drawCuteBlink(); else drawCuteNormal(); break;
    case FACE_HAPPY:  drawCuteHappy(); break;
    case FACE_SAD:    drawCuteSad(); break;
    case FACE_ANGRY:  drawCuteAngry(); break;
    case FACE_CRYING: drawCuteCrying(); break; 
  }
  display.display();
}

void drawCuteNormal() {
  display.fillRoundRect(20, 14, 24, 28, 4, SSD1306_WHITE); display.fillRoundRect(84, 14, 24, 28, 4, SSD1306_WHITE); 
  display.drawLine(54, 45, 57, 49, SSD1306_WHITE); display.drawLine(57, 49, 71, 49, SSD1306_WHITE); display.drawLine(71, 49, 74, 45, SSD1306_WHITE);
}
void drawCuteBlink() {
  display.fillRoundRect(20, 26, 24, 6, 2, SSD1306_WHITE); display.fillRoundRect(84, 26, 24, 6, 2, SSD1306_WHITE); 
  display.drawLine(54, 45, 57, 49, SSD1306_WHITE); display.drawLine(57, 49, 71, 49, SSD1306_WHITE); display.drawLine(71, 49, 74, 45, SSD1306_WHITE);
}
void drawCuteHappy() {
  display.fillRoundRect(20, 20, 24, 10, 3, SSD1306_WHITE); display.fillRect(20, 25, 24, 10, SSD1306_BLACK); 
  display.fillRoundRect(84, 20, 24, 10, 3, SSD1306_WHITE); display.fillRect(84, 25, 24, 10, SSD1306_BLACK); 
  display.fillTriangle(52, 44, 76, 44, 64, 57, SSD1306_WHITE);
}
void drawCuteSad() {
  display.fillRoundRect(20, 18, 24, 24, 4, SSD1306_WHITE); display.fillRoundRect(84, 18, 24, 24, 4, SSD1306_WHITE); 
  display.fillRect(20, 18, 10, 6, SSD1306_BLACK); display.fillRect(98, 18, 10, 6, SSD1306_BLACK);
  display.drawLine(56, 49, 64, 45, SSD1306_WHITE); display.drawLine(64, 45, 72, 49, SSD1306_WHITE);
}
void drawCuteAngry() {
  display.fillRoundRect(20, 18, 24, 24, 4, SSD1306_WHITE); display.fillRoundRect(84, 18, 24, 24, 4, SSD1306_WHITE); 
  display.fillRect(20, 14, 24, 7, SSD1306_BLACK); display.fillRect(84, 14, 24, 7, SSD1306_BLACK);
  display.drawFastHLine(56, 47, 16, SSD1306_WHITE);
}
void drawCuteCrying() {
  display.fillRoundRect(20, 18, 24, 24, 4, SSD1306_WHITE); display.fillRoundRect(84, 18, 24, 24, 4, SSD1306_WHITE); 
  display.fillRect(20, 18, 12, 6, SSD1306_BLACK); display.fillRect(96, 18, 12, 6, SSD1306_BLACK);
  display.drawLine(54, 49, 58, 45, SSD1306_WHITE); display.drawLine(58, 45, 62, 49, SSD1306_WHITE); display.drawLine(62, 49, 66, 45, SSD1306_WHITE);
  display.drawFastVLine(30, 44 + tearOffset, 8, SSD1306_WHITE); display.drawFastVLine(92, 44 + tearOffset, 8, SSD1306_WHITE);
}