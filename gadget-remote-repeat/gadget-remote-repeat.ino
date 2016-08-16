#include <IRremote.h>

const int recvPin = 11;
const int khz = 39; // observed 39 gives closer to 38.5kHz carrier which remote uses 
IRrecv irrecv(recvPin);
IRsend irsend;
decode_results results;

// Storage for the recorded code
unsigned int rawCodes[RAWBUF]; // The durations if raw
int codeLen; // The length of the code

// Stores the code for later playback
// Most of this code is just logging
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
  Serial.println("Saved raw code.");
}

void sendCode() {
  irsend.sendRaw(rawCodes, codeLen, khz);
  Serial.println("Sent raw.");
  irrecv.enableIRIn();
}

void setup() {
  Serial.begin(9600);   // Status message will be sent to PC at 9600 baud
  irrecv.enableIRIn();  // Start the receiver
}

void ircode(decode_results *result) {
  // Panasonic has an Address
  if (result->decode_type == PANASONIC) {
    Serial.print(result->address, HEX);
    Serial.print(":");
  }

  // Print Code
  Serial.print(result->value, HEX);
}

void encoding(decode_results *result) {
  switch (result->decode_type) {
    default:
    case UNKNOWN:      Serial.print("UNKNOWN");       break ;
    case NEC:          Serial.print("NEC");           break ;
    case SONY:         Serial.print("SONY");          break ;
    case RC5:          Serial.print("RC5");           break ;
    case RC6:          Serial.print("RC6");           break ;
    case DISH:         Serial.print("DISH");          break ;
    case SHARP:        Serial.print("SHARP");         break ;
    case JVC:          Serial.print("JVC");           break ;
    case SANYO:        Serial.print("SANYO");         break ;
    case MITSUBISHI:   Serial.print("MITSUBISHI");    break ;
    case SAMSUNG:      Serial.print("SAMSUNG");       break ;
    case LG:           Serial.print("LG");            break ;
    case WHYNTER:      Serial.print("WHYNTER");       break ;
    case AIWA_RC_T501: Serial.print("AIWA_RC_T501");  break ;
    case PANASONIC:    Serial.print("PANASONIC");     break ;
    case DENON:        Serial.print("Denon");         break ;
  }
}

void dumpInfo(decode_results *result) {
  // Check if the buffer overflowed
  if (result->overflow) {
    Serial.println("IR code too long. Edit IRremoteInt.h and increase RAWLEN");
    return;
  }

  // Show Encoding standard
  Serial.print("Encoding  : ");
  encoding(result);
  Serial.println("");

  // Show Code & length
  Serial.print("Code      : ");
  ircode(result);
  Serial.print(" (");
  Serial.print(result->bits, DEC);
  Serial.println(" bits)");
}

void repeatCode(decode_results *result) {
  //irsend.sendRaw((const unsigned int*) result->rawbuf, result->rawlen, khz); //needs to have USECPPERTICK 
  sendSamsungPowerCode();
}

void sendSamsungPowerCode() {
  /*
   * Samsung power on
  unsigned int data = 0xE0E040BF;
  */
  unsigned int rawData[67] = {4400,4450, 550,1650, 550,1650, 550,1700, 550,550, 550,550, 550,550, 550,550, 550,550, 600,1650, 500,1700, 550,1650, 550,550, 550,550, 550,600, 550,550, 550,550, 550,550, 550,1650, 550,550, 650,500, 500,600, 550,550, 550,550, 550,550, 550,1650, 550,550, 550,1700, 550,1650, 600,1600, 550,1700, 550,1650, 550,1650, 550};  // SAMSUNG E0E040BF
  irsend.sendRaw(rawData, sizeof(rawData) / sizeof(rawData[0]), khz);
  
  irrecv.enableIRIn();
}

void loop() {

  if (irrecv.decode(&results)) {  // Grab an IR code
    dumpInfo(&results);
    storeCode(&results);
    irrecv.resume(); 
    delay(2000);
    // Prepare for the next value
    sendCode();
  }
  /*
  sendSamsungPowerCode();
  delay(1000);
  */
}
