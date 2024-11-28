#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>
#include "HX711.h"

// HX711 circuit wiring
#define LOADCELL_DOUT_PIN 16
#define LOADCELL_SCK_PIN 4

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
  
  // WiFi 연결 상태 점검 및 재연결
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi disconnected, attempting to reconnect...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(100);
      Serial.println("Reconnecting...");
    }
    Serial.println("Reconnected to WiFi");
  }
}