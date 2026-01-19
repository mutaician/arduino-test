/*
  SMART CLOCK & TIMER FIRMWARE
  - Protocol: 'C' for Clock update, 'T' for Timer start, 'X' for Stop.
*/

// --- PINS ---
const int segA = 2; const int segB = 3; const int segC = 4;
const int segD = 5; const int segE = 6; const int segF = 7; const int segG = 8;
const int d1 = 10; const int d2 = 11; const int d3 = 12; 
const int d4 = 9;  // Digit 4 on Pin 9
const int buzzerPin = 13;

// --- STATE VARIABLES ---
enum Mode { MODE_CLOCK, MODE_TIMER, MODE_ALARM };
Mode currentMode = MODE_CLOCK;

// Display Data
int displayDigits[4] = {0, 0, 0, 0}; // Holds the 4 numbers to show

// Timer Variables
long timerSeconds = 0;
unsigned long previousMillis = 0;

// Digits 0-9 Patterns
const byte numbers[10][7] = {
  {1, 1, 1, 1, 1, 1, 0}, {0, 1, 1, 0, 0, 0, 0}, {1, 1, 0, 1, 1, 0, 1}, {1, 1, 1, 1, 0, 0, 1},
  {0, 1, 1, 0, 0, 1, 1}, {1, 0, 1, 1, 0, 1, 1}, {1, 0, 1, 1, 1, 1, 1}, {1, 1, 1, 0, 0, 0, 0},
  {1, 1, 1, 1, 1, 1, 1}, {1, 1, 1, 1, 0, 1, 1}
};

void setup() {
  for(int i=2; i<=12; i++) pinMode(i, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  
  // Startup Sequence: Show dashes
  loadDisplay(11, 11, 11, 11);
  Serial.begin(9600);
}

void loop() {
  // --- 1. LISTEN FOR COMMANDS ---
  if (Serial.available() > 0) {
    char cmd = Serial.read(); // Read first letter (C, T, or X)
    int value = Serial.parseInt(); // Read the number following it

    // Consume newline
    while(Serial.available() > 0) Serial.read();

    if (cmd == 'C') {
       // CLOCK MODE: Received HHMM (e.g. 1230)
       currentMode = MODE_CLOCK;
       noTone(buzzerPin);
       updateDisplayFromNumber(value);
    }
    else if (cmd == 'T') {
       // TIMER MODE: Received Seconds (e.g. 90)
       currentMode = MODE_TIMER;
       timerSeconds = value;
       noTone(buzzerPin);
       updateDisplayFromTimer(timerSeconds);
    }
    else if (cmd == 'X') {
       // STOP / RESET
       currentMode = MODE_CLOCK;
       noTone(buzzerPin);
       // We will wait for next 'C' command to update display
    }
  }

  // --- 2. HANDLE TIME LOGIC ---
  unsigned long currentMillis = millis();
  
  if (currentMode == MODE_TIMER) {
     if (currentMillis - previousMillis >= 1000) {
       previousMillis = currentMillis;
       if (timerSeconds > 0) {
         timerSeconds--;
         updateDisplayFromTimer(timerSeconds);
         Serial.println(timerSeconds); // Feedback to web
       } else {
         currentMode = MODE_ALARM;
         Serial.println("DONE");
       }
     }
  }

  // --- 3. HANDLE ALARM ---
  if (currentMode == MODE_ALARM) {
      // Casio Double Beep
      int rhythm = millis() % 1000;
      if ((rhythm >= 0 && rhythm < 100) || (rhythm >= 200 && rhythm < 300)) {
         tone(buzzerPin, 1046);
      } else {
         noTone(buzzerPin);
      }
      // Blink "0000"
      if (millis() % 500 < 250) updateDisplayFromNumber(0);
      else loadDisplay(10,10,10,10); // Blank
  }

  // --- 4. REFRESH DISPLAY ---
  renderDisplay();
}

// --- HELPERS ---

void updateDisplayFromNumber(int num) {
  // Expects HHMM format (e.g. 1230)
  displayDigits[0] = (num / 1000) % 10;
  displayDigits[1] = (num / 100) % 10;
  displayDigits[2] = (num / 10) % 10;
  displayDigits[3] = num % 10;
}

void updateDisplayFromTimer(long totalSecs) {
  // Converts Seconds to MM:SS
  int mins = totalSecs / 60;
  int secs = totalSecs % 60;
  displayDigits[0] = (mins / 10) % 10;
  displayDigits[1] = mins % 10;
  displayDigits[2] = (secs / 10) % 10;
  displayDigits[3] = secs % 10;
}

void loadDisplay(int a, int b, int c, int d) {
  displayDigits[0] = a; displayDigits[1] = b; 
  displayDigits[2] = c; displayDigits[3] = d;
}

void renderDisplay() {
  int pins[] = {d1, d2, d3, d4};
  for (int i=0; i<4; i++) {
     // Turn OFF all digits
     digitalWrite(d1, HIGH); digitalWrite(d2, HIGH); 
     digitalWrite(d3, HIGH); digitalWrite(d4, HIGH);

     // Skip if number is 10 (Blank)
     if (displayDigits[i] != 10) {
       // Set segments
       // Special case: If number is 11, show Dash (Optional, using index 0 for now)
       int num = displayDigits[i];
       digitalWrite(segA, numbers[num][0]);
       digitalWrite(segB, numbers[num][1]);
       digitalWrite(segC, numbers[num][2]);
       digitalWrite(segD, numbers[num][3]);
       digitalWrite(segE, numbers[num][4]);
       digitalWrite(segF, numbers[num][5]);
       digitalWrite(segG, numbers[num][6]);
       
       // Turn ON specific digit
       digitalWrite(pins[i], LOW);
     }
     delay(3); 
  }
}