#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "pitches.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1
#define BOARD_COLUMNS 10
#define BOARD_ROWS 20
#define BOARD_X 1
#define BOARD_Y 27
#define TOTAL_PIECES 7
#define TOTAL_ROTATIONS 4
#define PIECE_SIDE 4
#define PIXEL_SIZE 5
#define LEFT_BUTTON_PIN 4
#define RIGHT_BUTTON_PIN 2
#define ROTATE_BUTTON_PIN 3
#define LEFT 0
#define RIGHT 1
#define ROTATE 2
#define START_X 4
#define START_Y 0
#define LOGO_HEIGHT   40
#define LOGO_WIDTH    64
#define LINE_SCORE 10

#define START_SCREEN 0
#define PLAYING 1
#define GAME_OVER 2

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

static const unsigned char  PIECES[TOTAL_PIECES][TOTAL_ROTATIONS][PIECE_SIDE][PIECE_SIDE] PROGMEM =
{
  {
    //L PIECE
    {
      {1, 1, 1, 0},
      {1, 0, 0, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0}
    },
    {
      {1, 1, 0, 0},
      {0, 1, 0, 0},
      {0, 1, 0, 0},
      {0, 0, 0, 0}
    },
    {
      {0, 0, 1, 0},
      {1, 1, 1, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0}
    },
    {
      {1, 0, 0, 0},
      {1, 0, 0, 0},
      {1, 1, 0, 0},
      {0, 0, 0, 0}
    },
  },
  {
    //L-INVERTED PIECE
    {
      {1, 1, 1, 0},
      {0, 0, 1, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0}
    },
    {
      {0, 1, 0, 0},
      {0, 1, 0, 0},
      {1, 1, 0, 0},
      {0, 0, 0, 0}
    },
    {
      {1, 0, 0, 0},
      {1, 1, 1, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0}
    },
    {
      {1, 1, 0, 0},
      {1, 0, 0, 0},
      {1, 0, 0, 0},
      {0, 0, 0, 0}
    },
  },
  {
    //SQUARE PIECE
    {
      {1, 1, 0, 0},
      {1, 1, 0, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0}
    },
    {
      {1, 1, 0, 0},
      {1, 1, 0, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0}
    },
    {
      {1, 1, 0, 0},
      {1, 1, 0, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0}
    },
    {
      {1, 1, 0, 0},
      {1, 1, 0, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0}
    }
  },
  {
    //Z PIECE
    {
      {1, 1, 0, 0},
      {0, 1, 1, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0}
    },
    {
      {0, 1, 0, 0},
      {1, 1, 0, 0},
      {1, 0, 0, 0},
      {0, 0, 0, 0}
    },
    {
      {1, 1, 0, 0},
      {0, 1, 1, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0}
    },
    {
      {0, 1, 0, 0},
      {1, 1, 0, 0},
      {1, 0, 0, 0},
      {0, 0, 0, 0}
    }
  },
  {
    //Z-INVERTED PIECE
    {
      {0, 1, 1, 0},
      {1, 1, 0, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0}
    },
    {
      {1, 0, 0, 0},
      {1, 1, 0, 0},
      {0, 1, 0, 0},
      {0, 0, 0, 0}
    },
    {
      {0, 1, 1, 0},
      {1, 1, 0, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0}
    },
    {
      {1, 0, 0, 0},
      {1, 1, 0, 0},
      {0, 1, 0, 0},
      {0, 0, 0, 0}
    }
  },
  {
    //T PIECE
    {
      {0, 1, 0, 0},
      {1, 1, 1, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0}
    },
    {
      {1, 0, 0, 0},
      {1, 1, 0, 0},
      {1, 0, 0, 0},
      {0, 0, 0, 0}
    },
    {
      {1, 1, 1, 0},
      {0, 1, 0, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0}
    },
    {
      {0, 1, 0, 0},
      {1, 1, 0, 0},
      {0, 1, 0, 0},
      {0, 0, 0, 0}
    }
  },
  {
    //| PIECE
    {
      {1, 1, 1, 1},
      {0, 0, 0, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0}
    },
    {
      {1, 0, 0, 0},
      {1, 0, 0, 0},
      {1, 0, 0, 0},
      {1, 0, 0, 0}
    },
    {
      {1, 1, 1, 1},
      {0, 0, 0, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0}
    },
    {
      {1, 0, 0, 0},
      {1, 0, 0, 0},
      {1, 0, 0, 0},
      {1, 0, 0, 0}
    }
  }
};

