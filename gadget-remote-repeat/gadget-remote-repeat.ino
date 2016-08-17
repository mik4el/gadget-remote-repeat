#include <IRremote.h>

#define STATUS_PIN 7
#define RECEIVE_PIN 11
#define KHZ 39 // observed that 39 gives closer to 38.5kHz carrier which remote uses on nano
//Nano send pin 3

IRrecv irrecv(RECEIVE_PIN);
IRsend irsend;
decode_results results;

// Storage for the recorded code
unsigned int rawCodes[RAWBUF]; // The durations if raw
int codeLen; // The length of the code

void setup() {
  Serial.begin(9600);   // Status message will be sent to PC at 9600 baud
  irrecv.enableIRIn();  // Start the receiver
  Serial.println("Setup complete!");
  pinMode(STATUS_PIN, OUTPUT);
}

void storeCode(decode_results *results) {
  int count = results->rawlen;
  codeLen = results->rawlen - 1;
  // To store raw codes:
  // Drop first value (gap)
  // Convert from ticks to microseconds
  // Tweak marks shorter, and spaces longer to cancel out IR receiver distortion
  for (int i = 1; i <= codeLen; i++) {
    if (i % 2) {
      // Mark
      rawCodes[i - 1] = results->rawbuf[i]*USECPERTICK - MARK_EXCESS;
    } 
    else {
      // Space
      rawCodes[i - 1] = results->rawbuf[i]*USECPERTICK + MARK_EXCESS;
    }
  }
  Serial.println("Saved raw.");
}

void sendCode() {
  irsend.sendRaw(rawCodes, codeLen, KHZ);
  Serial.println("Sent raw.");
  irrecv.enableIRIn();
}

void sendSamsungPowerCode() {
  /*
   * Samsung power on
   * unsigned int data = 0xE0E040BF;
   * For debug purposes.
  */
  unsigned int rawData[67] = {4400,4450, 550,1650, 550,1650, 550,1700, 550,550, 550,550, 550,550, 550,550, 550,550, 600,1650, 500,1700, 550,1650, 550,550, 550,550, 550,600, 550,550, 550,550, 550,550, 550,1650, 550,550, 650,500, 500,600, 550,550, 550,550, 550,550, 550,1650, 550,550, 550,1700, 550,1650, 600,1600, 550,1700, 550,1650, 550,1650, 550};  // SAMSUNG E0E040BF
  irsend.sendRaw(rawData, sizeof(rawData) / sizeof(rawData[0]), KHZ);
  irrecv.enableIRIn();
}

void loop() {

  if (irrecv.decode(&results)) {  // Grab an IR code
    storeCode(&results);
    irrecv.resume(); 
    digitalWrite(STATUS_PIN, HIGH);
    delay(50);
    digitalWrite(STATUS_PIN, LOW);
    // Prepare for the next value
    sendCode();
  }
  /* Debug
  sendSamsungPowerCode();
  delay(1000);
  */
}
