#include <LedControl.h>
#include <EEPROM.h>
#include <LiquidCrystal.h>


// --------------- PREPARE VARIABLES FOR SETUP --------------------------

// Declarations for the menu
#define NONE -1
// states
#define WELCOME -2
#define MENU -1
#define START_GAME 1
#define HIGHSCORE 2
#define SETTINGS 3
#define ABOUT 4
#define HOW_TO 5
#define RESET_HIGHSCORES 6
#define MY_NAME 7
#define RESET_NAME 8
#define SET_BRIGHTNESS 9
#define SET_SOUND 10
#define SET_MATRIX_BRIGHTNESS 11
#define SET_LCD_BRIGHTNESS 12
#define ENTER_NAME 13
#define CHANGE_BRIGHTNESS_MATRIX_OPTION_INDEX 1
#define CHANGE_BRIGHTNESS_LCD_OPTION_INDEX 2

#define CHAR_DOWN_ARROW 0
#define CHAR_UP_ARROW 1
#define CHAR_HEART 2
#define CHAR_CLOCK 3
#define CHAR_HUMAN 4

// joystick movement directions
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

// switch presses
#define LONG_PRESS 1
#define SHORT_PRESS 0

// multipling factors to reduce options for brightness
#define LCD_BRIGHTNESS_FACTOR 20

//Declarations for connecting the lcd
const byte rs = 9;
const byte en = 8;
const byte d4 = 7;
const byte d5 = 6;
const byte d6 = 5;
const byte d7 = 4;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Variables for controlling the LED matrix display
const int dinPin = 12;    // Data input pin for the LED matrix
const int clkPin = 11;    // Clock input pin for the LED matrix
const int csPin = 10;     // Chip select input pin for the LED matrix
LedControl lc = LedControl(dinPin, clkPin, csPin, 1);  // LED matrix control object

const int ledPin = A2;       // Pin for bomb LED
const int buzzerPin = 13;    // Pin for the buzzer

const int lcdBacklightPin = 3;    // Pin number for controlling the LCD backlight
int matrixBrightness = 8;         // Default brightness level for the LED matrix


// ------------------- EEPROM VARIABLES ----------------------------
bool storedSoundSetting = false; // Variable to store the sound setting status in EEPROM
bool soundEnabled = true;        // Default state for sound (enabled by default)
// Memory addresses for EEPROM
const int matrixBrightnessAddress = 0;        // Address for storing LED matrix brightness setting
const int soundAddress = 2;                   // Address for storing sound setting status
const int theNameStartingAddress = 6;         // Starting address for storing player's name in EEPROM
const int maximScoreAddress = 60;             // Address for storing the maximum game score
const int lcdBrightnessAddress = 100;         // Address for storing LCD brightness setting
int address;                                  // Universal variable for temporary EEPROM addresses

// ------------------- JOYSTICK VARIABLES --------------------------

// -> Joystick declarations for during game
const int joystickDeadzone = 50;   // Threshold for joystick movement

// Timing variables
unsigned long lastMoveTime = 0;     // Timestamp of the last player movement
unsigned long bombLastPlacedTime = 0;  // Timestamp of the last bomb placement
unsigned long previousMillis = 0;      // Timestamp for various time-related tasks
const long interval = 200;             // Interval for certain periodic actions

// Game grid and player position variables
const int matrixSize = 8;   // Size of the game grid (matrixSize x matrixSize)
const int xPin = A0;         // Analog pin for the X-axis of the joystick
const int yPin = A1;         // Analog pin for the Y-axis of the joystick
const int buttonPin = 2;     // Digital pin for the joystick button
int playerX = matrixSize / 2;   // Initial X position of the player
int playerY = matrixSize / 2;   // Initial Y position of the player
unsigned long moveInterval = 20;

// -> Joystick declarations for menu/submenu etc
const int delayLong = 3000;      // Duration of a long delay in milliseconds
const int delayShort = 100;      // Duration of a short delay in milliseconds
const int nameSize = 3;          // Size of the player's name (number of characters)

const int initialXValue = 510;   // Initial value for the X-axis joystick position
const int initialYValue = 510;   // Initial value for the Y-axis joystick position

// minimum and maximum thresholds for joystick movement coordinates
int minThreshold = 250;
int maxThreshold = 750;

byte swState = LOW;      // Current state of a switch (button)
byte lastSwState = LOW;  // Previous state of the switch (button)

int switchPress = NONE;      // Variable to track switch/button press state (NONE, SHORT_PRESS, or LONG_PRESS)
int joystickMove = NONE;     // Variable to track joystick movement state (NONE, UP, DOWN, LEFT, or RIGHT)

// Variable used to detect a short press of the button
bool passedShortDelay = false;

// Flags for controlling scrolling directions and joystick movement
bool canScrollDown = true;   // Flag indicating whether scrolling down is allowed
bool canScrollUp = false;    // Flag indicating whether scrolling up is allowed
bool joyMoved = false;       // Flag indicating whether the joystick has been moved
unsigned long lastDebounceTime = 0;  // Timestamp for the last switch/button debounce
unsigned long lastTextLineMessageTime = -1;
// -------------- Game state variables -------------------------
bool walls[matrixSize][matrixSize];   // Array representing the presence of walls in the game
bool wallsGenerated = false;           // Flag indicating whether walls have been generated
bool startGame = false;             // Flag for displaying HI message
int maximScore = 0;


// --------------- Bomb-related variables ------------------------

bool bombPlaced = false;          // Flag indicating whether a bomb has been placed
bool detonateFlag = false;        // Flag indicating whether the bomb is set to detonate
int bombX, bombY;                 // Coordinates of the bomb on the game grid
bool flagBomb = false;             // Flag indicating the ability to place a bomb

unsigned long bombPreviousMillis = 0;  // Timestamp for bomb-related timing
const long bombInterval = 1000;         // Interval for bomb detonation
const long bombIntervalBlinking = 100;  // Blinking interval for the bomb LED
int score = 0;                         // Global game score
unsigned long gameStartTime;           // Timestamp of the start of the game
bool displayWelcome = true;      // Flag to control the display of the welcome message
int livesPlayer = 3;              // Initial number of lives for the player
int bombRadius = 1;               // Initial bomb explosion radius
unsigned long timerGame;          // Variable to store the game timer value


