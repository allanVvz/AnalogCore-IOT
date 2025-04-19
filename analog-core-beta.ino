#include <Mouse.h>
#include <Keyboard.h>

// === Pinos ===
const int btnX  =   15;
const int vrxPin = A0;
const int vryPin = A1;
const int btnLeft  = 9;
const int btnUp    = 8;
const int btnRight = 10;
const int btnDown  = 16;
const int btnComMode  =   15;

// === delays === 
const int smoothDelay = 500;
const int clickDelay = 100;

// === Parâmetros ===
const int center = 512;
const float sensitivity = 0.03;
const int samples = 10;

// === Inicialização ===
void setup() {
  Serial.begin(9600);
  while (!Serial);

  Mouse.begin();
  Keyboard.begin();

  initJoystick();
  setupButtons();
}

// ===  Loop principal  === //

void loop() {
  int deltaX, deltaY;
  readJoystick(deltaX, deltaY);
  moveMouse(deltaX, deltaY);
  readButtons();
  readMouseClick();
  delay(10);
}

// ==========----=========== //


void initJoystick() {
  pinMode(vrxPin, INPUT);
  pinMode(vryPin, INPUT);
}


int smoothAnalogRead(int pin) {
  long total = 0;
  for (int i = 0; i < samples; i++) {
    total += analogRead(pin);
    delayMicroseconds(smoothDelay);
  }
  return total / samples;
}


void readJoystick(int &deltaX, int &deltaY) {
  int xReading = smoothAnalogRead(vrxPin);
  int yReading = smoothAnalogRead(vryPin);

  deltaX = (xReading - center) * sensitivity;
  deltaY = (yReading - center) * sensitivity;
}


void moveMouse(int x, int y) {
  if (x != 0 || y != 0) {
    Mouse.move(x, -y); // Y invertido
  }
}


void setupButtons() {
  pinMode(btnLeft, INPUT_PULLUP);
  pinMode(btnUp, INPUT_PULLUP);
  pinMode(btnRight, INPUT_PULLUP);
  pinMode(btnDown, INPUT_PULLUP);
  pinMode(btnX, INPUT_PULLUP);
}


void readMouseClick() {
  if (digitalRead(btnX) == LOW){
    cliqueEsquerdoMouse();
  }
}


void readButtons() {
  if (digitalRead(btnLeft) == LOW) {
    Keyboard.press(KEY_LEFT_ARROW);
  } else {
    Keyboard.release(KEY_LEFT_ARROW);
  }

  if (digitalRead(btnUp) == LOW) {
    Keyboard.press(KEY_UP_ARROW);
  } else {
    Keyboard.release(KEY_UP_ARROW);
  }

  if (digitalRead(btnRight) == LOW) {
    Keyboard.press(KEY_RIGHT_ARROW);
  } else {
    Keyboard.release(KEY_RIGHT_ARROW);
  }

  if (digitalRead(btnDown) == LOW) {
    Keyboard.press(KEY_DOWN_ARROW);
  } else {
    Keyboard.release(KEY_DOWN_ARROW);
  }
}


// 🖱️ Função para executar um clique esquerdo do mouse
void cliqueEsquerdoMouse() {
  Mouse.press(MOUSE_LEFT);  // Pressiona o botão esquerdo
  delay(clickDelay);         // Mantém pressionado por 100 ms
  Mouse.release(MOUSE_LEFT); // Solta o botão
}
