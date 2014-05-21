/*
 * Alarm Clock Gadgetron Arduino Project
 * Created on: May 15, 2014
 * Modified on: May 20, 2014
 * Author: Sean Thomas Burke
 
 */
 
static float realtime = 100;  //delay for entire system
static float timeout = 10;  //timeout cycles for the alarm
static int clock_delay = 60; //number of seconds in a minute
static int clock_multiplier = 10; //delay multiplier for the clock minutes 
static int clock_increment = 5;  //discrete value interval when setting time
int clock_time = clock_multiplier*clock_delay; //delay for minutes

int seconds = 0; //initialize seconds to zero
int time = 0; //current time in minutes
int alarm = 130; //current alarm in minutes
int clock = 0; //initialize clock to zero
int animation_time = (250); //delay for the animation (not used)

int input[] = {0,1}; //not input anymore, used for alarm and led output
int pins[] = {
  2,3,4,5,6,7,8,9,10,11,12,13};
int values[] = {
  LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW};
byte hours = 12;
byte minutes = 0;
int hours_tens = 0;
byte hours_ones = 0;
byte minutes_tens = 0;
byte minutes_ones = 0;
boolean snooze = false;
int set_alarm = 0;
int dial_input = 0;
int set_time = 0;
int snooze_button = 0;
int alarm_timer = 0;

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  if((alarm + timeout) > 779)
  {
    alarm = 779 - timeout - 1; 
  }
  pinMode(input[0], OUTPUT);
  pinMode(input[1], OUTPUT);

  for(int i=0; i<sizeof(pins); i++)
  {
    pinMode(pins[i], OUTPUT);
    digitalWrite(pins[i], values[i]);
  }  
}

//show the time on the clock, offset makes sure to show 12:00 instead of 0:00
void show(int time, boolean offset)
{
  (time %= 779);
  if(time < 60 && offset)
  {
    time += 12*60;
  }

  hours = floor(time/60);
  minutes = time - hours*60;

  hours_tens = (floor(hours/10)>0)?1:0;
  hours_ones = hours - hours_tens*10;

  minutes_tens = floor(minutes/10);
  minutes_ones = minutes - minutes_tens*10;
  digitalWrite(pins[0], 1-hours_tens);

  for(int i=0; i<4;i++)
  {
    int offset = 1;
    digitalWrite(pins[i+offset], bitRead(hours_ones, i));
  }
  for(int i=0; i<3;i++)
  { 
    int offset = 5;
    digitalWrite(pins[i+offset], bitRead(minutes_tens, i)); 
  }
  for(int i=0; i<4;i++)
  {
    int offset = 8;
    digitalWrite(pins[i+offset], bitRead(minutes_ones, i)); 
  }

}


//Blink the LED and Sound the buzzer
void blinker(int frame){
  frame %= 6;
  if(frame > 3)
  {
    tone(0, 890);
      digitalWrite(1, HIGH);
  }
  else{
    noTone(0);
    digitalWrite(1, LOW);
  }
}

