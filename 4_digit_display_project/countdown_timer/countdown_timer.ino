/*
  Arduino Countdown Timer (MM:SS Format)
  Input: Type seconds into Serial Monitor (e.g., 90 -> 01:30)
*/

// --- PINS ---
// Segments A-G (via Resistors)
const int segA = 2;
const int segB = 3;
const int segC = 4;
const int segD = 5;
const int segE = 6;
const int segF = 7;
const int segG = 8;

// Digits (Direct connection)
const int d1 = 10; // Tens of Minutes
const int d2 = 11; // Minutes
const int d3 = 12; // Tens of Seconds
const int d4 = 9;  // Seconds (MOVED TO PIN 9)

// --- VARIABLES ---
long totalSeconds = 0;          // The main counter
unsigned long previousMillis = 0;
const long interval = 1000;     // 1 second speed

// Number Patterns (0-9)
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
  // Setup Pins
  for(int i=2; i<=12; i++) pinMode(i, OUTPUT);
  pinMode(9, OUTPUT); 
  
  // Digits OFF
  digitalWrite(d1, HIGH);
  digitalWrite(d2, HIGH);
  digitalWrite(d3, HIGH);
  digitalWrite(d4, HIGH);
  
  // Start Serial Monitor
  Serial.begin(9600);
  Serial.println("ENTER SECONDS TO COUNTDOWN:");
}

void loop() {
  // --- PART 1: CHECK FOR INPUT ---
  if (Serial.available() > 0) {
    // Read the integer typed by the user
    int inputTime = Serial.parseInt();
    
    // Clear the buffer (ignore newline characters)
    while (Serial.available() > 0) Serial.read();
    
    if (inputTime > 0) {
       totalSeconds = inputTime;
       Serial.print("Timer Set: ");
       Serial.println(totalSeconds);
    }
  }

  // --- PART 2: THE CLOCK (Tick Tock) ---
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
    // Only count down if we have time remaining
    if (totalSeconds > 0) {
      totalSeconds--;
      
      // Optional: Beep logic could go here
      if (totalSeconds == 0) {
        Serial.println("DONE!");
      }
    }
  }

  // --- PART 3: THE MATH (Seconds -> MM:SS) ---
  // Example: 90 seconds
  // Minutes = 90 / 60 = 1
  // Seconds = 90 % 60 = 30
  
  int minutes = totalSeconds / 60;
  int seconds = totalSeconds % 60;

  int digit1_val = (minutes / 10) % 10; // Tens of Mins
  int digit2_val = minutes % 10;        // Units of Mins
  int digit3_val = (seconds / 10) % 10; // Tens of Secs
  int digit4_val = seconds % 10;        // Units of Secs

  // --- PART 4: THE DISPLAY (Multiplexing) ---
  displayDigit(d1, digit1_val);
  delay(4);
  displayDigit(d2, digit2_val);
  delay(4);
  displayDigit(d3, digit3_val);
  delay(4);
  displayDigit(d4, digit4_val);
  delay(4);
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