#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>
#include "HX711.h"
#include <AccelStepper.h>

// HX711 circuit wiring
#define LOADCELL_DOUT_PIN 16
#define LOADCELL_SCK_PIN 4

// 초음파 센서 핀 정의
#define TRIG_PIN 17
#define ECHO_PIN 5

// 스텝 모터 핀 정의
#define IN1 14
#define IN2 27
#define IN3 26
#define IN4 25

// 초음파 감지 거리 (cm)
const int DISTANCE_THRESHOLD = 10;

// 스텝 모터 객체 생성 (FULL4WIRE 방식)
AccelStepper stepper(AccelStepper::FULL4WIRE, IN1, IN3, IN2, IN4);

// 로드셀
float loadcellValue = 372;
long baseweight;
HX711 scale;

// WiFi 설정
const char* ssid = "MDL";
const char* password = "19650407";

// 웹 서버 객체 생성 (포트 80)
WebServer server(8080);

// 로그인 정보 리스트
const char* usernames[] = {"seojin", "yusang"};
const char* passwords[] = {"seojin", "yusang"};
int points[] = {0, 0}; // 각각의 사용자 포인트
const int num_users = 2; // 유저 수

// 사용자 세션 상태
bool loggedIn = false;
int currentUserIndex = -1; // 현재 로그인한 사용자 인덱스

int servoPin = 14;

int score = 0;

Servo myservo; // 서보 모터 객체