//animate the display, (was used for alarm)
void animate(int frame)
{
  frame = (frame % animation_time*10);
  if(frame < 1*animation_time){
    for(int i=0; i<sizeof(pins); i++)
    {

      tone(0, 890);
      digitalWrite(1, HIGH);
      int values[] = {
        HIGH,HIGH,LOW,LOW,LOW,HIGH,LOW,LOW,HIGH,LOW,LOW,LOW            };
      digitalWrite(pins[i], values[i]);
    }  
  }
  else if(frame < 2*animation_time)
  {
    for(int i=0; i<sizeof(pins); i++)
    {
      int values[] = {
        HIGH,HIGH,LOW,LOW,LOW,HIGH,LOW,LOW,LOW,LOW,LOW,LOW            };
      digitalWrite(pins[i], values[i]);
    }  
  }
  else if(frame < 3*animation_time)
  {
    for(int i=0; i<sizeof(pins); i++)
    {
      int values[] = {
        HIGH,HIGH,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW            };
      digitalWrite(pins[i], values[i]);
    }  
  }
  else if(frame < 4*animation_time)
  {
    for(int i=0; i<sizeof(pins); i++)
    {
      int values[] = {
        HIGH,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW            };
      digitalWrite(pins[i], values[i]);
    }  
  }
  else if(frame < 5*animation_time)
  {
    for(int i=0; i<sizeof(pins); i++)
    {

      noTone(0);
      digitalWrite(1, LOW);
      int values[] = {
        LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW            };
      digitalWrite(pins[i], values[i]);
    } 
  }
  else if(frame < 6*animation_time)
  {
    for(int i=0; i<sizeof(pins); i++)
    {
      int values[] = {
        HIGH,HIGH,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW            };
      digitalWrite(pins[i], values[i]);
    }  
  }
  else if(frame < 7*animation_time)
  {
    for(int i=0; i<sizeof(pins); i++)
    {
      int values[] = {
        HIGH,HIGH,LOW,LOW,LOW,HIGH,LOW,LOW,LOW,LOW,LOW,LOW            };
      digitalWrite(pins[i], values[i]);
    }  
  }
  else if(frame < 8*animation_time)
  {
    for(int i=0; i<sizeof(pins); i++)
    {
      int values[] = {
        HIGH,HIGH,LOW,LOW,LOW,HIGH,LOW,LOW,HIGH,LOW,LOW,LOW            };
      digitalWrite(pins[i], values[i]);
    }  
  }
  else if(frame < 9*animation_time)
  {
    for(int i=0; i<sizeof(pins); i++)
    {
      int values[] = {
        HIGH,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW            };
      digitalWrite(pins[i], values[i]);
    }  
  }

  else if(frame < 10*animation_time)
  {
     for(int i=0; i<sizeof(pins); i++)
    {
      int values[] = {
        LOW,HIGH,LOW,LOW,LOW,HIGH,LOW,LOW,HIGH,LOW,LOW,LOW            };
      digitalWrite(pins[i], values[i]);
    }  
  }

  delay(animation_time);
}

//convert the dial input into a time
int convert_input(int input)
{
  float range = (720.0)/((1024.0-520.0));
  int temp = ((input-520)*range);
  return (temp/clock_increment)*clock_increment; 
}

// the loop routine runs over and over again forever:
void loop() {

  (time %= 779);
  if(time < 60)
  {
    //time += 60;
  }
  set_alarm = (analogRead(A0)>128)?HIGH:LOW;
  set_time = (analogRead(A1)>128)?HIGH:LOW;
  dial_input = analogRead(A2);
  snooze_button = (analogRead(A3)>128)?HIGH:LOW;
  
  //set the time with the dial
  if(set_time == HIGH)
  {
    time = convert_input(dial_input);
    seconds = 0;
    clock = 0;
    show(time,true);
  }
  
  //set alarm with dial
  if(set_alarm == HIGH)
  {
    alarm = convert_input(dial_input);
    show(alarm,true);
    snooze = false;
  }
  //if snooze, then snooze, also used to show seconds
  else if(snooze_button == HIGH)
  {
    snooze = true;
    show(seconds,false);
    noTone(0);
  }
  //if alarm is ready to go off, then blink
  else if((time >= alarm) && time <= (alarm + timeout))
  {
    show(time,true);
    if(snooze == false)
    {
      blinker(alarm_timer);
    }
  }
  //all else, just show the time
  else
  {
    show(time,true);
    
    //sets the LED to on if alarm is set, off if it is not.
    if(snooze == false)
    {
      digitalWrite(1,HIGH);
    }
    else
    {
      digitalWrite(1,LOW);
    }
  }
  
  delay(realtime);
  //tone(0, dial_input*100);
  clock++;
  alarm_timer++;
  
  //increment clock minutes
  if(clock % clock_time == 0)
  {
    time++;
    clock = 0;
  }
  
  //increment seconds from 0 to 59
  if(clock % clock_multiplier == 0){
    seconds++;
    if(seconds % clock_delay == 0)
    {
     seconds = 0; 
    }
  }
  
}





