/* Buttons to USB Keyboard Example

   You must select Keyboard from the "Tools > USB Type" menu

   This example code is in the public domain.
*/

#include <Bounce.h>
#include <EEPROM.h>

#define NUM_ALIASES 46
#define NUM_MODIFIERS 4
#define MAX_ACTIONS 100

#define numButtons 5
#define pin0 12
#define pin1 3
#define pin2 4
#define pin3 5
#define pin4 6

// Create Bounce objects for each button.  The Bounce object
// automatically deals with contact chatter or "bounce", and
// it makes detecting changes very simple.
Bounce button0 = Bounce(pin0, 10);
Bounce button1 = Bounce(pin1, 10);  // 10 = 10 ms debounce time
Bounce button2 = Bounce(pin2, 10);  // which is appropriate for
Bounce button3 = Bounce(pin3, 10);  // most mechanical pushbuttons
Bounce button4 = Bounce(pin4, 10);

int inStringIndex = 0;
char inString[MAX_ACTIONS] = {0};
char buttonActions[numButtons][2][MAX_ACTIONS] = {0};
int activeModifiers = 0;

String keyDict[NUM_ALIASES] = {
  "Enter", "Esc", "Backspace", "Tab", "Caps",
  "F1", "F2", "F3", "F4", "F5",
  "F6", "F7", "F8", "F9", "F10",
  "F11", "F12", "Print", "Scroll", "Pause",
  "Ins", "Home", "PgUp", "Del", "End",
  "PgDn", "Right", "Left", "Down", "Up", "NumLock",
  "Play", "Pause", "Record", "Stop", "Rewind",
  "FastForward", "Prev", "Next", "VolDn", "VolUp",
  "PlayPause", "Skip", "Mute", "Eject", "Space"
};
int keyIDs[NUM_ALIASES] = {
  KEY_ENTER, KEY_ESC, KEY_BACKSPACE, KEY_TAB, KEY_CAPS_LOCK,
  KEY_F1, KEY_F2, KEY_F3,  KEY_F4,  KEY_F5,
  KEY_F6, KEY_F7,  KEY_F8,  KEY_F9,  KEY_F10,
  KEY_F11, KEY_F12, KEY_PRINTSCREEN, KEY_SCROLL_LOCK, KEY_PAUSE,
  KEY_INSERT, KEY_HOME,  KEY_PAGE_UP, KEY_DELETE,  KEY_END,
  KEY_PAGE_DOWN, KEY_RIGHT, KEY_LEFT,  KEY_DOWN,  KEY_UP,  KEY_NUM_LOCK,
  KEY_MEDIA_PLAY,  KEY_MEDIA_PAUSE, KEY_MEDIA_RECORD, KEY_MEDIA_STOP, KEY_MEDIA_REWIND,
  KEY_MEDIA_FAST_FORWARD, KEY_MEDIA_PREV_TRACK,  KEY_MEDIA_NEXT_TRACK, KEY_MEDIA_VOLUME_DEC,  KEY_MEDIA_VOLUME_INC,
  KEY_MEDIA_PLAY_PAUSE,  KEY_MEDIA_PLAY_SKIP, KEY_MEDIA_MUTE,  KEY_MEDIA_EJECT, ' '
};

String modifierDict[NUM_MODIFIERS] = {"Ctrl", "Shift", "Alt", "Gui"};
int modifierIDs[NUM_MODIFIERS] = {MODIFIERKEY_CTRL, MODIFIERKEY_RIGHT_SHIFT, MODIFIERKEY_ALT, MODIFIERKEY_GUI};


