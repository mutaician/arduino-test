/*
  4-Digit Counter (0-9999)
  Uses millis() to count seconds while keeping the display active.
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
// Note: Digit 4 is on Pin 9 to avoid the Pin 13 LED conflict
const int d1 = 10;
const int d2 = 11;
const int d3 = 12;
const int d4 = 9; 

// --- VARIABLES ---
int counter = 0;          // The number we are displaying
unsigned long previousTime = 0; // Stores the last time we updated the counter
const long interval = 1000;     // 1000ms = 1 second speed

// Number Patterns (Common Cathode: 1=ON)
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
  // Don't forget pin 9 for Digit 4 if you moved it
  pinMode(9, OUTPUT); 

  // Start with digits OFF
  digitalWrite(d1, HIGH);
  digitalWrite(d2, HIGH);
  digitalWrite(d3, HIGH);
  digitalWrite(d4, HIGH);
}

void loop() {
  // --- PART 1: The Timer (Game Logic) ---
  unsigned long currentTime = millis();
  
  if (currentTime - previousTime >= interval) {
    previousTime = currentTime;
    counter++;
    if (counter > 9999) counter = 0; // Reset at 9999
  }

  // --- PART 2: The Display (Visual Logic) ---
  // We must run this constantly. No long delays allowed here!
  
  // Extract digits from the counter (e.g. 1234)
  int thousands = (counter / 1000) % 10;  // 1
  int hundreds  = (counter / 100) % 10;   // 2
  int tens      = (counter / 10) % 10;    // 3
  int ones      = counter % 10;           // 4

  // Multiplexing Cycle
  displayDigit(d1, thousands);
  delay(4); // Short delay for brightness
  displayDigit(d2, hundreds);
  delay(4);
  displayDigit(d3, tens);
  delay(4);
  displayDigit(d4, ones);
  delay(4);
}

void displayDigit(int digitPin, int number) {
  // 1. Turn OFF all digits to prevent ghosting
  digitalWrite(d1, HIGH);
  digitalWrite(d2, HIGH);
  digitalWrite(d3, HIGH);
  digitalWrite(d4, HIGH);

  // 2. Set Segments
  digitalWrite(segA, numbers[number][0]);
  digitalWrite(segB, numbers[number][1]);
  digitalWrite(segC, numbers[number][2]);
  digitalWrite(segD, numbers[number][3]);
  digitalWrite(segE, numbers[number][4]);
  digitalWrite(segF, numbers[number][5]);
  digitalWrite(segG, numbers[number][6]);

  // 3. Turn ON the active digit
  digitalWrite(digitPin, LOW);
}