static const unsigned char PROGMEM logo[] =
{ 
B00000000, B00000000, B00000000, B11000000, B11000000, B00000000, B00000000, B00000000,
B00000000, B00000000, B00000000, B11000000, B11000000, B00000000, B00000000, B00000000, 
B00000000, B00000000, B00000011, B00110011, B00110000, B00000000, B00000000, B00000000,
B00000000, B00000000, B00000011, B00110011, B00110000, B00000000, B00000000, B00000000, 
B00000000, B00000000, B00001100, B11001100, B11001100, B00000000, B00000000, B00000000,
B00000000, B00000000, B00001100, B11001100, B11001100, B00000000, B00000000, B00000000, 
B00000000, B00000000, B00001111, B11111111, B11111100, B00000000, B00000000, B00000000,
B00000000, B00000000, B00001111, B11111111, B11111100, B00000000, B00000000, B00000000, 
B00000000, B00000000, B00110000, B00000000, B00000011, B00000000, B00000000, B00000000,
B00000000, B00000000, B00110000, B00000000, B00000011, B00000000, B00000000, B00000000, 
B00000000, B00000000, B00110000, B00000000, B00000011, B00000000, B00000000, B00000000,
B00000000, B00000000, B00110000, B00000000, B00000011, B00000000, B00000000, B00000000, 
B00000000, B00000000, B00110000, B11000011, B00000011, B00000000, B00000000, B00000000,
B00000000, B00000000, B00110000, B11000011, B00000011, B00000000, B00000000, B00000000, 
B00000000, B00000000, B00110000, B11110011, B11000011, B00000000, B00000000, B00000000,
B00000000, B00000000, B00110000, B11110011, B11000011, B00000000, B00000000, B00000000, 
B00000000, B00000000, B00110000, B00000000, B00000011, B00000000, B00000000, B00000000,
B00000000, B00000000, B00110000, B00000000, B00000011, B00000000, B00000000, B00000000, 
B00000000, B00000000, B00110000, B00110011, B00000011, B00000000, B00000000, B00000000,
B00000000, B00000000, B00110000, B00110011, B00000011, B00000000, B00000000, B00000000, 
B00000000, B00000000, B00110000, B00111111, B00000011, B00000000, B00000000, B00000000,
B00000000, B00000000, B00110000, B00111111, B00000011, B00000000, B00000000, B00000000, 
B00000000, B00000000, B00001100, B00001100, B00001100, B00000000, B00000000, B00000000,
B00000000, B00000000, B00001100, B00001100, B00001100, B00000000, B00000000, B00000000, 
B00000000, B00000000, B00001111, B11111111, B11111100, B00000000, B00000000, B00000000,
B00000000, B00000000, B00001111, B11111111, B11111100, B00000000, B00000000, B00000000, 
B11110000, B11110000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
B11110000, B11110000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
B11110000, B11000011, B00111100, B11001100, B00000000, B11110000, B11111100, B00000000,
B11110000, B11000011, B00111100, B11001100, B00000000, B11110000, B11111100, B00000000, 
B11110000, B11000000, B00110000, B11001100, B00000000, B11000011, B00110000, B00000000,
B11110000, B11000000, B00110000, B11001100, B00000000, B11000011, B00110000, B00000000, 
B11111100, B11110011, B00111100, B11111100, B11000000, B11000000, B00110000, B00000000,
B11111100, B11110011, B00111100, B11111100, B11000000, B11000000, B00110000, B00000000, 
B11111100, B11000011, B00001100, B00110000, B11111100, B11110011, B00110000, B00001100,
B11111100, B11000011, B00001100, B00110000, B11111100, B11110011, B00110000, B00001100, 
B11111100, B11000011, B00001100, B00110000, B11001100, B11000011, B00110011, B11001111,
B11111100, B11000011, B00001100, B00110000, B11001100, B11000011, B00110011, B11001111, 
B11111100, B11110011, B00111100, B00110000, B11001100, B11110011, B00110011, B11001100,
B11111100, B11110011, B00111100, B00110000, B11001100, B11110011, B00110011, B11001100
};

unsigned char board[BOARD_ROWS][BOARD_COLUMNS];
unsigned char buttons[3];

void setup() {
  Serial.begin(9600);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  pinMode(LEFT_BUTTON_PIN, INPUT_PULLUP);
  pinMode(RIGHT_BUTTON_PIN, INPUT_PULLUP);
  pinMode(ROTATE_BUTTON_PIN, INPUT_PULLUP);

  
  display.clearDisplay();
  display.setRotation(3);
  randomSeed(analogRead(0));
}

