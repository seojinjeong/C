void setup() {
  // 시리얼 통신 초기화
  Serial.begin(9600);
  while (!Serial) {
    ; // 시리얼 포트가 연결될 때까지 대기
  }
  Serial.println("Arduino is ready");
}

void loop() {
  // 라즈베리 파이로부터 데이터 수신
  if (Serial.available()) {
    String receivedData = Serial.readStringUntil('\n');
    Serial.print("Received from Raspberry Pi: ");
    Serial.println(receivedData);

    // 데이터 응답
    Serial.print("Hello from Arduino!\n");
  }

  delay(100); // 0.1초 대기
}
