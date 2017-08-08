#include <Arduboy2.h>
Arduboy2 arduboy;

int16_t x1 = 48;
int16_t y1 = 16;
int16_t x2 = 84;
int16_t y2 = 22;

bool checkCollision = false;

enum class Direction : uint8_t {
  None = 0,
  Up = 1,  
  Down = 2,
  Left = 4,
  Right = 8,
};

constexpr Direction operator |(const Direction &left, const Direction &right) {
  return static_cast<Direction>(static_cast<uint8_t>(left) | static_cast<uint8_t>(right));
}

constexpr Direction operator &(const Direction &left, const Direction &right) {
  return static_cast<Direction>(static_cast<uint8_t>(left) & static_cast<uint8_t>(right));
}

void setup() {

  arduboy.begin();
  arduboy.setFrameRate(15);
  
}

void loop() {

  arduboy.clear();

  if (arduboy.pressed(UP_BUTTON) && y2 > -32)                              { --y2; checkCollision = true; }
  if (arduboy.pressed(DOWN_BUTTON) && y2 < 64)                             { ++y2; checkCollision = true; }
  if (arduboy.pressed(LEFT_BUTTON) && x2 > -32)                            { --x2; checkCollision = true; }
  if (arduboy.pressed(RIGHT_BUTTON) && x2 < 128)                           { ++x2; checkCollision = true; }

  arduboy.drawRect(x1, y1, 32, 32);
  arduboy.drawRect(x2, y2, 32, 32);
  arduboy.display();

  if (checkCollision) { 

    Rect r1 = {x1, y1, 32, 32};
    Rect r2 = {x2, y2, 32, 32};
    
    Direction direction = collide(r1, r2);

    if (direction != Direction::None) {

      Serial.print("Relative to the fixed rectangle, the moving rectangle is ");
      
      if ((direction & Direction::Up) == Direction::Up)         { Serial.print("above "); }
      if ((direction & Direction::Down) == Direction::Down)     { Serial.print("below "); }
      if ((direction & Direction::Left) == Direction::Left)     { Serial.print("left "); }
      if ((direction & Direction::Right) == Direction::Right)   { Serial.print("right "); }

      Serial.println(".");
      
    }
    
  }
  
  checkCollision = false;
  
  delay(50);

}






/* -----------------------------------------------------------------------------------------------------------------------------
 *  Determine if the Rect1 has collided with Rect2 in the nominated direction.
 *       
 *  Note that the direction is relative to Rect1.       
 */
bool collide(Rect rect1, Rect rect2, Direction testDirection) {
  
  return ((collide(rect1, rect2) & testDirection) == testDirection);
  
}


/* -----------------------------------------------------------------------------------------------------------------------------
 *  Determine on which sides two Rects may have collided.
 *       
 *  Note that the returned directions are relative to Rect1.       
 */
Direction collide(Rect rect1, Rect rect2) {

  Direction direction = Direction::None;
 
  if (!(rect2.x                >= rect1.x + rect1.width  ||
        rect2.x + rect2.width  <= rect1.x                ||
        rect2.y                >= rect1.y + rect1.height ||
        rect2.y + rect2.height <= rect1.y)) {

    if ((rect1.x < rect2.x) && (rect1.x + rect1.width > rect2.x))    direction = direction | Direction::Right;     // Rect 2 is to the right of Rect 1?
    if ((rect2.x < rect1.x) && (rect2.x + rect2.width > rect1.x))    direction = direction | Direction::Left;      // Rect 2 is to the left of Rect 1?
    if ((rect2.y < rect1.y) && (rect2.y + rect2.height > rect1.y))   direction = direction | Direction::Up;        // Rect 2 is above Rect 1?
    if ((rect1.y < rect2.y) && (rect1.y + rect1.height > rect2.y))   direction = direction | Direction::Down;      // Rect 2 is below Rect 1?

  }

  return direction;

}
