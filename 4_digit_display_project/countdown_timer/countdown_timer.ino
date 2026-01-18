/*
  FINAL ROBUST TIMER
  - Non-Blocking Serial Buffer (Fixes "Start" failure)
  - Casio Double-Beep Alarm
  - Pins: Digits 10,11,12,9 | Segments 2-8 | Buzzer 13
*/

// --- PINS ---
const int segA = 2; const int segB = 3; const int segC = 4;
const int segD = 5; const int segE = 6; const int segF = 7; const int segG = 8;
const int d1 = 10; const int d2 = 11; const int d3 = 12; 
const int d4 = 9;  // Digit 4 is on Pin 9
const int buzzerPin = 13; // Buzzer on Pin 13

// --- VARIABLES ---
long totalSeconds = 0;
unsigned long previousMillis = 0;
const long interval = 1000;
bool isRunning = false;
bool isAlarming = false;

// Serial Buffer
String inputString = "";      // Holds incoming characters
bool stringComplete = false;  // Flags when \n is received

// Digits 0-9
const byte numbers[10][7] = {
  {1, 1, 1, 1, 1, 1, 0}, {0, 1, 1, 0, 0, 0, 0}, {1, 1, 0, 1, 1, 0, 1}, {1, 1, 1, 1, 0, 0, 1},
  {0, 1, 1, 0, 0, 1, 1}, {1, 0, 1, 1, 0, 1, 1}, {1, 0, 1, 1, 1, 1, 1}, {1, 1, 1, 0, 0, 0, 0},
  {1, 1, 1, 1, 1, 1, 1}, {1, 1, 1, 1, 0, 1, 1}
};

void setup() {
  for(int i=2; i<=12; i++) pinMode(i, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  noTone(buzzerPin);
  digitalWrite(d1, HIGH); digitalWrite(d2, HIGH);
  digitalWrite(d3, HIGH); digitalWrite(d4, HIGH);

  Serial.begin(9600);
  inputString.reserve(10); // Pre-allocate memory
  Serial.println("READY");
}

void loop() {
  // --- 1. READ DATA (Character by Character) ---
  // This never pauses the loop.
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    // If it's a number, add it to buffer
    if (isDigit(inChar)) {
      inputString += inChar;
    }
    // If it's a newline (Command Finished)
    if (inChar == '\n') {
      stringComplete = true;
    }
  }

  // --- 2. PROCESS COMMAND ---
  if (stringComplete) {
    if (inputString.length() > 0) {
      int val = inputString.toInt();
      if (val > 0) {
        // START
        totalSeconds = val;
        isRunning = true;
        isAlarming = false;
        Serial.println(totalSeconds);
      } else {
        // STOP (0 received)
        totalSeconds = 0;
        isRunning = false;
        isAlarming = false;
        noTone(buzzerPin);
        Serial.println(0);
      }
    }
    // Reset Buffer
    inputString = "";
    stringComplete = false;
  }

  // --- 3. TIMER LOGIC ---
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
    if (isRunning && totalSeconds > 0) {
      totalSeconds--;
      Serial.println(totalSeconds);
      if (totalSeconds == 0) {
        isRunning = false;
        isAlarming = true;
      }
    }
  }

  // --- 4. CASIO ALARM LOGIC ---
  if (isAlarming) {
    int rhythm = millis() % 1000;
    // Use 1046Hz (High C) or 880Hz (A5)
    int pitch = 4096;
    
    // Pattern: BEEP (0-100) ... BEEP (200-300) ... Silence
    if ((rhythm >= 0 && rhythm < 100) || (rhythm >= 200 && rhythm < 300)) {
       tone(buzzerPin, pitch);
    } else {
       noTone(buzzerPin);
    }
  } else {
    noTone(buzzerPin);
  }

  // --- 5. DISPLAY REFRESH ---
  refreshDisplay(totalSeconds);
}

void refreshDisplay(int val) {
  int mins = val / 60;
  int secs = val % 60;
  
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
  digitalWrite(d1, HIGH); digitalWrite(d2, HIGH);
  digitalWrite(d3, HIGH); digitalWrite(d4, HIGH);
  digitalWrite(segA, numbers[number][0]);
  digitalWrite(segB, numbers[number][1]);
  digitalWrite(segC, numbers[number][2]);
  digitalWrite(segD, numbers[number][3]);
  digitalWrite(segE, numbers[number][4]);
  digitalWrite(segF, numbers[number][5]);
  digitalWrite(segG, numbers[number][6]);
  digitalWrite(digitPin, LOW);
}