#include <HX711.h>

// HX711 핀 정의
#define LOADCELL_DOUT_PIN 16  // 데이터 핀 (DT)
#define LOADCELL_SCK_PIN 4    // 클록 핀 (SCK)

// HX711 객체 생성
HX711 scale;

// 임계값 (1kg)
const float THRESHOLD = 1.0;  // 단위: kg
const float CALIBRATION_FACTOR = -7050;  // HX711 교정 값 (조정 필요)

// LED 핀 정의
#define LED_PIN 2  // GPIO 2 (내장 LED)

void setup() {
  Serial.begin(115200);
  Serial.println("Starting Load Cell Test...");

  // 로드셀 핀 설정
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  // 교정 값 설정
  scale.set_scale(CALIBRATION_FACTOR);

  // 초기 영점 설정
  Serial.println("Taring... Please ensure the load cell is empty.");
  scale.tare();  // 현재 로드셀 값을 0으로 설정
  Serial.println("Tare complete! Ready to measure weight.");

  // LED 핀 모드 설정
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  // 무게 측정
  float weight = scale.get_units();  // 단위: kg

  // 무게 출력
  Serial.print("Weight: ");
  Serial.print(weight, 2);  // 소수점 두 자리까지 출력
  Serial.println(" kg");

  // 임계값 비교
  if (weight >= THRESHOLD) {
    Serial.println("Threshold exceeded!");
    digitalWrite(LED_PIN, HIGH);  // LED 켜기
  } else {
    digitalWrite(LED_PIN, LOW);   // LED 끄기
  }

  // 사용자 입력에 따라 영점 재설정
  if (Serial.available()) {
    char command = Serial.read();
    if (command == 't') {  // 't' 입력 시 영점 설정
      Serial.println("Re-taring... Please ensure the load cell is empty.");
      scale.tare();
      Serial.println("Tare complete!");
    }
  }

  delay(500);  // 0.5초 대기
}
