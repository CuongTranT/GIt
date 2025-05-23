#include <Arduino.h>
#include <Matrix_Buffer.h>

// ==========================
// ===== MAIN SETUP ========
// ==========================
void setup() {
  setupPins();
  clearDisplay();

  // ======= DEBUG: In nội dung Buffer_display sau khi clear =======
  Serial.begin(115200);
  delay(100);

  Serial.println("Buffer_display content after clearDisplay():");
  for (int y = 0; y < 16; y++) {
    Serial.print("Row ");
    Serial.print(y);
    Serial.print(": ");
    for (int b = 0; b < 4; b++) {
      if (Buffer_display[y][b] < 0b10000000) Serial.print("0"); // in đủ 8 bit
      Serial.print(Buffer_display[y][b], BIN);
      Serial.print(" ");
    }
    Serial.println();
  }

  // ======= Bật pixel tại (0, 0) =======
  setPixel(0, 5, true);  // Bật pixel trái-trên cùng
  Serial.println("Buffer_display sau khi setPixel(0, 0, true):");

  for (int y = 0; y < 16; y++) {
    Serial.print("Row ");
    Serial.print(y);
    Serial.print(": ");
    for (int b = 0; b < 4; b++) {
      if (Buffer_display[y][b] < 0b10000000) Serial.print("0");
      Serial.print(Buffer_display[y][b], BIN);
      Serial.print(" ");
    }
    Serial.println();
  }
}

// ==========================
// ===== MAIN LOOP =========
// ==========================
void loop() {
  // ======= Hiển thị pixel (0,0) liên tục trong 1 giây =======
  unsigned long start = millis();
  while (millis() - start < 1000) {
    for (int row = 0; row < 4; row++) {  // 1/4 scan: 4 dòng quét
      displayRowPair(row);
    }
  }

  delay(1000); // nghỉ 1 giây giữa các vòng lặp để quan sát
}
