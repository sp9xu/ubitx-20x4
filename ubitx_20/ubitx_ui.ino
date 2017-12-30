/**
 * The user interface of the ubitx consists of the encoder, the push-button on top of it
 * and the 16x2 LCD display.
 * The upper line of the display is constantly used to display frequency and status
 * of the radio. Occasionally, it is used to provide a two-line information that is 
 * quickly cleared up.
 */

//returns true if the button is pressed
int btnDown(){
  if (digitalRead(FBUTTON) == HIGH)
    return 0;
  else
    return 1;
}

// Set your callsign here. must be 7 characters, if using shorter call, pad the end with spaces. 
// Do not use a call longer than 7 characters as this will cause display problems
//    eg. "2E0ABC " or "VK7FZZZ" or "K1AA   "
char callsign[8] = "VK3DAN ";

/**
 * Meter (not used in this build for anything) if you wish to use it, you will have to get rid of the custom characters
 * used in ubitx_20.ino, as the addresses overlap and the screen can only hold 8 custom chars.
 * the meter is drawn using special characters. Each character is composed of 5 x 8 matrix.
 * The  s_meter array holds the definition of the these characters. 
 * each line of the array is is one character such that 5 bits of every byte 
 * makes up one line of pixels of the that character (only 5 bits are used)
 * The current reading of the meter is assembled in the string called meter
 */

/*char meter[17];

byte s_meter_bitmap[] = {
  B00000,B00000,B00000,B00000,B00000,B00100,B00100,B11011,
  B10000,B10000,B10000,B10000,B10100,B10100,B10100,B11011,
  B01000,B01000,B01000,B01000,B01100,B01100,B01100,B11011,
  B00100,B00100,B00100,B00100,B00100,B00100,B00100,B11011,
  B00010,B00010,B00010,B00010,B00110,B00110,B00110,B11011,
  B00001,B00001,B00001,B00001,B00101,B00101,B00101,B11011
};



// initializes the custom characters
// we start from char 1 as char 0 terminates the string!
void initMeter(){
  lcd.createChar(1, s_meter_bitmap);
  lcd.createChar(2, s_meter_bitmap + 8);
  lcd.createChar(3, s_meter_bitmap + 16);
  lcd.createChar(4, s_meter_bitmap + 24);
  lcd.createChar(5, s_meter_bitmap + 32);
  lcd.createChar(6, s_meter_bitmap + 40);
}

/**
 * The meter is drawn with special characters.
 * character 1 is used to simple draw the blocks of the scale of the meter
 * characters 2 to 6 are used to draw the needle in positions 1 to within the block
 * This displays a meter from 0 to 100, -1 displays nothing
 */
/*void drawMeter(int8_t needle){
  int16_t best, i, s;

  if (needle < 0)
    return;

  s = (needle * 4)/10;
  for (i = 0; i < 8; i++){
    if (s >= 5)
      meter[i] = 1;
    else if (s >= 0)
      meter[i] = 2 + s;
    else
      meter[i] = 1;
    s = s - 5;
  }
  if (needle >= 40)
    meter[i-1] = 6;
  meter[i] = 0;
}*/

// The generic routine to display one line on the LCD 
void printLine(char linenmbr, char *c) {
  if (strcmp(c, printBuff[linenmbr])) {     // only refresh the display when there was a change
    lcd.setCursor(0, linenmbr);             // place the cursor at the beginning of the selected line
    lcd.print(c);
    strcpy(printBuff[linenmbr], c);

    for (byte i = strlen(c); i < 20; i++) { // add white spaces until the end of the 20 characters line is reached
      lcd.print(' ');
    }
  }
}

//  short cut to print to the first line
void printLine1(char *c){
  printLine(0,c);
}
//  short cut to print to the second line
void printLine2(char *c){
  printLine(1,c);
}

void printLine3(char *c){
  printLine(2,c);
}

void printLine4(char *c){
  printLine(3,c);
}



