/* 
 * Arduino USB Keyboard HID demo
 * Sean Burke <sean@seantburke.com>
 * Created: 5/23/2014
 * Modified: 5/23/2014
 * code+/code-/code keys
 */
 
uint8_t buf[8] = { 
  0 }; 	/* Keyboard report buffer */

#define PIN_ARROW_LEFT 2
#define PIN_ARROW_RIGHT 3
#define PIN_ARROW_UP 4
#define PIN_ARROW_DOWN 5 
#define PIN_ENTER 6
#define PIN_ESC 7
#define PIN_TAB 8
#define PIN_SPACEBAR 9

#define KEY_ARROW_LEFT 80
#define KEY_ARROW_RIGHT 79
#define KEY_ARROW_UP 82
#define KEY_ARROW_DOWN 81 
#define KEY_ENTER 40
#define KEY_ESC 41
#define KEY_TAB 43
#define KEY_SPACEBAR 44

#define DELAY_TIME 200
 
int state = 1;
 
void setup() 
{
  Serial.begin(9600);
  pinMode(PIN_ARROW_LEFT, INPUT);
  pinMode(PIN_ARROW_RIGHT, INPUT);
  pinMode(PIN_ARROW_UP, INPUT);
  pinMode(PIN_ARROW_DOWN, INPUT);
  pinMode(PIN_SPACEBAR, INPUT);
  pinMode(PIN_ENTER, INPUT);
  pinMode(PIN_ESC, INPUT);
  pinMode(PIN_TAB, INPUT);
  // enable internal pull-ups
  
  digitalWrite(PIN_ARROW_LEFT, 1);
  digitalWrite(PIN_ARROW_RIGHT, 1);
  digitalWrite(PIN_ARROW_UP, 1); 
  digitalWrite(PIN_ARROW_DOWN, 1); 
  digitalWrite(PIN_SPACEBAR, 1); 
  digitalWrite(PIN_ENTER, 1); 
  digitalWrite(PIN_ESC, 1);
  digitalWrite(PIN_TAB, 1);
 
  delay(DELAY_TIME);
}
 
void loop() 
{
    state = digitalRead(PIN_ARROW_LEFT);
  if (state != 1) {
    delay(DELAY_TIME);
    buf[2] = KEY_ARROW_LEFT;	  // code left key
    Serial.write(buf, 8);	// Send keypress
    releaseKey();
  } 
 
  state = digitalRead(PIN_ARROW_RIGHT);
  if (state != 1) {
    delay(DELAY_TIME);
    buf[2] = KEY_ARROW_RIGHT;	  // code right key
    Serial.write(buf, 8);	// Send keypress
    releaseKey();
  } 
  
  state = digitalRead(PIN_ARROW_UP);
  if (state != 1) {
    delay(DELAY_TIME);
    buf[2] = KEY_ARROW_UP;	  // code up key
    Serial.write(buf, 8);	// Send keypress
    releaseKey();
  } 
 
  state = digitalRead(PIN_ARROW_DOWN);
  if (state != 1) {
    delay(DELAY_TIME);
    buf[2] = KEY_ARROW_DOWN;	  // code down key
    Serial.write(buf, 8);	// Send keypress
    releaseKey();
  } 
 
  state = digitalRead(PIN_SPACEBAR);
  if (state != 1) {
    delay(DELAY_TIME);
    buf[2] = KEY_SPACEBAR;	  // code key
    Serial.write(buf, 8);	// Send keypress
    releaseKey();
  } 
  
  state = digitalRead(PIN_ESC);
  if (state != 1) {
    delay(DELAY_TIME);
    buf[2] = KEY_ESC;	  // code key
    Serial.write(buf, 8);	// Send keypress
    releaseKey();
  } 
  
  state = digitalRead(PIN_TAB);
  if (state != 1) {
    delay(DELAY_TIME);
    buf[2] = KEY_TAB;	  // code key
    Serial.write(buf, 8);	// Send keypress
    releaseKey();
  } 
  
  
  state = digitalRead(PIN_ENTER);
  if (state != 1) {
    delay(DELAY_TIME);
    buf[2] = 40;	  // code key
    Serial.write(buf, 8);	// Send keypress
    releaseKey();
  } 
}
 
void releaseKey() 
{
  buf[0] = 0; 
  buf[2] = 0;
  Serial.write(buf, 8);	// Release key  
}