void setup() {
  // Configure the pins for input mode with pullup resistors.
  // The pushbuttons connect from each pin to ground.  When
  // the button is pressed, the pin reads LOW because the button
  // shorts it to ground.  When released, the pin reads HIGH
  // because the pullup resistor connects to +5 volts inside
  // the chip.  LOW for "on", and HIGH for "off" may seem
  // backwards, but using the on-chip pullup resistors is very
  // convenient.  The scheme is called "active low", and it's
  // very commonly used in electronics... so much that the chip
  // has built-in pullup resistors!
  pinMode(pin0, INPUT_PULLUP);
  pinMode(pin1, INPUT_PULLUP);
  pinMode(pin2, INPUT_PULLUP);
  pinMode(pin3, INPUT_PULLUP);
  pinMode(pin4, INPUT_PULLUP);

  Serial.begin(9600);
  delay(500);
  Serial.println("Welcome to the Teensy Keyboard. To set a key, enter the key number (0-4), followed by a space followed either 'p' or 'r' to indicate press or relese. Follow this with space separated pairs of the keystrokes you wish to occur, with !being recognized as a keyup event. ");
  Serial.print("Special keys are: ");
  for (int i = 0; i < NUM_ALIASES; i++)
  {
    Serial.print(keyDict[i]);
    Serial.print(", ");
  }
  for (int i = 0; i < 3; i++)
  {
    Serial.print(modifierDict[i]);
    Serial.print(", ");
  }
  Serial.println(modifierDict[3]);
  Serial.println("The Win / Cmd key is 'Gui'");

  for(int i = 0; i < numButtons; i++)
  {
    loadChannelEEPROM(buttonActions[i][0], i, 0);
    loadChannelEEPROM(buttonActions[i][1], i, 1);
  }
//  String blank = "Ctrl a !Ctrl !a";
//  for (int i = 0; i < numButtons; i++)
//  {
//
//
//    blank.toCharArray(buttonActions[i][0], MAX_ACTIONS);
//    blank.toCharArray(buttonActions[i][1], MAX_ACTIONS);
//  }
  //keyAction(*buttonActions[2][1]);

}

void loop() {
  // Update all the buttons.  There should not be any long
  // delays in loop(), so this runs repetitively at a rate
  // faster than the buttons could be pressed and released.



  button0.update();
  button1.update();
  button2.update();
  button3.update();
  button4.update();

  // Check each button for "falling" edge.
  // Type a message on the Keyboard when each button presses
  // Update the Joystick buttons only upon changes.
  // falling = high (not pressed - voltage from pullup resistor)
  //           to low (pressed - button connects pin to ground)
  if (button0.fallingEdge()) {
    Serial.println("B0 press");
    keyAction(buttonActions[0][0]);
  }
  if (button1.fallingEdge()) {
    Serial.println("B1 press");
    keyAction(buttonActions[1][0]);
  }
  if (button2.fallingEdge()) {
    Serial.println("B2 press");
    keyAction(buttonActions[2][0]);
  }
  if (button3.fallingEdge()) {
    Serial.println("B3 press");
    keyAction(buttonActions[3][0]);
  }
  if (button4.fallingEdge()) {
    Serial.println("B4 press");
    keyAction(buttonActions[4][0]);
  }

  // Check each button for "rising" edge
  // Type a message on the Keyboard when each button releases.
  // For many types of projects, you only care when the button
  // is pressed and the release isn't needed.
  // rising = low (pressed - button connects pin to ground)
  //          to high (not pressed - voltage from pullup resistor)
  if (button0.risingEdge()) {
    Serial.println("B0 release");
    keyAction(buttonActions[0][1]);
  }
  if (button1.risingEdge()) {
    Serial.println("B1 release");
    keyAction(buttonActions[1][1]);
  }
  if (button2.risingEdge()) {
    Serial.println("B2 release");
    keyAction(buttonActions[2][1]);
  }
  if (button3.risingEdge()) {
    Serial.println("B3 release");
    keyAction(buttonActions[3][1]);
  }
  if (button4.risingEdge()) {
    Serial.println("B4 release");
    keyAction(buttonActions[4][1]);
  }


  //read data from serial port.
  while (Serial.available() > 0) {
    char incomingByte = Serial.read();
    if (incomingByte == '\n') //complete message. Attempt to parse
    {
      parseSetting(inString);
      inStringIndex = 0;
      for(int i = 0; i < MAX_ACTIONS; i++)
      {
        inString[i] = 0; //clear  
      }
      
    }
    else
    {
      inString[inStringIndex] = incomingByte;
      inStringIndex++;
    }
  }
}

void parseSetting(char *messageAry)
{

  String message = messageAry;

  //format
  //buttonNumber[0-4] press/release[p,r] [space seperated series of keypresses, up to 100]
  Serial.println(message);

  //parse out which button
  int buttonVal = -1;
  if (message.substring(1, 2) == " ")
  {
    buttonVal = message.substring(0, 1).toInt();
    if (buttonVal < 0 || buttonVal > 4)
    {
      Serial.println("Invalid Key ID");
      return;
    }
  }
  else
  {
    Serial.println("Invalid Key ID");
    return;
  }

  int pr = -1;
  if (message.substring(3, 4) == " ") //parse out press or release
  {
    String prmsg = message.substring(2, 3);
    if (prmsg == "r")
    {
      pr = 1;
    }
    else if (prmsg == "p")
    {

      pr = 0;
    }
    else
    {

      Serial.println("p / r not valid");
      return;
    }
  }
  else
  {
    Serial.println("p / r not valid");
    return;
  }


  String commandString = "";
  commandString = message.substring(4);
  commandString.toCharArray(buttonActions[buttonVal][pr], MAX_ACTIONS);

  Serial.println("saving.");
  storeChannelEEPROM(buttonActions[buttonVal][pr], buttonVal, pr);
  return;

}

