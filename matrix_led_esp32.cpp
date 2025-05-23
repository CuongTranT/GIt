#include <Arduino.h>

// ============================
// ===== DEFINE GPIO PINS =====
// ============================
#define R0 25
#define G0 26
#define B0 27
#define R1 14
#define G1 12
#define B1 13
#define ROW_A 23
#define ROW_B 19
#define CLK 16
#define LAT 4
#define OE 15

// =========================
// ===== DISPLAY BUFFER ====
// =========================
uint8_t Buffer_display[16][4]; // Buffer 16 hàng, 4 byte/hàng


// ==========================
// ===== SETUP PINS ========
// ==========================
void setupPins() {
  pinMode(R0, OUTPUT);
  pinMode(G0, OUTPUT);
  pinMode(B0, OUTPUT);
  pinMode(R1, OUTPUT);
  pinMode(G1, OUTPUT);
  pinMode(B1, OUTPUT);

  pinMode(ROW_A, OUTPUT);
  pinMode(ROW_B, OUTPUT);
  pinMode(CLK, OUTPUT);
  pinMode(LAT, OUTPUT);
  pinMode(OE, OUTPUT);

  digitalWrite(OE, HIGH);
  digitalWrite(LAT, LOW);
  digitalWrite(CLK, LOW);
}


// ==========================
// ===== PULSE FUNCTION =====
// ==========================
void pulse(int pin) {
  digitalWrite(pin, HIGH);
  digitalWrite(pin, LOW);
}

// ==========================
// ===== SET ROW ========
// ==========================
void setRow(uint8_t row) {
digitalWrite(ROW_A, row & 0x01);
digitalWrite(ROW_B, (row >> 1) & 0x01);
}

// ==========================
// ===== DISPLAY ROW ========
// ==========================
void displayRowPair(uint8_t row) {
  setRow(row); // Chọn nhóm hàng

  for (int byte = 0; byte < 4; byte++) {
    for (int bit = 7; bit >= 0; bit--) {
      bool pixel_top = (Buffer_display[row][byte] >> bit) & 0x01;
      bool pixel_bottom = (Buffer_display[row + 8][byte] >> bit) & 0x01;

      digitalWrite(R0, pixel_top);
      digitalWrite(G0, pixel_top); // Mặc định màu trắng
      digitalWrite(B0, pixel_top);

      digitalWrite(R1, pixel_bottom);
      digitalWrite(G1, pixel_bottom); // Mặc định màu trắng
      digitalWrite(B1, pixel_bottom);

      pulse(CLK); // Đẩy dữ liệu
    }
  }

  pulse(LAT); // Chốt dữ liệu
  digitalWrite(R0, LOW);
  digitalWrite(G0, LOW);
  digitalWrite(B0, LOW);
  digitalWrite(R1, LOW);
  digitalWrite(G1, LOW);
  digitalWrite(B1, LOW);
  digitalWrite(OE, LOW);
  delayMicroseconds(100);
  digitalWrite(OE, HIGH);
}

// ==========================
// ===== CLEAR DISPLAY =====
// ==========================
void clearDisplay() {
  for (int y = 0; y < 16; y++) {
    for (int x = 0; x < 4; x++) {
      Buffer_display[y][x] = 0;
    }
  }
}
// ==========================
// ===== SET PIXEL ========
// ==========================
void setPixel(int x, int y, bool color) {
  if (x < 0 || x > 31 || y < 0 || y > 15) return;

  int byte_index = x / 8;
  int bit_index = x % 8;
  
  // Nếu wiring bị đảo bit theo cột
  uint8_t bit_mask = (0x80 >> bit_index);  // Bit thông thường
  // uint8_t bit_mask = (1 << bit_index);  // Nếu wiring bị đảo ngược

  if (color) {
    Buffer_display[y][byte_index] |= bit_mask;
  } else {
    Buffer_display[y][byte_index] &= ~bit_mask;
  }


//   if (color) {
//     Buffer_display[y][x / 8] |= (0x80 >> (x % 8)); // Bật bit tại vị trí (x, y)
//   } else {
//     Buffer_display[y][x / 8] &= ~(0x80 >> (x % 8)); // Tắt bit tại vị trí (x, y)
//   }
}
// ==========================
// ===== TEST PIXEL ========
// ==========================
void testPixel(int x, int y) {
  clearDisplay();               // Xóa toàn bộ trước
  setPixel(x, y, true);         // Bật 1 pixel

  unsigned long start = millis();
  while (millis() - start < 1000) {
    for (int row = 0; row < 4; row++) {
      displayRowPair(row);      // Quét cả 16 hàng qua 4 lượt
    }
  }
}
