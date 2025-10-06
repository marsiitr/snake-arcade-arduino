#include <LedControl.h>

// LED Matrix DIN, CLK, CS, NumDevices
LedControl lc = LedControl(12, 11, 10, 1);

int score=0;

// Joystick pins
#define VRx A0
#define VRy A1
#define SW 2 

// Matrix size
#define SIZE 8
#define MAX_LEN 32

// Directions
enum Direction { UP, DOWN, LEFT, RIGHT };
Direction dir = RIGHT;

// Snake
int snakeX[MAX_LEN], snakeY[MAX_LEN];
int length = 3;

// Fruit
int fruitX, fruitY;

unsigned long lastMove = 0;
int speedDelay = 500; // ms

bool gameOver = false;

void setup() {
  lc.shutdown(0, false);
  lc.setIntensity(0, 4);
  lc.clearDisplay(0);

  // Joystick input
  pinMode(VRx, INPUT);
  pinMode(VRy, INPUT);
  pinMode(SW, INPUT_PULLUP);


  // Init snake in center
  int startX = SIZE / 2;
  int startY = SIZE / 2;
  for (int i = 0; i < length; i++) {
    snakeX[i] = startX - i;
    snakeY[i] = startY;
  }

  spawnFruit();
}

void loop() {
  // If game is over, wait for joystick button press to reset
  if (gameOver) {
    if (digitalRead(SW) == LOW) { // button pressed
      delay(200); // debounce
      resetGame();
    }
    return; // stop moving snake until reset
  }

  // Normal game logic
  if (millis() - lastMove > speedDelay) {
    readInput();
    moveSnake();

    if (isCollision()) {
      showGameOver();
      gameOver = true;
      return;
    }

    if (ateFruit()) {
      growSnake();
      spawnFruit();
      score += 1;
      speedDelay = max(100, speedDelay - 10);
    }

    draw();
    lastMove = millis();
  }
}


void resetGame() {
  lc.clearDisplay(0);
  
  // Reset snake
  length = 3;
  dir = RIGHT;
  score = 0;
  speedDelay = 500;
  gameOver = false;
  lastMove = millis();

  // Initialize snake in center
  int startX = SIZE / 2;
  int startY = SIZE / 2;
  for (int i = 0; i < length; i++) {
    snakeX[i] = startX - i;
    snakeY[i] = startY;
  }

  // Spawn fruit
  spawnFruit();
}

void readInput() {
  int x = analogRead(VRx);
  int y = analogRead(VRy);

  if (x < 300 && dir != RIGHT) dir = LEFT;
  else if (x > 700 && dir != LEFT) dir = RIGHT;
  else if (y < 300 && dir != DOWN) dir = UP;
  else if (y > 700 && dir != UP) dir = DOWN;
}

void moveSnake() {
  // shift body
  for (int i = length - 1; i > 0; i--) {
    snakeX[i] = snakeX[i - 1];
    snakeY[i] = snakeY[i - 1];
  }

  if (dir == RIGHT) snakeX[0]++;
  else if (dir == LEFT) snakeX[0]--;
  else if (dir == UP) snakeY[0]--;
  else if (dir == DOWN) snakeY[0]++;
}

bool isCollision() {
  // wall
  if (snakeX[0] < 0 || snakeX[0] >= SIZE || snakeY[0] < 0 || snakeY[0] >= SIZE)
    return true;

  // self
  for (int i = 1; i < length; i++) {
    if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i])
      return true;
  }
  return false;
}

bool ateFruit() {
  return (snakeX[0] == fruitX && snakeY[0] == fruitY);
}

void growSnake() {
  if (length < MAX_LEN) {
    snakeX[length] = snakeX[length - 1];
    snakeY[length] = snakeY[length - 1];
    length++;
  }
}

void spawnFruit() {
  bool valid;
  do {
    valid = true;
    fruitX = random(0, SIZE);
    fruitY = random(0, SIZE);
    for (int i = 0; i < length; i++) {
      if (snakeX[i] == fruitX && snakeY[i] == fruitY) {
        valid = false;
        break;
      }
    }
  } while (!valid);
}

void draw() {
  lc.clearDisplay(0);

  // snake
  for (int i = 0; i < length; i++) {
    lc.setLed(0, snakeY[i], snakeX[i], true);
  }

  // fruit
  lc.setLed(0, fruitY, fruitX, true);
}


void showGameOver() {
  lc.clearDisplay(0);

  if (score < 10) {
    showDigit(score, 2); // show digit centered at column 2
  } else {
    int tens = score / 10;
    int ones = score % 10;
    showDigit(tens, 0);
    showDigit(ones, 4);
  }

  delay(2000); // Show score for 2 seconds

  for (int k = 0; k < 3; k++) {
    lc.clearDisplay(0);
    delay(200);
    
    for (int y = 0; y < SIZE; y++)
      for (int x = 0; x < SIZE; x++)
        lc.setLed(0, y, x, true);
    delay(200);
  }
  lc.clearDisplay(0);
}


// Draws a single digit (0–9) on the LED matrix at a given starting column
void showDigit(int num, int offsetX) {
  static const byte digits[10][5] = {
    {B01110, B10001, B10001, B10001, B01110}, // 0
    {B00100, B01100, B00100, B00100, B01110}, // 1
    {B01110, B10001, B00010, B00100, B11111}, // 2
    {B11110, B00001, B00110, B00001, B11110}, // 3
    {B00010, B00110, B01010, B11111, B00010}, // 4
    {B11111, B10000, B11110, B00001, B11110}, // 5
    {B01110, B10000, B11110, B10001, B01110}, // 6
    {B11111, B00010, B00100, B01000, B10000}, // 7
    {B01110, B10001, B01110, B10001, B01110}, // 8
    {B01110, B10001, B01111, B00001, B01110}  // 9
  };

  for (int row = 0; row < 5; row++) {
    byte line = digits[num][row];
    for (int col = 0; col < 5; col++) {
      if (line & (1 << (4 - col))) {
        int x = offsetX + col;
        int y = row + 1; // vertically centered
        if (x >= 0 && x < SIZE && y >= 0 && y < SIZE)
          lc.setLed(0, y, x, true);
      }
    }
  }
}
