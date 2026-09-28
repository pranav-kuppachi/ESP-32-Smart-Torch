// ESP32 Smart Torch
// Modes:
// 0 = Automatic LDR mode
// 1 = Manual potentiometer mode

// Pin Configuration
const int BLUE_LED  = 25;
const int WHITE_LED = 26;
const int CYAN_LED  = 27;

const int MODE_BUTTON = 14;
const int POT_PIN     = 34;
const int LDR_PIN     = 35;

// PWM Configuration 
const int PWM_FREQUENCY = 5000;
const int PWM_RESOLUTION = 8;
const int MAX_BRIGHTNESS = 255;

//  Variables 
int mode = 0;  // 0 = Auto, 1 = Manual

bool lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 200;


// Setup 
void setup() {
  Serial.begin(115200);

  // Button
  pinMode(MODE_BUTTON, INPUT_PULLUP);

  // PWM setup
  ledcAttach(BLUE_LED, PWM_FREQUENCY, PWM_RESOLUTION);
  ledcAttach(WHITE_LED, PWM_FREQUENCY, PWM_RESOLUTION);
  ledcAttach(CYAN_LED, PWM_FREQUENCY, PWM_RESOLUTION);

  // Start with all LEDs OFF
  setLEDs(0, 0, 0);

  Serial.println("================================");
  Serial.println("     ESP32 SMART TORCH");
  Serial.println("================================");
  Serial.println("Mode: AUTOMATIC");
}


//  Main  
void loop() {

  // Check whether MODE button was pressed
  handleButton();

  int brightnessPercent;

  if (mode == 0) {
    //  AUTOMATIC MODE 
    int ldrValue = analogRead(LDR_PIN);

    // Invert LDR reading:
    // More light -> lower value
    // Less light -> higher brightness needed
    int lightLevel = 4095 - ldrValue;

    brightnessPercent = map(lightLevel, 0, 4095, 0, 100);
    brightnessPercent = constrain(brightnessPercent, 0, 100);

    Serial.print("AUTO | LDR: ");
    Serial.print(ldrValue);
    Serial.print(" | Brightness: ");
    Serial.print(brightnessPercent);
    Serial.println("%");

  } else {
    //  MANUAL MODE 
    int potValue = analogRead(POT_PIN);

    brightnessPercent = map(potValue, 0, 4095, 0, 100);
    brightnessPercent = constrain(brightnessPercent, 0, 100);

    Serial.print("MANUAL | POT: ");
    Serial.print(potValue);
    Serial.print(" | Brightness: ");
    Serial.print(brightnessPercent);
    Serial.println("%");
  }

  // Control LEDs based on brightness
  controlLEDs(brightnessPercent);

  delay(50);
}


//  Button Handling 
void handleButton() {

  bool buttonState = digitalRead(MODE_BUTTON);

  // Detect button press
  if (lastButtonState == HIGH && buttonState == LOW) {

    if (millis() - lastDebounceTime > debounceDelay) {

      mode = !mode;

      lastDebounceTime = millis();

      if (mode == 0) {
        Serial.println(">>> Switched to AUTOMATIC mode");
      } else {
        Serial.println(">>> Switched to MANUAL mode");
      }
    }
  }

  lastButtonState = buttonState;
}


// LED Control 
void controlLEDs(int brightnessPercent) {

  // Convert percentage to PWM value
  int brightness = map(
    brightnessPercent,
    0,
    100,
    0,
    MAX_BRIGHTNESS
  );

  brightness = constrain(brightness, 0, MAX_BRIGHTNESS);

  // Turn all LEDs off first
  int blueBrightness = 0;
  int cyanBrightness = 0;
  int whiteBrightness = 0;

  // Select LED according to brightness range
  if (brightnessPercent <= 33) {

    // Low brightness = WHITE
    whiteBrightness = brightness;

  } else if (brightnessPercent <= 66) {

    // Medium brightness = CYAN
    cyanBrightness = brightness;

  } else {

    // High brightness = BLUE
    blueBrightness = brightness;
  }

  setLEDs(
    blueBrightness,
    whiteBrightness,
    cyanBrightness
  );
}


//  Write PWM Values 
void setLEDs(
  int blueBrightness,
  int whiteBrightness,
  int cyanBrightness
) {

  ledcWrite(BLUE_LED, blueBrightness);
  ledcWrite(WHITE_LED, whiteBrightness);
  ledcWrite(CYAN_LED, cyanBrightness);
}