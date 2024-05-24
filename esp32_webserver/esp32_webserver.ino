#include <WiFi.h>
#include <WebServer.h>

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

// 로그인 페이지 HTML
const char login_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP32 Login</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
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
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'></head><body>";
  html += "<h1>Welcome!</h1>";
  html += "<p>Current Points: " + String(points) + "</p>";
  html += "<form action='/add_point' method='POST'><input type='submit' value='Add Point'></form>";
  html += "</body></html>";
  return html;
}

void setup() {
  Serial.begin(115200);
  
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
    if (loggedIn==1) {
      server.send(200, "text/html", points_html());
    } else {
      server.send_P(200, "text/html", login_html);
    }
  });

  // 로그인 핸들러
  server.on("/login", HTTP_POST, []() {
    if (server.hasArg("username") && server.hasArg("password")) {
      String inputUser = server.arg("username");
      String inputPass = server.arg("password");
      if (inputUser == username && inputPass == user_password) {
        loggedIn = true;
        server.send(200, "text/html", points_html());
        return;
      }
    }
    server.send(200, "text/html", "<h1>Login Failed</h1><a href='/'>Go back</a>");
  });

  // 포인트 추가 핸들러
  server.on("/add_point", HTTP_POST, []() {
    if (loggedIn) {
      points += 1;
      server.send(200, "text/html", points_html());
    } else {
      server.send(401, "text/html", "<h1>Unauthorized</h1>");
    }
  });

  // 웹 서버 시작
  server.begin();
}

void loop() {
  // 클라이언트 요청 처리
  server.handleClient();
}