// 로그인 페이지 HTML
const char login_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP32 Login</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body { font-family: Arial, sans-serif; text-align: center; padding: 50px; }
    form { display: inline-block; margin-top: 20px; }
    input[type="text"], input[type="password"] {
      width: 100%; padding: 12px; margin: 8px 0; display: inline-block;
      border: 1px solid #ccc; box-sizing: border-box;
    }
    input[type="submit"] {
      background-color: #4CAF50; color: white; padding: 14px 20px;
      margin: 8px 0; border: none; cursor: pointer; width: 100%;
    }
    input[type="submit"]:hover { background-color: #45a049; }
    h1 { color: #333; }
  </style>
</head>
<body>
  <h1>Login</h1>
  <form action="/login" method="POST">
    <label for="username">Username:</label><br>
    <input type="text" id="username" name="username"><br>
    <label for="password">Password:</label><br>
    <input type="password" id="password" name="password"><br><br>
    <input type="submit" value="Login">
  </form>
</body>
</html>)rawliteral";

// 포인트 페이지 HTML
String points_html() {
  String html = "<!DOCTYPE HTML><html><head><title>ESP32 Points</title>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<style>body { font-family: Arial, sans-serif; text-align: center; padding: 50px; }";
  html += "h1 { color: #333; } p { font-size: 1.2em; }";
  html += "form { display: inline-block; margin-top: 20px; }";
  html += "input[type='submit'] { background-color: #4CAF50; color: white; padding: 14px 20px;";
  html += "margin: 8px 0; border: none; cursor: pointer; width: 100%; }";
  html += "input[type='submit']:hover { background-color: #45a049; }</style>";
  html += "</head><body>";
  html += "<h1>Welcome!</h1>";
  html += "<p>Current Points: " + String(points[currentUserIndex]) + "</p>";
  html += "<form action='/add_point' method='POST'><input type='submit' value='Calculate'></form>";
  html += "<form action='/logout' method='POST'><input type='submit' value='Logout'></form>";
  html += "</body></html>";
  return html;
}

void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // 스텝 모터 초기 설정
  stepper.setMaxSpeed(1000);       // 최대 속도 (스텝/초)
  stepper.setAcceleration(500);   // 가속도 (스텝/초^2)
  stepper.setCurrentPosition(0);  // 현재 위치를 0으로 설정

  Serial.println("Ultrasonic sensor with stepper motor control initialized.");
  
  // 모든 타이머 할당 허용
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);

  myservo.attach(servoPin); // 서보 모터를 핀에 연결
  
  // WiFi 연결 시작
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");

  // WiFi 연결 대기
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }

  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP()); // IP 주소 출력

  // 기본 경로 설정 (로그인 페이지)
  server.on("/", HTTP_GET, []() {
    server.send_P(200, "text/html", login_html);
  });

  // 로그인 핸들러
  server.on("/login", HTTP_POST, []() {
    if (server.hasArg("username") && server.hasArg("password")) {
      String inputUser = server.arg("username");
      String inputPass = server.arg("password");
      bool validLogin = false;
      
      for (int i = 0; i < num_users; i++) {
        if (inputUser == usernames[i] && inputPass == passwords[i]) {
          validLogin = true;
          currentUserIndex = i; // 현재 사용자 인덱스 저장
          break;
        }
      }

      if (validLogin) {
        loggedIn = true;  // 로그인 상태 업데이트
        server.send(200, "text/html", points_html());
        // 서보 모터를 0도에서 90도 사이로 회전 -->> 뚜껑 열림
        for (int angle = 110; angle >= 10; angle--) {
          myservo.write(angle);
          delay(20); // 각도 변경 후 대기 시간
        }
        return;
      }
    }
    server.send(200, "text/html", "<h1>Login Failed</h1><a href='/'>Go back</a>");
  });

  // 포인트 페이지 핸들러
  server.on("/points", HTTP_GET, []() {
    if (loggedIn) {
      server.send(200, "text/html", points_html());
    } else {
      server.send_P(401, "text/html", login_html);
    }
  });

  // 포인트 추가 핸들러
  server.on("/add_point", HTTP_POST, []() {
    if (loggedIn) {  // 로그인 상태 확인
      // 포인트 추가 로직
      long currentWeight = -scale.get_units();
      long weightChange = currentWeight - baseweight;

      baseweight = currentWeight;

      delay(2000);

      if (weightChange > 1) {
        score = weightChange * 0.32;
        Serial.print("Added points: ");
        Serial.println(score);
        points[currentUserIndex] += score;
        score = 0;
      }

      server.send(200, "text/html", points_html());
    } else {
      server.send(401, "text/html", "<h1>Unauthorized</h1>");
    }
  });

  // 로그아웃 핸들러
  server.on("/logout", HTTP_POST, []() {
    loggedIn = false;
    currentUserIndex = -1; // 현재 사용자 인덱스 초기화
    server.send_P(200, "text/html", login_html);

    // 서보 모터를 90도에서 0도 사이로 회전 -->> 뚜껑 닫힘
    for (int angle = 10; angle <= 110; angle++) {
      myservo.write(angle);
      delay(20); // 각도 변경 후 대기 시간
    }
  });

  // 웹 서버 시작
  server.begin();

  // 로드셀 HX711 보드 pin 설정
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  delay(2000);
  scale.set_scale(loadcellValue);
  scale.tare(10);    
  delay(2000);
  baseweight = -scale.get_units();
}

void loop() {
  // 클라이언트 요청 처리
  server.handleClient();

  // 거리 측정
  long distance = measureDistance();
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // 거리 임계값 확인
  if (distance > 0 && distance <= DISTANCE_THRESHOLD) {
    Serial.println("Object detected! Rotating stepper motor...");
    
    // 스텝 모터 90도 회전 (CW)
    stepper.moveTo(512);  // 90도 = 512 스텝 (28BYJ-48 기준)
    while (stepper.distanceToGo() != 0) {
      stepper.run();
    }
    delay(8000);  // 2초 대기

    // 스텝 모터 원래 위치로 복귀 (CCW)
    stepper.moveTo(0);
    while (stepper.distanceToGo() != 0) {
      stepper.run();
    }

    Serial.println("Stepper motor returned to original position.");
  }

  delay(500);  // 0.5초 대기 후 거리 재측정
}

// 초음파 센서를 사용하여 거리 측정
long measureDistance() {
  // 초음파 펄스 전송
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // ECHO 핀의 펄스 길이 측정
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);  // 타임아웃 30ms (약 5m)
  if (duration == 0) {
    return -1;  // 타임아웃 발생 시 -1 반환
  }

  // 거리 계산 (음속: 343 m/s)
  long distance = duration * 0.034 / 2;  // cm 단위 거리
  return distance;
}