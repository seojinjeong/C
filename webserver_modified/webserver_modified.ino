#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>

// WiFi 설정
const char* ssid = "yegrina";
const char* password = "9999999999";

// 웹 서버 객체 생성 (포트 80)
WebServer server(80);

// 로그인 정보
const char* username = "admin";
const char* user_password = "admin";

// 사용자 세션 상태
bool loggedIn = false;
int points = 0;

// 초음파 센서 핀 설정
const int trigPin = 5;
const int echoPin = 18;
long duration;
int distance;
int servoPin = 14;

Servo myservo;     // 서보 모터 객체

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
  html += "<p>Current Points: " + String(points) + "</p>";
  html += "<form action='/add_point' method='POST'><input type='submit' value='Add Point'></form>";
  html += "<form action='/logout' method='POST'><input type='submit' value='Logout'></form>";
  html += "</body></html>";
  return html;
}

void setup() {
  Serial.begin(115200);
  
  // 초음파 센서 핀 모드 설정
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
   
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
      if (inputUser == username && inputPass == user_password) {
        loggedIn = true;  // 로그인 상태 업데이트
        server.send(200, "text/html", points_html());
        // 서보 모터를 0도에서 90도 사이로 회wjs -->> 뚜껑 열림
        for (int angle = 0; angle <= 90; angle++) {
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
      points += 1;
      server.send(200, "text/html", points_html());
    } else {
      server.send(401, "text/html", "<h1>Unauthorized</h1>");
    }
  });

  // 로그아웃 핸들러
  server.on("/logout", HTTP_POST, []() {
    loggedIn = false;
    server.send_P(200, "text/html", login_html);

    // 서보 모터를 90도에서 0도 사이로 회전 -->> 뚜껑 닫힘
    for (int angle = 90; angle >= 0; angle--) {
      myservo.write(angle);
      delay(20); // 각도 변경 후 대기 시간
    }
  });

  // 웹 서버 시작
  server.begin();
}

void loop() {
  // 클라이언트 요청 처리
  server.handleClient();
  
  // 초음파 센서 거리 측정
  if (loggedIn) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = duration * 0.034 / 2;

    // 거리 확인 및 포인트 추가
    // if (distance <= 10) {
    //   points += 1;
    //   delay(1000);  // 1초 딜레이 후 다시 측정
    // }
  }
  
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