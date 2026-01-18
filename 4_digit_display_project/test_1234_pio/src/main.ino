// Simple Test for 5641AS (Common Cathode)

// 1. Define the Segment Pins (Connected via Resistors)
const int segA = 2;
const int segB = 3;
const int segC = 4;
const int segD = 5;
const int segE = 6;
const int segF = 7;
const int segG = 8;

// 2. Define the Digit Control Pins (Connected Directly)
const int d1 = 10; // Leftmost digit
const int d2 = 11;
const int d3 = 12;
const int d4 = 13; // Rightmost digit

// This array stores the pattern for numbers 0-9
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
  // Set all pins to OUTPUT
  for(int i=2; i<=8; i++) pinMode(i, OUTPUT);
  for(int i=10; i<=13; i++) pinMode(i, OUTPUT);
  
  // Start with all digits OFF (HIGH = OFF for Common Cathode)
  digitalWrite(d1, HIGH);
  digitalWrite(d2, HIGH);
  digitalWrite(d3, HIGH);
  digitalWrite(d4, HIGH);
}

void loop() {
  // We blink each digit in sequence very fast
  drawDigit(d1, 1); // Show '1' on Digit 1
  delay(5);
  drawDigit(d2, 2); // Show '2' on Digit 2
  delay(5);
  drawDigit(d3, 3); // Show '3' on Digit 3
  delay(5);
  drawDigit(d4, 4); // Show '4' on Digit 4
  delay(5);
}

void drawDigit(int digitPin, int number) {
  // A. Turn OFF all digits to prevent "Ghosting"
  digitalWrite(d1, HIGH);
  digitalWrite(d2, HIGH);
  digitalWrite(d3, HIGH);
  digitalWrite(d4, HIGH);

  // B. Set the segments for the specific number
  digitalWrite(segA, numbers[number][0]);
  digitalWrite(segB, numbers[number][1]);
  digitalWrite(segC, numbers[number][2]);
  digitalWrite(segD, numbers[number][3]);
  digitalWrite(segE, numbers[number][4]);
  digitalWrite(segF, numbers[number][5]);
  digitalWrite(segG, numbers[number][6]);

  // C. Turn ON the single digit we want
  digitalWrite(digitPin, LOW);
}