int prev_ms;
int piece = 0;
int next_piece = 0;
int rotation = 0;
int piece_x = 0;
int piece_y = 0;
int level = 0;
int score = 0;
int total_lines = 0;
int state = START_SCREEN;

void initGame() {
  for (int r = 0; r < BOARD_ROWS; r++) {
    for (int c = 0; c < BOARD_COLUMNS; c++) {
      board[r][c] = 0;
    }
  }
  buttons[LEFT] = 0;
  buttons[RIGHT] = 0;
  buttons[ROTATE] = 0;
  level = 0;
  score = 0;
  total_lines = 0;
  prev_ms = millis();
  next_piece = random(TOTAL_PIECES);
  randomPiece();
  playMusic();
  state = PLAYING;
  
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
}

void randomPiece() {
  piece = next_piece;
  do {
    next_piece = random(TOTAL_PIECES);
  } while(next_piece == piece);
  rotation = 0;
  piece_y = START_Y;
  piece_x = START_X;
}

void transferPieceToBoard() {
  for (int16_t j = 0; j < PIECE_SIDE; j++) {
    for (int16_t i = 0; i < PIECE_SIDE; i++) {
      if (pgm_read_byte(&(PIECES[piece][rotation][j][i]))) {
        board[piece_y + j][piece_x + i] = 1;
      }
    }
  }
}

void clearLines() {
  int multiplier = 1;
  for (int r = 0; r < BOARD_ROWS; r++) {
    bool line = true;
    for (int c = 0; c < BOARD_COLUMNS; c++) {
      if (!board[r][c]) {
        line = false;
        break;
      }
    }
    if (line) {
      for (int rr = r; rr > 0; rr--) {
        for (int c = 0; c < BOARD_COLUMNS; c++) {
          board[rr][c] = board[rr - 1][c];
        }
      }
      score = score + (LINE_SCORE * multiplier);
      multiplier++;
      total_lines++;
      if (total_lines % 10 == 0) {
        level++;
      }
    }
  }
}

void readButtons() {
  buttons[LEFT] = digitalRead(LEFT_BUTTON_PIN) == 0;
  buttons[RIGHT] = digitalRead(RIGHT_BUTTON_PIN) == 0;
  buttons[ROTATE] = digitalRead(ROTATE_BUTTON_PIN) == 0;
}

bool movePiece(int x, int y) {
  bool bottom = false;
  for (int16_t j = 0; j < PIECE_SIDE && !bottom; j++) {
    for (int16_t i = 0; i < PIECE_SIDE && !bottom; i++) {
      if (pgm_read_byte(&(PIECES[piece][rotation][j][i]))) {
        if (x + i < 0) {
          x = 0;
        } else if (x + i >= BOARD_COLUMNS) {
          x = BOARD_COLUMNS - i - 1;
        }
        if (y + j >= BOARD_ROWS) {
          y = BOARD_ROWS - j - 1;
          bottom = true;
        }
        if (board[y + j][x + i]) {
          y = BOARD_ROWS - j - 1;
          bottom = true;
        }
      }
    }
  }
  piece_x = x;
  piece_y = y;
  return bottom;
}

void pieceMoveLeft() {
  if (!piece_x) {
    return;
  }
  int new_x = piece_x - 1;
  for (int16_t j = 0; j < PIECE_SIDE; j++) {
    for (int16_t i = 0; i < PIECE_SIDE; i++) {
      if (pgm_read_byte(&(PIECES[piece][rotation][j][i]))) {
        if (board[piece_y + j][new_x + i]) {
          return;
        }
      }
    }
  }
  piece_x = new_x;
}

void pieceMoveRight() {
  int new_x = piece_x + 1;
  for (int16_t j = 0; j < PIECE_SIDE; j++) {
    for (int16_t i = 0; i < PIECE_SIDE; i++) {
      if (pgm_read_byte(&(PIECES[piece][rotation][j][i]))) {
        if (new_x + i >= BOARD_COLUMNS) {
          return;
        }
        if (board[piece_y + j][new_x + i]) {
          return;
        }
      }
    }
  }
  piece_x = new_x;
}

void pieceRotate() {
  int new_rotation = (rotation + 1) % TOTAL_ROTATIONS;
  int x = piece_x;
  int y = piece_y;
  for (int16_t j = 0; j < PIECE_SIDE; j++) {
    for (int16_t i = 0; i < PIECE_SIDE; i++) {
      if (pgm_read_byte(&(PIECES[piece][new_rotation][j][i]))) {
        if (x + i >= BOARD_COLUMNS) {
          x = BOARD_COLUMNS - i - 1;
          j = -1;
          break;
        }
        if (y + j >= BOARD_ROWS) {
          y = BOARD_ROWS - j - 1;
          j = -1;
          break;
        }
        if (board[x + j][y + i]) {
          return;
        }
      }
    }
  }
  piece_x = x;
  piece_y = y;
  rotation = new_rotation;
}