//    --------------- FLAGS ---------------------
const int constrastOptions = 3;       // Number of contrast options
const int aboutOptions = 7;           // Number of options in the 'About' section
const int settingsOptions = 7;        // Number of options in the 'Settings' section
const int howToOptions = 8;           // Number of options in the 'How To Play' section
const int soundOptions = 2;           // Number of sound options
const int menuOptions = 6;            // Number of options in the main menu

const int welcomeTextSize = 6;       // Size of the welcome text
const int maxColValueLcd = 16;        // Maximum column value for the LCD
const int maxLcdBrightness = 10;      // Maximum brightness level for the LCD

int aboutIndex = 1;                   // Index for navigating options in the 'About' section
int settingsIndex = 1;                // Index for navigating options in the 'Settings' section
int howToIndex = 1;                   // Index for navigating options in the 'How To Play' section
int soundIndex = 1;                   // Index for navigating sound options
int menuIndex = 1;                    // Index for navigating options in the main menu
int letterPos = 0;                    // Position for selecting a letter in the player's name
int brightnessOptionIndex = 1;        // Index for navigating brightness options


// the current state we are in regarding functionality options
int currState = WELCOME;

char theName[nameSize];            // Character array to store the player's name
// Define the starting character for the set of characters supported by the LCD
#define LCD_START_CHAR 'A'
// Define the ending character for the set of characters supported by the LCD
#define LCD_END_CHAR 'Z'
int lcdBrightness = maxLcdBrightness;  // Initial LCD brightness level set to the maximum

// Custom character representing a down arrow symbol
const byte upArrowSymbol[8] = {
  0b00100,
  0b01110,
  0b10101,
  0b00100,
  0b00100,
  0b00000,
  0b00000,
  0b00000
};

const byte downArrowSymbol[8] = {
  0b00100,
  0b00100,
  0b10101,
  0b01110,
  0b00100,
  0b00000,
  0b00000,
  0b00000
};

// Custom character representing a player symbol
const byte player[8] = {
  B00000,
  B01010,
  B00100,
  B01110,
  B10101,
  B00100,
  B00100,
  B01110
};

// Custom character representing a heart symbol
const byte heart[8] = {
  B00000,
  B11011,
  B11111,
  B01110,
  B00100,
  B00000,
  B00000,
  B00000
};


void setLcdContrast() {
  EEPROM.get(lcdBrightnessAddress, lcdBrightness);  // Retrieve LCD brightness setting from EEPROM
  analogWrite(lcdBacklightPin, lcdBrightness * LCD_BRIGHTNESS_FACTOR);  // Set LCD contrast using analogWrite
}

// sets current matrix brightness by accessing EEPROM address where value is stored
void setMatrixContrast() {
  EEPROM.get(matrixBrightnessAddress, matrixBrightness);
  lc.setIntensity(0, matrixBrightness); 
}


// Retrieve the player's name from EEPROM and store it in the character array 'theName'
void setNameFromEEPROM() {
  address = theNameStartingAddress;

  for (int i = 0 ; i < nameSize; i++) {
    EEPROM.get(address, theName[i]);
    address += sizeof(char);
  }
}
bool nameNotSet() {
  return theName[0] == LCD_START_CHAR;
}

void setup() {
  Serial.begin(9600);
  lc.shutdown(0, false);
  lc.setIntensity(0, 8);
  lc.clearDisplay(0);
  lcd.createChar(CHAR_UP_ARROW, upArrowSymbol);
  lcd.createChar(CHAR_DOWN_ARROW, downArrowSymbol);
  lcd.createChar(CHAR_HUMAN, player);
  lcd.createChar(CHAR_HEART, heart);
  setNameFromEEPROM();
  setLcdContrast();
  setMatrixContrast();
  EEPROM.get(maximScoreAddress, maximScore);
  lcd.begin(16, 2);
  pinMode(3, OUTPUT); 
  analogWrite(3, 90); 
  pinMode(buttonPin, INPUT);
  digitalWrite(buttonPin, HIGH);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  // Seed the random number generator with an analog reading from pin A2
  randomSeed(analogRead(A2));
  generateRandomWalls();
  gameStartTime = millis();
  score = 0;
}
void loop() {
  mainMenu();
}

void enterWelcome() {
  const char welcomeText[][16] = {
    {'T', 'h', 'e', ' ', 'b', 'e', 's', 't', ' ', 'o', 'f', ' ', 'b', 'e', 's', 't'},
    {'g', 'a', 'm', 'e', ' ', ' ', 'i', 's', ' ', 'b', 'a', 'c', 'k', '!', ' ', ' '},
    {'W', 'e', 'l', 'c', 'o', 'm', 'e', ' ', 't', 'o',  ' ', '.', '.', '.', ' ', ' '},
    {' ', ' ', ' ', 'B', 'O', 'M', 'B', 'E', 'R', 'M', 'A', 'N', '!', ' ', ' ', ' '},
    {'P', 'l', 'e', 'a', 's', 'e', ' ', 'l', 'e', 't', ' ', 'm', 'e', ' ', 't', 'o'},
    {'s', 'h', 'o', 'w', ' ', 'y', 'o', 'u', ' ', 'm', 'e', 'n', 'u', '.', '.', '.'},
  };
  displayHi();
  displayScrollingText(welcomeText, welcomeTextSize, MENU);
  switchPress = getSwitchPress();
  if (switchPress != NONE) {
    //playBeep
    nextOption(MENU);
  }
} 

void menuOption() {
  static const char menuText[][16] = {
    {' ', ' ', ' ', ' ', ' ', 'M', 'e', 'n', 'u', ' ', ' ', ' ', ' ', ' ', ' '},
    {'S', 't', 'a', 'r', 't', ' ', 'G', 'a', 'm', 'e', ' ', ' ', ' ', ' ', ' '},
    {'H', 'i', 'g', 'h', 's', 'c', 'o', 'r', 'e', 's', ' ', ' ', ' ', ' ', ' '},
    {'S', 'e', 't', 't', 'i', 'n', 'g', 's', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'A', 'b', 'o', 'u', 't', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'H', 'o', 'w', ' ', 't', 'o', ' ', 'p', 'l', 'a', 'y', ' ', ' ', ' ', ' '},
  };
  displayBomb();
  menuScroll(menuText, menuOptions, menuIndex, MENU, 1, menuOptions - 1, 1, true);
  switchPress = getSwitchPress();
  if (switchPress != NONE) {
    // by pressing the user choose the menu option displayed on the lcd
    //playBeep();
    subMenu(menuIndex);
  }
}

