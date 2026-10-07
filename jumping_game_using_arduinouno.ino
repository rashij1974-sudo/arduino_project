#include <LCD_I2C.h>

LCD_I2C lcd(0x27, 16, 2);

const int irPin = 9;

// Obstacle position
int obstaclePos = 15;

// Score
int score = 0;

// Human character
byte human[8] = {
  B00100,
  B01110,
  B00100,
  B01110,
  B10101,
  B00100,
  B01010,
  B10001
};

// Obstacle character
byte obstacle[8] = {
  B00100,
  B00100,
  B10100,
  B11100,
  B10100,
  B00100,
  B00100,
  B00100
};

void setup()
{
  pinMode(irPin, INPUT);

  lcd.begin();
  lcd.backlight();

  lcd.createChar(0, human);
  lcd.createChar(1, obstacle);

  // Starting message
  lcd.setCursor(2, 0);
  lcd.print("JUMPING GAME");

  delay(2000);
  lcd.clear();
}

void loop()
{
  int sensorState = digitalRead(irPin);

  // Clear LCD
  lcd.clear();

  // --------------------------------
  // DISPLAY SCORE
  // --------------------------------

  lcd.setCursor(9, 0);
  lcd.print("S:");
  lcd.print(score);

  // --------------------------------
  // HUMAN
  // --------------------------------

  if (sensorState == LOW)
  {
    // IR detects obstacle
    // Human jumps
    lcd.setCursor(1, 0);
    lcd.write(byte(0));
  }
  else
  {
    // No obstacle detected
    // Human stays on ground
    lcd.setCursor(1, 1);
    lcd.write(byte(0));
  }

  // --------------------------------
  // OBSTACLE
  // --------------------------------

  lcd.setCursor(obstaclePos, 1);
  lcd.write(byte(1));

  // --------------------------------
  // COLLISION DETECTION
  // --------------------------------

  // Human is at position 1.
  // If obstacle reaches position 1
  // while human is on ground, GAME OVER.

  if (obstaclePos == 1 && sensorState == HIGH)
  {
    gameOver();
    return;
  }

  // --------------------------------
  // MOVE OBSTACLE
  // --------------------------------

  obstaclePos--;

  // --------------------------------
  // SUCCESSFUL JUMP
  // --------------------------------

  if (obstaclePos < 0)
  {
    score++;

    obstaclePos = 15;
  }

  delay(300);
}


// =================================
// GAME OVER FUNCTION
// =================================

void gameOver()
{
  lcd.clear();

  lcd.setCursor(3, 0);
  lcd.print("GAME OVER!");

  delay(1500);

  lcd.clear();

  lcd.setCursor(2, 0);
  lcd.print("TOTAL SCORE");

  lcd.setCursor(6, 1);
  lcd.print(score);

  delay(3000);

  // Restart game
  score = 0;
  obstaclePos = 15;

  lcd.clear();

  lcd.setCursor(2, 0);
  lcd.print("NEW GAME");

  delay(1500);

  lcd.clear();
}