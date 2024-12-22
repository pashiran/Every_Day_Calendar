#include <Wire.h>
#include <Adafruit_MCP23017.h>

Adafruit_MCP23017 mcp1;  // MCP23017 인스턴스 1
Adafruit_MCP23017 mcp2;  // MCP23017 인스턴스 2
Adafruit_MCP23017 mcp3;  // MCP23017 인스턴스 3

void setup() {
  Serial.begin(9600);
  
  // MCP23017 초기화
  mcp1.begin(0); // I2C 주소 0x20
  mcp2.begin(1); // I2C 주소 0x21
  mcp3.begin(2); // I2C 주소 0x22

  // 행을 출력으로 설정 (MCP1의 0~15 핀)
  for (int i = 0; i < 16; i++) {
    mcp1.pinMode(i, OUTPUT);
    mcp1.digitalWrite(i, HIGH);
  }
  
  // MCP2의 핀 0~2를 출력으로 설정 (나머지 3개 행)
  for (int i = 0; i < 3; i++) {
    mcp2.pinMode(i, OUTPUT);
    mcp2.digitalWrite(i, HIGH);
  }
  
  // 열을 입력으로 설정
  for (int i = 3; i < 16; i++) {  // MCP2의 3~15 핀
    mcp2.pinMode(i, INPUT);
    mcp2.pullUp(i, HIGH);
  }
  for (int i = 0; i < 7; i++) {   // MCP3의 0~6 핀
    mcp3.pinMode(i, INPUT);
    mcp3.pullUp(i, HIGH);
  }
}

void loop() {
  scanMatrix();
  delay(50); // 스캔 주기를 조정
}

void scanMatrix() {
  // MCP1 (행의 첫 16개)
  for (int row = 0; row < 16; row++) {
    mcp1.digitalWrite(row, LOW);  // 현재 행을 LOW로 설정
    for (int col = 3; col < 16; col++) {  // MCP2의 3~15 핀
      if (mcp2.digitalRead(col) == LOW) {  // 키가 눌렸는지 확인
        Serial.print("Key pressed at Row ");
        Serial.print(row);
        Serial.print(", Col ");
        Serial.println(col - 3);
      }
    }
    for (int col = 0; col < 7; col++) {  // MCP3의 0~6 핀
      if (mcp3.digitalRead(col) == LOW) {
        Serial.print("Key pressed at Row ");
        Serial.print(row);
        Serial.print(", Col ");
        Serial.println(col + 13);
      }
    }
    mcp1.digitalWrite(row, HIGH);  // 현재 행을 다시 HIGH로 설정
  }

  // MCP2의 나머지 3개 행
  for (int row = 0; row < 3; row++) {
    mcp2.digitalWrite(row, LOW);  // 현재 행을 LOW로 설정
    for (int col = 3; col < 16; col++) {  // MCP2의 3~15 핀
      if (mcp2.digitalRead(col) == LOW) {
        Serial.print("Key pressed at Row ");
        Serial.print(row + 16);
        Serial.print(", Col ");
        Serial.println(col - 3);
      }
    }
    for (int col = 0; col < 7; col++) {  // MCP3의 0~6 핀
      if (mcp3.digitalRead(col) == LOW) {
        Serial.print("Key pressed at Row ");
        Serial.print(row + 16);
        Serial.print(", Col ");
        Serial.println(col + 13);
      }
    }
    mcp2.digitalWrite(row, HIGH);  // 현재 행을 다시 HIGH로 설정
  }
}