void aboutOption() {
  static const char aboutText[][16] = {
    {' ', ' ', ' ', ' ', ' ', 'A', 'b', 'o', 'u', 't', ' ', ' ', ' ', ' ', ' '},
    {'N', 'a', 'm', 'e', ':', ' ', 'B', 'o', 'm', 'b', 'e', 'r', 'm', 'a', 'n'},
    {'F', 'u', 'n', ' ', '&', ' ', 'S', 't', 'r', 'a', 't', 'e', 'g', 'y', ' '},
    {'A', 'u', 't', 'h', 'o', 'r', ':', ' ', 'A', 'n', 'a', ' ', ' ', ' ', ' '},
    {'M', 'a', 'r', 'i', 'a', ' ', 'C', 'i', 'm', 'p', 'e', 'a', 'n', 'u', ' '},
    {'G', 'i', 't', 'h', 'u', 'b', ' ', 'U', 's', 'e', 'r', 'n', 'a', 'm', 'e'},
    {'a', 'n', 'a', 'c', 'i', 'm', 'p', 'e', 'a', 'n', 'u', ' ', ' ', ' ', ' '},
  };
  displayMe();
  menuScroll(aboutText, aboutOptions, aboutIndex, MENU, 1, aboutOptions - 1, 1, true);
}
void howOption() {
  static const char howToMatrix[][16] = {
    {' ', ' ', 'H', 'o', 'w', ' ', 't', 'o', ' ', 'p', 'l', 'a', 'y', ' ', ' '},
    {'D', 'i', 's', 't', 'r', 'o', 'y', ' ', 'a', 'l', 'l', ' ', 'w', 'a', 'l'},
    {'U', 's', 'e', ' ', 'j', 'o', 'y', 's', 't', 'i', 'c', 'k', ' ', 't', 'o'},
    {'U', 'p', ' ', '&', ' ', 'D', 'o', 'w', 'n', ' ', 'm', 'e', 'n', 'u', ' '},
    {'L', 'e', 'f', 't', ' ', 'f', 'o', 'r', ' ', 'b', 'a', 'c', 'k', ' ', ' '},
    {'W', 'A', 'S', 'D', ' ', 't', 'o', ' ', 'm', 'o', 'v', 'e', ' ', ' ', ' '},
    {'i', 'n', 't', 'h', 'e', ' ', 'g', 'a', 'm', 'e', '!', ' ', ' ', ' ', ' '},
    {'G', 'o', 'o', 'd', ' ', 'L', 'u', 'c', 'k', ' ', ' ', ' ', ' ', ' ', '!'},
  };
  displayHow();
  menuScroll(howToMatrix, howToOptions, howToIndex, MENU, 1, howToOptions - 1, 1, true);
}

// Display a scrolling menu on the LCD, allowing the user to navigate through options
void menuScroll(const char text[][16], const int options, int &scrollIndex, const int returnToState, const int lowerIndex, const int upperIndex, const int upperBoundCursorRow, bool automaticSave) {
  if (!automaticSave) {
    // Display current option and a message to prompt user to save
    lcd.setCursor(0, 0);
    lcd.print(text[scrollIndex]);
    lcd.setCursor(0, 1);
    lcd.print("Press to save.");
  } else {
    // Display the first option and the current selected option
    lcd.setCursor(0, 0);
    lcd.print(text[0]);
    lcd.setCursor(0, 1);
    lcd.print(text[scrollIndex]);
  }

  // Allow the user to scroll up or down
  upOrDown();

  // Get joystick movement
  joystickMove = getJoystickMove();

  // Handle joystick movements
  if (joystickMove == LEFT) {
    lcd.clear();
    scrollIndex = max(scrollIndex - 1, lowerIndex);
  } else if (joystickMove == RIGHT) {
    lcd.clear();
    scrollIndex = min(scrollIndex + 1, upperIndex);
  } else if (joystickMove == DOWN && automaticSave) {
    // Save option and return to the specified state
    //playBeep();
    scrollIndex = lowerIndex;
    nextOption(returnToState);
  }

  // Update flags to indicate scrollability
  canScrollUp = !(scrollIndex == lowerIndex);
  canScrollDown = !(scrollIndex == upperIndex);
}


// Display up and down arrows on the LCD to indicate scrollability
void upOrDown() {
  // Display up arrow if scrolling up is allowed
  lcd.setCursor(15, 0);
  if (canScrollUp) {
    lcd.write(CHAR_UP_ARROW);
  } else {
    lcd.print(" ");
  }

  // Display down arrow if scrolling down is allowed
  lcd.setCursor(15, 1);
  if (canScrollDown) {
    lcd.write(char(CHAR_DOWN_ARROW));
  } else {
    lcd.print(" ");
  }
}

// Initialize the name with default values and save to EEPROM
void initializeName() {
  for (int i = 0; i < nameSize; i++) {
    theName[i] = LCD_START_CHAR;  // Initialize with the default character 'A'
  }
  saveName();  // Save the initialized name to EEPROM
  lcd.clear();
  lcd.setCursor(0, 0);
  letterPos = 0;
}

// Save the current name to EEPROM
void saveName() {
  address = theNameStartingAddress;

  for (int i = 0; i < nameSize; i++) {
    EEPROM.put(address, theName[i]);  // Save each character of the name to EEPROM
    address += sizeof(char);
  }
}

