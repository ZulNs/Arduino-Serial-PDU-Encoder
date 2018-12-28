/*
  Serial PDU Encoder
 
 Encodes a line of readable text to PDU hex text.
 Entered text must be terminate by a newline or a carriage return or both characters.
 It's recognize '\t', '\n', '\r', and '\\' characters.
 Any single character after '\' other than four characters above will be ignored.
 
 Created 19 August 2017
 @Gorontalo, Indonesia
 by ZulNs
 
 This example code is in the public domain.
 */

char strBuffer[9]; // for storing max 8 chrs plus null termination chr
byte blockCtr = 0;
byte chrCtr = 0;
boolean isSpecialChr = false;

void setup() {
  Serial.begin(115200);    // the GPRS baud rate 
  Serial.println(F("*** Serial PDU Encoder by ZulNs ***"));
  Serial.println();
  strBuffer[8] = 0;
}

void loop() {
  byte chr;
  while (Serial.available()) {
    chr = Serial.read();
    if (chr == '\n' || chr == '\r') {
      if (blockCtr > 0) {
        strBuffer[blockCtr] = 0;
        encode2Pdu();
        printHex();
        //Serial.print(strBuffer);
        chrCtr += blockCtr;
        blockCtr = 0;
      }
      if (chrCtr > 0) {
        Serial.print(F("\nLength: "));
        Serial.print(chrCtr, DEC);
        Serial.print(F(" chrs\n\n"));
        chrCtr = 0;
      }
      isSpecialChr = false;
      break;
    }
    if (chr == '\\' && !isSpecialChr) {
      isSpecialChr = true;
      break;
    }
    if (isSpecialChr) {
      isSpecialChr = false;
      if (chr == 't')
        chr = '\t';
      else if (chr == 'n')
        chr = '\n';
      else if (chr == 'r')
        chr = '\r';
      else if (chr != '\\')
        break;
    }
    strBuffer[blockCtr++] = chr;
    if (blockCtr == 8) {
      encode2Pdu();
      printHex();
      //Serial.print(strBuffer);
      chrCtr += 8;
      blockCtr = 0;
    }
  }
}

void encode2Pdu() {
  byte ctr = 0;
  uint16_t wrd = 0;
  while (strBuffer[ctr + 1] != 0) {
    wrd = strBuffer[ctr + 1] << 8;
    wrd >>= ctr + 1;
    strBuffer[ctr] |= lowByte(wrd);
    strBuffer[++ctr] = highByte(wrd);
  }
}

void printHex() {
  byte ctr = 0;
  while (strBuffer[ctr] != 0) {
    Serial.print((byte)strBuffer[ctr] >> 4, HEX);
    Serial.print((byte)strBuffer[ctr++] & 0x0F, HEX);
  }
}

