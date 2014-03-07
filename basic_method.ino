/*----------------------------------------------------------------------------------------------
This code is meant to be used for other primitive microprocessors since most of them do not have
a specific library for LCD. For Arduino, the LiquidCrystal library should be the first choice, 
because it is much easier and powerful, although the code below also can do most of them.
------------------------------------------------------------------------------------------------*/

// Pin assignment. For other microprocessors, the way may different, but should be the same idea.
const int RS = 42;
const int RW = 40;
const int E = 38;
const int D[8] = {36,34,32,30,28,26,24,22};

// Variable declaration. 
char str[] = "Hello World";  // Used in the lcdPrint function.
unsigned char pattern[8] = {0x0e,0x1b,0x11,0x11,0x11,0x11,0x11,0x1f}; // Used for create your own pattern.

// Write command to LCD 1602. (For other microprocessors, the way to set a pin to low/high may different)
void lcdWriteCommand(unsigned char com){
  digitalWrite(RS,LOW);
  digitalWrite(RW,LOW);
  for (int i = 0; i < 8; i++){
      digitalWrite(D[i],((com >> i)&0x01));
  }
  enablePulse();
}

// Write data to LCD 1602.
void lcdWriteData(unsigned char data){
  digitalWrite(RS,HIGH);
  digitalWrite(RW,LOW);
  for (int i = 0; i < 8; i++){
      digitalWrite(D[i],((data >> i)&0x01));
  }
  enablePulse();
}

// Produce a negative edge at enable bit so that data/command can be sent to LCD 1602. 
void enablePulse(){
  digitalWrite(E,0);
  delay(1); // delay some time (from the data sheet)
  digitalWrite(E,1);
  delay(1);
  digitalWrite(E,0);
}

// Clear the screen.
void lcdClear(){
  lcdWriteCommand(0x01);
}

// Set the cursor back to the upper left corner.
void lcdHome(){
  lcdWriteCommand(0x02); 
}

// Set the cursor to a certain row (1-2) and column (1-16). 
void lcdSetCursor(unsigned int row, unsigned int column){
   unsigned int row_offsets[2] = {0x00,0x40};
   lcdWriteCommand(0x80+row_offsets[row-1]+(column-1));
}

// Shoe a single letter on the screen.
void lcdWrite(char singleletter){
  lcdWriteData(singleletter); 
}

// Print a char array on the screen.
void lcdPrint(char string[]){
  for(int j = 0;; j++){
    if(str[j] == 0x00)
       return; 
    lcdWrite(str[j]);
  }
}

// Setup (initializing) LCD module with some specifications.
void lcdSetup(
unsigned char lines, 
unsigned char displayOnOff,
unsigned char cursorOnOff,
unsigned char cursorBlink, 
unsigned char cursorDre,
unsigned char autoShift){
  lcdWriteCommand(0x30+(lines<<3));
  lcdWriteCommand(0x08+(displayOnOff<<2)+(cursorOnOff<<1)+cursorBlink);
  lcdWriteCommand(0x04+(cursorDre<<1)+autoShift);
  lcdWriteCommand(0x01);
}

// Move the contents (including the cursor) on the screen to the left by one unit. 
void lcdDisplayLeft(){
  lcdWriteCommand(0x18);
}

// Move the contents (including the cursor) on the screen to the right by one unit. 
void lcdDisplayRight(){
  lcdWriteCommand(0x1c);
}

// Move the cursor to the left by one unit.
void lcdCursorShiftLeft(){
  lcdWriteCommand(0x10);
}

// Move the cursor to the right by one unit.
void lcdCursorShiftRight(){
  lcdWriteCommand(0x14);
}

// Creat you own character pattern at the memory location specified by "cellNum."
// Note that you can only create 8 patterns due to the small memory in HD44780.
// Display these patterns by using "lcdWrite" function. Be sure to create your 
// patterns before calling "lcdSetup" function, because sometimes there are 
// something weird happen if you do not follow this order.
void lcdCreatePattern(unsigned char cellNum, unsigned char pattern[]){
  cellNum = cellNum & 0x07;
  lcdWriteCommand(0x40 + cellNum<<3);
  for (int u = 0; u < 8; u++){
    lcdWriteData(pattern[u]);
  }
}

void setup() {
  // Typical pin mode selection, maybe not needed for other microprocessors. 
  pinMode(RS,OUTPUT);
  pinMode(RW,OUTPUT);
  pinMode(E,OUTPUT);
  pinMode(D[0],OUTPUT);
  pinMode(D[1],OUTPUT);
  pinMode(D[2],OUTPUT);
  pinMode(D[3],OUTPUT);
  pinMode(D[4],OUTPUT);
  pinMode(D[5],OUTPUT);
  pinMode(D[6],OUTPUT);
  pinMode(D[7],OUTPUT);
}

void loop() {
  
}