// Print the player's name stored in EEPROM to the Serial monitor and LCD
void printName() {
  address = theNameStartingAddress;  // Set the EEPROM address to start retrieving the name

  Serial.print("Current Name (EEPROM): ");

  // Loop through each character position in the array and retrieve from EEPROM
  for (int i = 0; i < nameSize; i++) {
    EEPROM.get(address, theName[i]);  // Retrieve a character and store it in the array
    Serial.print(theName[i]);         // Print the character to the Serial monitor
    lcd.print(theName[i]);            // Print the character to the LCD
    address += sizeof(char);           // Move to the next EEPROM address
  }

  Serial.println();  // Print a newline to the Serial monitor for formatting
}


void clearMatrix() {
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, 0);
    }
  }
}

void displayGameDetails() {
  unsigned long elapsedTime = millis() - timerGame;
  int seconds = elapsedTime / 1000;
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Time: ");
  lcd.print(seconds);
  lcd.print("s");
  //lcd.print(CHAR_CLOCK);
  lcd.setCursor(0, 1);
  lcd.print("Lives: ");
  lcd.print(livesPlayer);
  lcd.write(CHAR_HEART);
  lcd.setCursor(12,1);
  lcd.write(CHAR_HUMAN);
  printName();

}
void mainMenu() {
  switch (currState) {
    case WELCOME:
      // display welcome message and correspoding matrix image
      enterWelcome();
      break;
    case MENU:
      // enters the menu of the game
      menuOption();
      break;
    case START_GAME:
      // starts a new game
      gameOption();
      break;
  case SETTINGS:
  //    enters the settings menu
       settingOption();
       break;
    case ABOUT:
      // enters the about section
      aboutOption();
      break;
    case HOW_TO:
       howOption();
       break;
  case SET_SOUND:
      setSound();
      break;
  case SET_BRIGHTNESS:
      setBrightness();
      break;
  case ENTER_NAME:
     nameOption();
      break;
  case SET_MATRIX_BRIGHTNESS:
      setMatrixBrightness();
      break;
  case SET_LCD_BRIGHTNESS:
      setLcdBrightness();
      break;
  case HIGHSCORE:
    displayHighscore();
    break;
  case RESET_HIGHSCORES:
    displayTop();
    break;
  case MY_NAME:
    displayName();
    break;
  case RESET_NAME:
    displayReset();
    break;

  }
}
void subMenu(const int menuIndex) {
    lcd.clear();
    clearMatrix();

    switch(menuIndex) {
      case START_GAME:
        nextOption(START_GAME);
        break;
      case HIGHSCORE:
        nextOption(HIGHSCORE);
        break;
      case SETTINGS:
        nextOption(SETTINGS);
        break;
      case ABOUT:
        nextOption(ABOUT);
        break;
      case HOW_TO:
        nextOption(HOW_TO);
        break;
      default:
        break;
    }
}
void scrollSettingsOptions(const int settingsIndex) {
  switch(settingsIndex) {
    case 1:
      if (nameNotSet())
      {
        initializeName();
      }
      nextOption(ENTER_NAME);
      break;
    case 2:
      nextOption(RESET_NAME);
      break;
    case 3:
      nextOption(SET_BRIGHTNESS);
      break;
    case 4:
      nextOption(SET_SOUND);
      break;
    case 5:
      nextOption(RESET_HIGHSCORES);
    break;
    case 6:
      nextOption(MY_NAME);
    break;
  }
}

// Get the state of the switch (button) press, distinguishing between short and long presses
int getSwitchPress() {
  int reading = digitalRead(2);  // Read the current state of the switch

  // Update debounce time if the switch state changes
  if (reading != lastSwState) {
    lastDebounceTime = millis();
  }

  lastSwState = reading;  // Update the last switch state

  // Check for debounce time and determine switch press type
  if (millis() - lastDebounceTime >= delayShort) {
    if (reading != swState) {
      swState = reading;

      if (swState == LOW) {
        passedShortDelay = true;
      }
    }

    if (swState == LOW) {
      if (millis() - lastDebounceTime >= delayLong) {
        return LONG_PRESS;
      }
    } else {
      if (passedShortDelay) {
        passedShortDelay = false;
        return SHORT_PRESS;
      }
    }
  }

  return NONE;  // Return no switch press if conditions are not met
}

// Display scrolling text on the LCD
void displayScrollingText(const char text[][16], const int noOfLines, const int nextState) {
  static int scrollInterval = 175;
  static int lineIndex = 0;
  static int columnIndex = 0;
  static int cursorRow = 0;

  // Initialize last message time at the first call of the function if unset to keep track of scroll interval per row
  if (lastTextLineMessageTime == -1 && millis() != 0) {
    lcd.setCursor(0, cursorRow);
    lastTextLineMessageTime = millis();
  }

  // If the scrolling interval has been reached, display another letter
  if (millis() - lastTextLineMessageTime >= scrollInterval && columnIndex < 17) {
    lcd.print(text[lineIndex][columnIndex]);
    lastTextLineMessageTime = millis();
    columnIndex += 1;
  }

  // Maximum column index has been reached; if the whole message has been displayed, transition to the next state, otherwise, transition to the other cursor row
  if (columnIndex > maxColValueLcd) {
    // Reinitialize last message time, column index, and line index
    lastTextLineMessageTime = -1;
    columnIndex = 0;
    lineIndex += 1;

    // The whole message has been displayed, and the next step is to transition to the next state
    if (lineIndex == noOfLines) {
      nextOption(nextState);
    }

    // Reset row to the other cursor row value because the maximum column index has been reached on the current value
    if (cursorRow == 1) {
      lcd.clear();
      cursorRow = 0;
    } else {
      cursorRow = 1;
    }
  }
}

void nextOption(const int nextState) {
  lcd.clear();
  lcd.setCursor(0,0);
  currState = nextState;
}