void storeChannelEEPROM(char* channelCommands, int channel, int pr)
{
  int bytesPerBuffer = MAX_ACTIONS * sizeof(char);
  int offsetIdx = ((2 * channel) + pr) * (bytesPerBuffer + sizeof(int));
  int checksum = 0;
  for(int i = 0; i < bytesPerBuffer; i+=sizeof(char))
  {
    checksum += channelCommands[i/sizeof(char)];
    EEPROM.put(offsetIdx+i, channelCommands[i/sizeof(char)]);
    //Serial.println(channelCommands[i/sizeof(int)], HEX);
    //int getVal = 0;
    //EEPROM.get(offsetIdx+i, getVal);
    //Serial.println(getVal, HEX);
  }
  EEPROM.put(offsetIdx+bytesPerBuffer, checksum);
}

void loadChannelEEPROM(char* channelBuffer, int channel, int pr)
{
  char channelCommands[MAX_ACTIONS] = {0};
  int bytesPerBuffer = MAX_ACTIONS * sizeof(char);
  int offsetIdx = ((2 * channel) + pr) * (bytesPerBuffer + sizeof(int));
  int checksum = 0;
  for(int i = 0; i < bytesPerBuffer; i+=sizeof(char))
  {
    EEPROM.get(offsetIdx+i, channelCommands[i/sizeof(char)]);
    checksum += channelCommands[i/sizeof(char)];
    //Serial.println(channelCommands[i/sizeof(int)], HEX);
    //int getVal = 0;
    //EEPROM.get(offsetIdx+i, getVal);
    //Serial.println(getVal, HEX);
  }
  int readChecksum = 0;
  EEPROM.get(offsetIdx+bytesPerBuffer, readChecksum);
  Serial.println(checksum);
  Serial.println(readChecksum);
  Serial.println(channelCommands);
  if(readChecksum == checksum)
  {
    strcpy(channelBuffer, channelCommands);
    Serial.println("Loaded.");
  }
  else
  {
    Serial.println("Not Loaded.");
  }
}


void keyAction(char *actionsMsg)
{
  //digest actions
  String actions = actionsMsg;
  Serial.print("Received: ");
  Serial.println(actions);
  if(actions.length() == 0)
  {
    return;
  }
    for(int i = 0; i < MAX_ACTIONS; i++)
    {
      int spaceIndex = actions.indexOf(" ");
      String action = "";
      if(spaceIndex > 0) //there is a space
      {
        action = actions.substring(0, spaceIndex);
        actions = actions.substring(spaceIndex+1);
      }
      else //no spaces left
      {
        action = actions.substring(0, actions.length());
        actions = "";
      }
      bool pressRelease = true;
      if(action.substring(0,1) == "!")
      {
        pressRelease = false;
        action = action.substring(1);
      }
      if(action.length() > 1) //not a single character, parse down
      {
         for(int j = 0; j < NUM_ALIASES; j++)
         {
            if(action == keyDict[j])
            {
              if(pressRelease)
              {
                Keyboard.press(keyIDs[j]);
                Keyboard.send_now();
              }
              else
              {
                Keyboard.release(keyIDs[j]);
                Keyboard.send_now();
              }
            }
         }
  
         for(int j = 0; j < NUM_MODIFIERS; j++)
         {
            if(action == modifierDict[j])
            {
              if(pressRelease)
              {
                activeModifiers |= modifierIDs[j];
              }
              else
              {
                activeModifiers &= ~modifierIDs[j];
              }
              Keyboard.set_modifier(activeModifiers);
              delay(10);
              Keyboard.send_now();
              delay(10);
              
            }
         }
      }
      else //parse to single character
      {
        if(pressRelease)
        {
          Keyboard.press(action[0]);
          Keyboard.send_now();
        }
        else
        {
          Keyboard.release(action[0]);
          Keyboard.send_now();
        }
      }
  
      if(actions == "") //if string fully digested, return
      {
        return;
      }
    }
}

