/*
  4-Digit Countdown Timer with SIREN ALARM
  - Input: Type seconds in Serial Monitor (e.g., 10)
  - Output: Countdown -> Siren on Pin 13
*/

// --- PINS ---
// Segments A-G (Via Resistors)
const int segA = 2;
const int segB = 3;
const int segC = 4;
const int segD = 5;
const int segE = 6;
const int segF = 7;
const int segG = 8;

// Digits (Direct connection)
const int d1 = 10;
const int d2 = 11;
const int d3 = 12;
const int d4 = 9;  // Keep this on 9!

const int buzzerPin = 13; // NEW: Buzzer on Pin 13

// --- VARIABLES ---
long totalSeconds = 0;
unsigned long previousMillis = 0;
const long interval = 1000;

// Alarm State
bool isRunning = false;
bool isAlarming = false;

// Digits 0-9 Pattern
const byte numbers[10][7] = {
  {1, 1, 1, 1, 1, 1, 0}, // 0
  {0, 1, 1, 0, 0, 0, 0}, // 1
  {1, 1, 0, 1, 1, 0, 1}, // 2
  {1, 1, 1, 1, 0, 0, 1}, // 3
  {0, 1, 1, 0, 0, 1, 1}, // 4
  {1, 0, 1, 1, 0, 1, 1}, // 5
  {1, 0, 1, 1, 1, 1, 1}, // 6
  {1, 1, 1, 0, 0, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1}, // 8
  {1, 1, 1, 1, 0, 1, 1}  // 9
};

void setup() {
  for(int i=2; i<=12; i++) pinMode(i, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  // Digits OFF
  digitalWrite(d1, HIGH); digitalWrite(d2, HIGH);
  digitalWrite(d3, HIGH); digitalWrite(d4, HIGH);

  Serial.begin(9600);
  Serial.println("READY: Enter Seconds to Start.");
}

void loop() {
  // --- 1. INPUT HANDLER ---
  if (Serial.available() > 0) {
    int input = Serial.parseInt();
    // Clear excess characters
    while(Serial.available() > 0) Serial.read();

    if (input > 0) {
      totalSeconds = input;
      isRunning = true;
      isAlarming = false;
      Serial.print("Counting down: "); Serial.println(input);
    } else {
      // Typing '0' kills the alarm
      totalSeconds = 0;
      isRunning = false;
      isAlarming = false;
      noTone(buzzerPin); // Silence immediately
      Serial.println("STOPPED.");
    }
  }

  // --- 2. TIMER LOGIC ---
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
    if (isRunning && totalSeconds > 0) {
      totalSeconds--;
      if (totalSeconds == 0) {
        isRunning = false;
        isAlarming = true; // Trigger the siren
        Serial.println("TIME UP! (Type 0 to stop)");
      }
    }
  }

// --- 3. CASIO WATCH ALARM LOGIC ---
  if (isAlarming) {
    // Cycle duration: 1000ms (1 second)
    int rhythm = millis() % 1000;
    
    // The Frequency: 1046Hz is a crisp "Digital Watch" High C
    int pitch = 4096;

    // Beep 1: 0 to 100ms
    if (rhythm >= 0 && rhythm < 100) {
       tone(buzzerPin, pitch);
    }
    // Gap: 100 to 200ms (Silence)
    else if (rhythm >= 100 && rhythm < 200) {
       noTone(buzzerPin);
    }
    // Beep 2: 200 to 300ms
    else if (rhythm >= 200 && rhythm < 300) {
       tone(buzzerPin, pitch);
    }
    // Long Pause: 300 to 1000ms
    else {
       noTone(buzzerPin);
    }
  } else {
    noTone(buzzerPin);
  }

  // --- 4. DISPLAY LOGIC ---
  int mins = totalSeconds / 60;
  int secs = totalSeconds % 60;

  displayDigit(d1, (mins / 10) % 10);
  delay(3);
  displayDigit(d2, mins % 10);
  delay(3);
  displayDigit(d3, (secs / 10) % 10);
  delay(3);
  displayDigit(d4, secs % 10);
  delay(3);
}

void displayDigit(int digitPin, int number) {
  // Turn OFF all digits
  digitalWrite(d1, HIGH); digitalWrite(d2, HIGH);
  digitalWrite(d3, HIGH); digitalWrite(d4, HIGH);

  // Set Segments
  digitalWrite(segA, numbers[number][0]);
  digitalWrite(segB, numbers[number][1]);
  digitalWrite(segC, numbers[number][2]);
  digitalWrite(segD, numbers[number][3]);
  digitalWrite(segE, numbers[number][4]);
  digitalWrite(segF, numbers[number][5]);
  digitalWrite(segG, numbers[number][6]);

  // Turn ON target digit
  digitalWrite(digitPin, LOW);
}