// this builds up the top line of the display with frequency and mode
void updateDisplay() {
  // tks Jack Purdum W8TEE
  // replaced fsprint commmands by str commands for code size reduction
  memset(c, 0, sizeof(c));
  memset(b, 0, sizeof(b));

  ultoa(frequency, b, DEC);

  if (inTx){
    if (cwTimeout > 0)
      strcpy(c, " CW |");
    else
      if (isUSB)
        strcpy(c, "USB |");
      else
        strcpy(c, "LSB |");
  
    if (vfoActive == VFO_A) // VFO A is active
      strcat(c, " A |");
    else
      strcat(c, " B |");
}  else {
    if (ritOn)
      strcpy(c, "RIT |");
    else {
      if (isUSB)
        strcpy(c, "USB |");
      else
        strcpy(c, "LSB |");
    }
    if (vfoActive == VFO_A) // VFO A is active
      strcat(c, " A |");
    else
      strcat(c, " B |");
  }
/*************
 * This section displays band names, this is set up for Australian Advanced allocations and can easily be changed
 * to suit your own allocations. It also shows a warning when outside the bands but does not inhibit TX,
 * This is only a warning to remind you of band edges at a glance.
 * It is  up to you as a licensed amateur to ensure you only transmit where you are allowed to do so.
 */
  if (frequency >= 3500000 && frequency <= 3700000){
      strcat(c, "  80 m  ");
    } else if (frequency >= 3776000 && frequency <= 3800000){
      strcat(c, "80 m DXw");
    } else if (frequency >= 7000000 && frequency <= 7300000){
      strcat(c, "  40 m  ");
    } else if (frequency >= 10100000 && frequency <= 10150000){
      strcat(c, "  30 m  ");
    } else if (frequency >= 14000000 && frequency <= 14350000){
      strcat(c, "  20 m  ");
    } else if (frequency >= 18068000 && frequency <= 18168000){
      strcat(c, "  17 m  ");
    } else if (frequency >= 21000000 && frequency <= 21450000){
      strcat(c, "  15 m  ");
    } else if (frequency >= 24890000 && frequency <= 24990000){
      strcat(c, "  12 m  ");
    } else if (frequency >= 28000000 && frequency <= 29700000){
      strcat(c, "  10 m  ");
    } else {
      strcat(c, "OOB: \x03\x04\!");
  }
      

  if (inTx)
  {  strcat(c, "| \xF8");
  }
    else
  {  strcat(c, "|\xF8\ ");
  }

  printLine1("Mode|VFO|  Band  |\x01\x02");
  printLine(1, c);
  memset(c, 0, sizeof(c));
  strcpy(c, callsign);
  if (Serial.available() > 0){
    strcat(c, "      CAT control");
  }else{
    strcat(c, "        \xE4\BitX");
  }
  printLine(3, c);


  //one mhz digit if less than 10 M, two digits if more
  memset(c, 0, sizeof(c));
    strcpy(c, "  Freq: ");

  if (frequency < 10000000l){
    c[8] = ' ';
    c[9]  = b[0];
    strcat(c, ".");
    strncat(c, &b[1], 3);    
    strcat(c, ".");
    strncat(c, &b[4], 3);
  }
  else {
    strncat(c, b, 2);
    strcat(c, ".");
    strncat(c, &b[2], 3);
    strcat(c, ".");
    strncat(c, &b[5], 3);    
  }


  printLine(2, c);

/*
  //now, the second line
  memset(c, 0, sizeof(c));
  memset(b, 0, sizeof(b));

  if (inTx)
    strcat(c, "TX ");
  else if (ritOn)
    strcpy(c, "RIT");

  strcpy(c, "      \xff");
  drawMeter(meter_reading);
  strcat(c, meter);
  strcat(c, "\xff");
  printLine2(c);*/
}

int enc_prev_state = 3;

/**
 * The A7 And A6 are purely analog lines on the Arduino Nano
 * These need to be pulled up externally using two 10 K resistors
 * 
 * There are excellent pages on the Internet about how these encoders work
 * and how they should be used. We have elected to use the simplest way
 * to use these encoders without the complexity of interrupts etc to 
 * keep it understandable.
 * 
 * The enc_state returns a two-bit number such that each bit reflects the current
 * value of each of the two phases of the encoder
 * 
 * The enc_read returns the number of net pulses counted over 50 msecs. 
 * If the puluses are -ve, they were anti-clockwise, if they are +ve, the
 * were in the clockwise directions. Higher the pulses, greater the speed
 * at which the enccoder was spun
 */

byte enc_state (void) {
    return (analogRead(ENC_A) > 500 ? 1 : 0) + (analogRead(ENC_B) > 500 ? 2: 0);
}

int enc_read(void) {
  int result = 0; 
  byte newState;
  int enc_speed = 0;
  
  long stop_by = millis() + 50;
  
  while (millis() < stop_by) { // check if the previous state was stable
    newState = enc_state(); // Get current state  
    
    if (newState != enc_prev_state)
      delay (1);
    
    if (enc_state() != newState || newState == enc_prev_state)
      continue; 
    //these transitions point to the encoder being rotated anti-clockwise
    if ((enc_prev_state == 0 && newState == 2) || 
      (enc_prev_state == 2 && newState == 3) || 
      (enc_prev_state == 3 && newState == 1) || 
      (enc_prev_state == 1 && newState == 0)){
        result--;
      }
    //these transitions point o the enccoder being rotated clockwise
    if ((enc_prev_state == 0 && newState == 1) || 
      (enc_prev_state == 1 && newState == 3) || 
      (enc_prev_state == 3 && newState == 2) || 
      (enc_prev_state == 2 && newState == 0)){
        result++;
      }
    enc_prev_state = newState; // Record state for next pulse interpretation
    enc_speed++;
    delay(1);
  }
  return(result);
}