void settingOption() {
  static const char settingsText[][16] = {
    {' ', ' ', ' ', 'S', 'e', 't', 't', 'i', 'n', 'g', 's', ' ', ' ', ' ', ' '},
    {'E', 'n', 't', 'e', 'r', ' ', 'N', 'a', 'm', 'e', ' ', ' ', ' ', ' ', ' '},
    {'R', 'e', 's', 'e', 't', ' ', 'N', 'a', 'm', 'e', ' ', ' ', ' ', ' ', ' '},
    {'B', 'r', 'i', 'g', 'h', 't', 'n', 'e', 's', 's', ' ', ' ', ' ', ' ', ' '},
    {'S', 'o', 'u', 'n', 'd', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'R', 'e', 's', 'e', 't', ' ', 'h', 'i', 'g', 'h', 's', 'c', 'o', 'r', 'e'},
    {'V', 'i', 'e', 'w', ' ', ' ', 'M', 'y', ' ', ' ', 'n', 'a', 'm', 'e', ' '},
  };
  displaySettings();
  menuScroll(settingsText, settingsOptions, settingsIndex, MENU, 1, settingsOptions - 1, 1, true);
  
  switchPress = getSwitchPress();

  if (switchPress != NONE) {
    lcd.clear();
    //playBeep();
    scrollSettingsOptions(settingsIndex);
  }
}

void displayHighscore() {
  displayHighscores();
  lcd.setCursor(0,0);
  lcd.print("Your highscore");
  lcd.setCursor(0,1);
  lcd.print(maximScore);
  lcd.setCursor(12,0);
  lcd.print("Name");
  lcd.setCursor(12,1);
  printName();
  joystickMove = getJoystickMove();
  if(joystickMove == DOWN){
    nextOption(MENU);
    //playBeep();
  }

  
}
void displayReset() {
  resetName();
  lcd.setCursor(0,0);
  lcd.print("Press to");
  lcd.setCursor(0,1);
  lcd.print("clear your name");
  joystickMove = getJoystickMove();
  if(joystickMove == DOWN){
    nextOption(SETTINGS);
    //playBeep();
  }
}
void displayTop() {
  resetHighscore();
  lcd.setCursor(0,0);
  lcd.print("Press and ");
  lcd.setCursor(0,1);
  lcd.print("reset highscore ");
  joystickMove = getJoystickMove();
  if(joystickMove == DOWN){
    nextOption(SETTINGS);
    //playBeep();
  }
}
void displayName() {
  lcd.setCursor(0,0);
  lcd.print("Your name");
  lcd.setCursor(0,1);
  printName();
  joystickMove = getJoystickMove();
  if(joystickMove == DOWN){ 
     nextOption(SETTINGS);
     //playBeep();
  }
}