bool pieceMoveDown() {
  int new_y = piece_y + 1;
  for (int16_t j = 0; j < PIECE_SIDE; j++) {
    for (int16_t i = 0; i < PIECE_SIDE; i++) {
      if (pgm_read_byte(&(PIECES[piece][rotation][j][i]))) {
        if (new_y + j >= BOARD_ROWS) {
          return true;
        }
        if (board[new_y + j][piece_x + i]) {
          return true;
        }
      }
    }
  }
  piece_y = new_y;
  return false;
}

int buttons_prev_ms;
int rotate_prev_ms;
void update(int ms) {
  if (ms - buttons_prev_ms >= 90) {
    buttons_prev_ms = ms;
    if (buttons[LEFT]) {
      pieceMoveLeft();
    } else if (buttons[RIGHT]) {
      pieceMoveRight();
    }
  }
  if (ms - rotate_prev_ms >= 120) {
    rotate_prev_ms = ms;
    if (buttons[ROTATE]) {
      pieceRotate();
    }
  }
  if (ms - prev_ms >= 250 - (level * 25)) {
    prev_ms = ms;
    if (pieceMoveDown()) {
      if(piece_x == START_X && piece_y == START_Y) {
        state = GAME_OVER;
      }
      transferPieceToBoard();
      clearLines();
      randomPiece();
    }
  }
}

void draw() {
  display.clearDisplay();
  display.drawRect(BOARD_X - 1,BOARD_Y - 1,BOARD_COLUMNS*PIXEL_SIZE + 2, BOARD_ROWS*PIXEL_SIZE + 2, 1);
  for (int r = 0; r < BOARD_ROWS; r++) {
    for (int c = 0; c < BOARD_COLUMNS; c++) {
      drawPixel(c, r, board[r][c]);
    }
  }
  drawPiece();
  drawNextPiece();
  drawScore();
  display.display();
}

void loop() {
  readButtons();
  if (state == START_SCREEN) {
    drawLogo();
    if (buttons[ROTATE]) {
      initGame();
    }
  } else if(state == PLAYING) {
    update(millis());
    draw();
  } else if (state == GAME_OVER) {
    display.fillRect(0,45,display.width(),28, 0);
    display.drawRect(0,45,display.width(),28, 1);
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(10,48);
    display.print(F("PERDISTE\n    POR\n   LULU!"));
    display.display();
    if (buttons[ROTATE]) {
      initGame();
    }
  }
  delay(0);
}

void drawPiece() {
  for (int16_t i = 0; i < PIECE_SIDE; i++) {
    for (int16_t j = 0; j < PIECE_SIDE; j++) {
      if (pgm_read_byte(&(PIECES[piece][rotation][j][i]))) {
        drawPixel(piece_x + i, piece_y + j, 1);
      }
    }
  }
}

void drawNextPiece() {
  display.setCursor(0,0);
  display.print(F("Nxt"));
  for (int16_t i = 0; i < PIECE_SIDE; i++) {
    for (int16_t j = 0; j < PIECE_SIDE; j++) {
      if (pgm_read_byte(&(PIECES[next_piece][0][j][i]))) {
        drawPixel(i, j - 3, 1);
      }
    }
  }
}

void drawScore() {
  display.setCursor(24,0);
  display.print(F("Score"));
  display.setCursor(24,12);
  display.print(score);
  
  display.setRotation(0);
  display.setCursor(BOARD_Y,0);
  display.print(F("Lvl"));
  display.setCursor(BOARD_Y + 22,0);
  display.print(level + 1);
  display.setCursor(BOARD_Y + 44,0);
  display.print(F("Lns"));
  display.setCursor(BOARD_Y + 66,0);
  display.print(total_lines);
  display.setRotation(3);
}

void drawPixel(int x, int y, int color) {
  display.drawRect(x * PIXEL_SIZE + BOARD_X, y * PIXEL_SIZE + BOARD_Y, PIXEL_SIZE, PIXEL_SIZE, color);
}

void drawLogo() {
  display.clearDisplay();

  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2,
    (display.height() - LOGO_HEIGHT) / 2,
    logo, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display();
}

void playMusic() {
  int melody[] = {
    NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
  };
  int noteDurations[] = {
    4, 8, 8, 4, 4, 4, 4, 4
  };
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 8; thisNote++) {

    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(A1, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(A1);
  }
}