void nameOption() {
  // Display the corresponding message
  lcd.setCursor(0, 1);
  lcd.print("Press to save.");
  lcd.setCursor(0, 0);

  // Display the current name to be saved
  for (int i = 0; i < nameSize; i++) {
    lcd.print(theName[i]);
  }

  lcd.setCursor(9, 0);
  upOrDown();

  joystickMove = getJoystickMove();

  switch (joystickMove) {
    case LEFT:
      // Selects the letter before the current one in alphabetical order if it exists
      if (theName[letterPos] != LCD_START_CHAR) {
        theName[letterPos]--;
      }
      break;
    case RIGHT:
      // Selects the letter after the current one in alphabetical order if it exists
      if (theName[letterPos] != LCD_END_CHAR) {
        theName[letterPos]++;
      }
      break;
    case UP:
      // Selects the next available position of a letter to be changed (e.g., from the first position to the second)
      letterPos = min(letterPos + 1, nameSize - 1);
      break;
    case DOWN:
      // Selects the previous available position of a letter to be changed (e.g., from the second position to the first)
      letterPos = max(letterPos - 1, 0);
      break;
    default:
      break;
  }

  canScrollUp = !(theName[letterPos] == LCD_START_CHAR);
  canScrollDown = !(theName[letterPos] == LCD_END_CHAR);

  switchPress = getSwitchPress();

  if (switchPress != NONE) {
    saveName();
    letterPos = 0;
  }
  if (!switchPress) {
    currState = SETTINGS;
  }
}
void setSound() {
  static const char soundText[][16] = {
    {'O', 'N', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'O', 'F', 'F', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
  };

  menuScroll(soundText, soundOptions, soundIndex, SETTINGS, 0, soundOptions - 1, 0, false);

  switchPress = getSwitchPress();

  if (switchPress != NONE) {
    EEPROM.put(soundAddress, (soundIndex == 0));  // 0 pentru ON, 1 pentru OFF
    setSoundFromEEPROM();
    nextOption(SETTINGS);
    //playBeep();
  }
}

void setSoundFromEEPROM() {
  EEPROM.get(soundAddress, storedSoundSetting);

  Serial.print("Stored Sound Setting: ");
  Serial.println(storedSoundSetting);

  soundEnabled = storedSoundSetting;
}

void setBrightness() {
  static const char brightnessMatrix[][16] = {
    {'B', 'r', 'i', 'g', 'h', 't', 'n', 'e', 's', 's', ' ', ' ', ' ', ' ', ' '},
    {'M', 'a', 't', 'r', 'i', 'x', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'L', 'c', 'd', ' ', 'D', 'i', 's', 'p', 'l', 'a', 'y', ' ', ' ', ' ', ' '},
  };

  menuScroll(brightnessMatrix, constrastOptions, brightnessOptionIndex, SETTINGS, 1, constrastOptions - 1, 1, true);

  switchPress = getSwitchPress();

  if (switchPress != NONE) {
    parseBrightnessOption(brightnessOptionIndex);
  }
} 
void setLcdBrightness() {
  displayBrightness(lcdBrightness);

  joystickMove = getJoystickMove();

  // decrease brightness
  if (joystickMove == RIGHT) {
    lcd.clear();
    lcdBrightness = min(lcdBrightness + 1, maxLcdBrightness);
    
    EEPROM.put(lcdBrightnessAddress, lcdBrightness);
  }  
  // increase brightness
  else if (joystickMove == LEFT) {
    lcd.clear();
    lcdBrightness = max(lcdBrightness - 1, 0);
    EEPROM.put(lcdBrightnessAddress, lcdBrightness);
  }

  setLcdContrast();

  // go back to previous setting
  if (joystickMove == DOWN) {
    EEPROM.put(lcdBrightnessAddress, lcdBrightness);
    nextOption(SET_BRIGHTNESS);
    //playBeep();
  }

  canScrollUp = !(lcdBrightness == maxLcdBrightness);
  canScrollDown = !(lcdBrightness == 0);
}

void parseBrightnessOption(const int brightnessOptionIndex) {
  lcd.clear();

  switch(brightnessOptionIndex) {
    case CHANGE_BRIGHTNESS_MATRIX_OPTION_INDEX:
      nextOption(SET_MATRIX_BRIGHTNESS);
      break;
    case CHANGE_BRIGHTNESS_LCD_OPTION_INDEX:
      nextOption(SET_LCD_BRIGHTNESS);
      break;
    default:
      break;

  }
  // go back to settings (automatic save)
  joystickMove = getJoystickMove();
	
  if (joystickMove == DOWN) {
    nextOption(SETTINGS);
  }
}

void setMatrixBrightness() {
  displayBrightness(matrixBrightness);

  joystickMove = getJoystickMove();

  if (joystickMove == RIGHT) {
    lcd.clear();
    matrixBrightness = max(matrixBrightness - 1, 0);
    EEPROM.put(matrixBrightnessAddress, matrixBrightness);
  }  
  else if (joystickMove == LEFT) {
    lcd.clear();
    matrixBrightness = min(matrixBrightness + 1, 15);
    EEPROM.put(matrixBrightnessAddress, matrixBrightness);
  }  setMatrixContrast();

  if (joystickMove == DOWN) {
    nextOption(SET_BRIGHTNESS);
    //playBeep();
  }

  canScrollUp = !(matrixBrightness == 0);
  canScrollDown = !(matrixBrightness == 15);
}
void displayBrightness(const int brightnessValue) {
  lcd.setCursor(0,0);
  lcd.print("Brightness:");
  lcd.setCursor(0,1);
  lcd.print(brightnessValue);
  upOrDown();
}

// Get the joystick movement direction
int getJoystickMove() {
  static int xValue = 0,
             yValue = 0,
             xChange = 0,
             yChange = 0;

  xValue = analogRead(xPin);
  yValue = analogRead(yPin);

  xChange = abs(initialXValue - xValue);
  yChange = abs(initialYValue - yValue);

  if (!joyMoved) {     
    if (yChange >= xChange) {
      if (yValue < minThreshold) {
        joyMoved = true;
        return RIGHT;
      }
      if (yValue > maxThreshold) {
        joyMoved = true;
        return LEFT;
      }
    } 
    else {
      if (xValue < minThreshold) {
        joyMoved = true;
        return UP;
      }
      if (xValue > maxThreshold) {
        joyMoved = true;
        return DOWN;
      }
    }
  }

  if (xValue >= minThreshold && xValue <= maxThreshold && yValue >= minThreshold && yValue <= maxThreshold) {
    joyMoved = false;
  }

  return NONE;
}

// Reset the highscore to zero and update the EEPROM
void resetHighscore() {
  maximScore = 0;                            // Reset the local variable for highscore
  EEPROM.put(maximScoreAddress, maximScore);  // Save the highscore to EEPROM
}

// Reset the name to default values "AAA" and save to EEPROM
void resetName() {
  // Set the name to "AAA"
  for (int i = 0; i < nameSize; i++) {
    theName[i] = LCD_START_CHAR;
  }

  // Save the reset name to EEPROM
  saveName();
}

void gameOption(){
  handleButtonPress();
  if (startGame){
    handleJoystickInput();
  }
}

// ------- [code for game functionalities]---------------
void handleButtonPress() {
  // Show to the display the countdown images and make decisions 
  if (digitalRead(buttonPin) == LOW && !wallsGenerated) {
    for (int count = 3; count >= 0; count--) {
      displayCountdown(count);
    }
    wallsGenerated = true;  // Mark that walls have been generated and the game has started
    startGame = true;  // Turn off the hi display flag to switch to the game display
    timerGame = millis();
  }
}

void handleJoystickInput() {
    //handleButtonPress();
    //generateRandomWalls();
    int xValue = analogRead(xPin);  // Read the analog value from the X-axis of the joystick
    int yValue = analogRead(yPin);  // Analog

    // Check if joystick movement is significant
    if (abs(xValue - 512) > joystickDeadzone || abs(yValue - 512) > joystickDeadzone) {
      if (millis() - lastMoveTime >= moveInterval || bombPlaced) {
        updatePlayerPosition(xValue, yValue);
        lastMoveTime = millis();
      }
    }
    
     // Check if the joystick button is pressed
    if (digitalRead(buttonPin) == LOW) {
      if (!bombPlaced && flagBomb) {  // Place a bomb if none is already placed and the flag is set
        placeBomb();
      }
    } else {
      // Check if a bomb is placed and the detonation flag is set
      if (bombPlaced && detonateFlag && millis() - bombLastPlacedTime > bombInterval) {
        detonateBomb(); // Detonate the bomb if conditions are methandleGameWin
      }
    }

    displayWalls(); // Display the walls on the LED matrix
    displayBlinkingPlayer();   // Display the player's blinking position on the LED matrix
    displayBlinkingBomb();     // Display the blinking bomb on the LED matrix
    checkGameWin();  // Check if the player has won the game
    displayGameDetails();
}


void checkGameWin() {
  if (allWallsEliminated()) {
    handleGameWin();
  }
}

// Check if all the walls have been removed
bool allWallsEliminated() {
  for (int row = 0; row < matrixSize; row++) {
    for (int column = 0; column < matrixSize; column++) {
      if (walls[row][column]) {
        return false;
      }
    }
  }
  return true;
}

void handleGameWin() {
  // Record the end time of the game
  unsigned long gameEndTime = millis();

  // Calculate the total game time
  unsigned long gameTime = gameEndTime - gameStartTime;

  // Calculate the bonus based on the number of broken walls
  int wallBreakBonus = countWallsBroken() * 10;
  // Update the global score with the wall break bonus and game time

  score += wallBreakBonus + gameTime;

  if (abs(score) > maximScore) {
    maximScore = score;

    // Salvează noul maxim scor în EEPROM
    EEPROM.put(maximScoreAddress, maximScore);
  }
  Serial.print("Score: ");
  Serial.println(abs(score));
  displayWinAnimation();

  // Reset the game state to prepare for a new game
  resetGameState();
}

void resetGameState() {
  // Reset flags and variables to their initial state
  wallsGenerated = false;    // Reset the flag indicating whether walls have been generated
  startGame = false;   
  displayWelcome = true;     // Set the flag to display the "hi" message
  bombPlaced = false;        // Reset the flag indicating whether a bomb is placed
  detonateFlag = false;      // Reset the flag indicating whether to detonate a bomb
  generateRandomWalls();     // Generate a new random wall configuration
  gameStartTime = millis();   // Record the current time as the start time of the new game
  currState = MENU;
}


void displayWinAnimation() {
  const uint64_t smileyImage = 0x3c42a581a599423c;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Your score : ");
  lcd.setCursor(0,1);
  lcd.print(score);
  displayImage(smileyImage);
  playWinSound();
  delay(2000);
  lc.clearDisplay(0);
  lcd.clear();
}

// Count how many walls were broken for the score
int countWallsBroken() {
  int count = 0;
  for (int row = 0; row < matrixSize; row++) {
    for (int column = 0; column < matrixSize; column++) {
      if (!walls[row][column]) {
        count++;
      }
    }
  }
  return count;
}

// Function with which we make sure that no walls are generated above or next to the player, to allow movement at the beginning of the game

bool isPlayerSurrounded(int x, int y) {
  if ((x > 0 && walls[y][x - 1]) || (x < matrixSize - 1 && walls[y][x + 1]) ||
      (y > 0 && walls[y - 1][x]) || (y < matrixSize - 1 && walls[y + 1][x])) {
    return true;
  }
  return false;
}

void generateRandomWalls() {
  generateRandomPosition();

  // Generate random walls in the matrix
  for (int i = 0; i < matrixSize; i++) {
    for (int j = 0; j < matrixSize; j++) {
      walls[i][j] = random(2) == 0; // Randomly set each cell as a wall or empty space
    }
  }
}

void generateRandomPosition() {
  playerX = random(matrixSize);  // Generate a random X coordinate for the player
  playerY = random(matrixSize);  // Generate a random Y coordinate for the player

  // Check if the generated position is valid
  if (walls[playerY][playerX] || isPlayerSurrounded(playerX, playerY)) {
    // If the position is not valid, recursively call the function to generate another one
    generateRandomPosition();
  }
}

// Function to display the walls on the display
void displayWalls() {
  for (int row = 0; row < matrixSize; row++) {
    for (int column = 0; column < matrixSize; column++) {
      if (walls[row][column]) {
        lc.setLed(0, row, column, true);
      }
    }
  }
}

void displayBlinkingPlayer() {
  unsigned long currentMillis = millis();
  static bool playerLEDState = false;

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    playerLEDState = !playerLEDState;

    lc.setLed(0, playerY, playerX, playerLEDState);
  }
}

void displayBlinkingBomb() {
  unsigned long currentMillis = millis();
  static bool bombLEDState = false;

  if (bombPlaced && detonateFlag) {
    if (currentMillis - bombPreviousMillis >= bombIntervalBlinking) {
      bombPreviousMillis = currentMillis;
      bombLEDState = !bombLEDState;

      lc.setLed(0, bombY, bombX, bombLEDState);
    }
  }
}

const int moveDelay = 200;

unsigned long lastMoveMillis = 0;

void updatePlayerPosition(int xValue, int yValue) {
  // Calculate the change in X and Y coordinates based on joystick input
  int deltaX = xValue - 512;
  int deltaY = yValue - 512;

  // Store the player's current position
  int previousX = playerX;
  int previousY = playerY;

  lc.setLed(0, previousY, previousX, false);

  unsigned long currentMillis = millis();

  if (currentMillis - lastMoveMillis >= moveDelay) {
    // Check joystick input for X-axis movement
    if (abs(deltaX) > joystickDeadzone) {
      if (deltaX > 0 && playerX < matrixSize - 1 && !walls[playerY][playerX + 1]) {
        playerX++;
      } else if (deltaX < 0 && playerX > 0 && !walls[playerY][playerX - 1]) {
        playerX--;
      }
    }

    // Check joystick input for Y-axis movement
    if (abs(deltaY) > joystickDeadzone) {
      if (deltaY > 0 && playerY < matrixSize - 1 && !walls[playerY + 1][playerX]) {
        playerY++;
      } else if (deltaY < 0 && playerY > 0 && !walls[playerY - 1][playerX]) {
        playerY--;
      }
    }

    // Check if the player's position has changed
    if (playerX != previousX || playerY != previousY) {

      // Turn on the LED at the new player position
      lc.setLed(0, playerY, playerX, true);

      // Set the flagBomb to true, indicating that the player can place a bomb
      flagBomb = true;
    }

    lastMoveMillis = currentMillis;
  }
}

void placeBomb() {
   // Check if a bomb is not already placed
  if (!bombPlaced) {
    // Set the bomb's position to the player's current position
    bombX = playerX;
    bombY = playerY;
    bombPlaced = true;
    detonateFlag = true;
    // Turn on the LED and buzzer to indicate the bomb placement
    digitalWrite(ledPin, HIGH);
    //digitalWrite(buzzerPin, HIGH);
    playBoomSound();
    bombLastPlacedTime = millis();
  }
}

void detonateBomb() {
  // Check if a bomb is currently placed
  if (bombPlaced) {
    // Eliminate walls in all four directions from the bomb
    eliminateFirstWallInDirection(bombX, bombY, 1, 0);
    eliminateFirstWallInDirection(bombX, bombY, -1, 0);
    eliminateFirstWallInDirection(bombX, bombY, 0, 1);
    eliminateFirstWallInDirection(bombX, bombY, 0, -1);

    // Check if the player is within the blast radius
    int deltaX = abs(playerX - bombX);
    int deltaY = abs(playerY - bombY);

    if (deltaX <= bombRadius && deltaY <= bombRadius) {
      // Player is within the blast radius, decrement lives
      livesPlayer--;
      playLifeLostSound();
      if (livesPlayer <= 0) {
        // Player has no lives left, reset the game
        livesPlayer = 3;
        displayLooseAnimation();
        resetGameState();
      }
    }

    // Clear bomb on the matrix
    lc.setLed(0, bombY, bombX, false);

    // Turn off the LED and buzzer
    digitalWrite(ledPin, LOW);
    digitalWrite(buzzerPin, LOW);

    // Mark the bomb as no longer placed and ready to detonate
    bombPlaced = false;
    detonateFlag = false;
  }
}

void eliminateFirstWallInDirection(int startX, int startY, int dirX, int dirY) {

  // Iterate in the specified direction until reaching the matrix boundaries

  for (int x = startX + dirX, y = startY + dirY;
       x >= 0 && x < matrixSize && y >= 0 && y < matrixSize;
       x += dirX, y += dirY) {
    if (walls[y][x]) {
      // Eliminate the wall at the current position
      walls[y][x] = false;
       // Turn off the LED at the corresponding position on the display
      lc.setLed(0, y, x, false);
      // Play a sound to indicate the elimination of the wall
      playCrashSound(); 
       // Exit the loop after eliminating the first wall in the specified direction
      break;
    }
  }
}


void displayHi() {
  const uint64_t hiImage = 0x00a9a9afa929a900;

  unsigned long currentMillis = millis();
  if (millis() - currentMillis < interval) {
    displayImage(hiImage);
    //lc.clearDisplay(0);
  }
}

void displayImage(uint64_t image) {
  for (int i = 0; i < 8; i++) {
    byte rowData = (image >> (i * 8)) & 0xFF;
    lc.setRow(0, i, rowData);
  }
}
void displayCountdown(int count) {
  const uint64_t countdownImage[] = {
    0x0000000000000000,
    // Define binary representations for numbers 3, 2, 1
    0x0018181c1818187e, // 1
    0x003c6660300c067e, // 2
    0x003c66603860663c, // 3
  };

  if (count >= 0 && count <= 3) {
    displayImage(countdownImage[count]);
    playCountdownSound();
    delay(1000); // Adjust the delay as needed
    lc.clearDisplay(0);
  }
}
void displayBomb() {
  const uint64_t bombImage = 0x9da5a59da5a59dff;

  unsigned long currentMillis = millis();
  if (millis() - currentMillis < interval) {
    displayImage(bombImage);
    //lc.clearDisplay(0);
  }
  
}
void displaySettings() {
  const uint64_t settingsImage = 0x003c425a5a423c00;

  unsigned long currentMillis = millis();
  if (millis() - currentMillis < interval) {
    displayImage(settingsImage);
    //lc.clearDisplay(0);
  }
  
}
void displayMe() {
  const uint64_t meImage = 0x0066ffffff7e3c18;

  unsigned long currentMillis = millis();
  if (millis() - currentMillis < interval) {
    displayImage(meImage);
    //lc.clearDisplay(0);
  }
  
}
void displayHow() {
  const uint64_t howImage = 0x3844440810100010;

  unsigned long currentMillis = millis();
  if (millis() - currentMillis < interval) {
    displayImage(howImage);
    //lc.clearDisplay(0);
  }
  
}
void displayHighscores() {
  const uint64_t highscores = 0x7e7e3c1818187e7e;

  unsigned long currentMillis = millis();
  if (millis() - currentMillis < interval) {
    displayImage(highscores);
    //lc.clearDisplay(0);
  }
  
}
void displayLooseAnimation() {
  const uint64_t sadImage = 0x3c42a58199a5423c;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" You lost! ");
  lcd.setCursor(0,1);
  lcd.print(" Try again!");
  displayImage(sadImage);
  playLoserSound();
  delay(2000);
  lc.clearDisplay(0);
  lcd.clear();
}
// Buzzer functions with relatively different sounds
void playBoomSound() {
  if(soundEnabled){
  tone(buzzerPin, 400, 100);
  delay(500);
  noTone(buzzerPin);
  }
}

void playCrashSound() {
  if(soundEnabled){
  tone(buzzerPin, 300, 100); 
  delay(200); 
  noTone(buzzerPin);
  }
}

void playCountdownSound() {
  if(soundEnabled){
  tone(buzzerPin, 500, 200);
  delay(200);
  noTone(buzzerPin);
  }
}
void playLoserSound() {
  if (soundEnabled) {
    int melody[] = {
      392, 349, 330, 294, 294, 330, 349, 392, 392, 349, 330, 294,
      294, 330, 349, 392, 349, 294, 294, 349, 392, 349, 330, 294
    };
    
    int noteDurations[] = {
      4, 8, 8, 4, 4, 4, 8, 8, 4, 8, 8, 4,
      4, 4, 8, 8, 4, 4, 4, 8, 8, 4, 8, 8
    };

    for (int i = 0; i < sizeof(melody) / sizeof(melody[0]); i++) {
      int noteDuration = 1000 / noteDurations[i];
      tone(buzzerPin, melody[i], noteDuration);
      delay(noteDuration * 1.30);
      noTone(buzzerPin);
      delay(50);
    }
  }
}

// A function that helps the buzzer sing Jingles Bells more specially
 void playWinSound() {
  if(soundEnabled){
  int melody[] = {
    659, 659, 659, 659, 659, 659, 659, 784, 523, 587, 659,
    698, 698, 698, 698, 698, 659, 659, 659, 659, 587, 587,
    659, 587, 784
  };
   int noteDurations[] = {
     8, 8, 4, 8, 8, 4, 8, 8, 4, 8, 8, 4, 8, 8, 4, 8, 8, 4, 8, 8, 4, 8, 8, 4, 8, 8
   };

  for (int i = 0; i < sizeof(melody) / sizeof(melody[0]); i++) {

    int noteDuration = 1000 / noteDurations[i];
    tone(buzzerPin, melody[i], noteDuration);
    delay(noteDuration * 1.30); 
    noTone(buzzerPin);
    delay(50);
  }
  }
}


void playLifeLostSound() {
  if (soundEnabled) {
    tone(buzzerPin, 330, 200);  
    delay(250);  
    noTone(buzzerPin); 
  }
}
// void playBeep() {
//   if(soundEnabled){
//   tone(buzzerPin, 1000, 200);  
//   delay(250);  
//   noTone(buzzerPin);
//   }
// }

//the menu code is inspired from github
//playBeep is added after the presentation, for sounds during the menu, but I didn't have anything to test the functionality on (my kit is in Bucharest, I arrived home in the meantime)
