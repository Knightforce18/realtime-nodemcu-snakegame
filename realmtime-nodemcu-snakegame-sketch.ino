#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>

const char* ssid = "MiniProject";
const char* password = "911911911";

const char index_html[] PROGMEM = R"html(
<!DOCTYPE html>
<html>
<head>
  <title>NodeMCU Snake Game</title>
  <style>
    body {
      text-align: center;
      font-family: Arial, sans-serif;
      background-color: #1e3c72;
      color: white;
    }
    canvas {
      border: 3px solid #333;
      background-color: #e0e0e0;
      margin-top: 20px;
    }
    h1 {
      color: #f39c12;
    }
    #score {
      color: #f39c12;
      font-size: 20px;
    }
  </style>
</head>
<body>
  <h1>NodeMCU Snake Game</h1>
  <div id="score">Score: 0 | High Score: 0</div>
  <canvas id="gameCanvas" width="640" height="480"></canvas>
  <script>
    const ws = new WebSocket('ws://192.168.4.1:81');
    const canvas = document.getElementById('gameCanvas');
    const ctx = canvas.getContext('2d');
    const scoreDiv = document.getElementById('score');
    let highScore = 0;
    ws.onmessage = (event) => {
      const gameState = JSON.parse(event.data);
      drawGame(gameState);
      updateScore(gameState.score, gameState.highScore);
    };
    function drawGame(gameState) {
      ctx.clearRect(0, 0, canvas.width, canvas.height);
      ctx.fillStyle = 'green';
      for (let i = 0; i < gameState.snake.length; i++) {
        const segment = gameState.snake[i];
        ctx.fillRect(segment.x * 20, segment.y * 20, 20, 20);
      }
      ctx.fillStyle = 'red';
      ctx.beginPath();
      const foodX = gameState.food.x * 20 + 10;
      const foodY = gameState.food.y * 20 + 10;
      ctx.arc(foodX, foodY, 10, 0, Math.PI * 2);
      ctx.fill();
    }
    function updateScore(score, highScore) {
      scoreDiv.textContent = `Score: ${score} | High Score: ${highScore}`;
    }
  </script>
</body>
</html>
)html";

const char controller_html[] PROGMEM = R"html(
<!DOCTYPE html>
<html>
<head>
  <title>Snake Game Controller</title>
  <style>
    body {
      font-family: 'Arial', sans-serif;
      display: flex;
      justify-content: center;
      align-items: center;
      height: 100vh;
      background: linear-gradient(to bottom, #1e3c72, #2a5298);
      margin: 0;
    }
    .container {
      text-align: center;
      background-color: rgba(255, 255, 255, 0.9);
      border-radius: 15px;
      padding: 40px;
      box-shadow: 0 8px 30px rgba(0, 0, 0, 0.3);
      width: 320px;
    }
    h1 {
      color: #333;
      font-size: 28px;
      margin-bottom: 20px;
    }
    button {
      background-color: #3498db;
      color: white;
      border: none;
      padding: 25px;
      font-size: 24px;
      margin: 10px;
      border-radius: 50%;
      cursor: pointer;
      transition: all 0.3s ease;
      box-shadow: 0 6px 15px rgba(52, 152, 219, 0.3);
    }
    button:hover {
      background-color: #2980b9;
      transform: translateY(-4px);
      box-shadow: 0 8px 20px rgba(52, 152, 219, 0.5);
    }
    button:active {
      transform: translateY(2px);
      box-shadow: 0 3px 10px rgba(52, 152, 219, 0.4);
    }
    .controls {
      display: grid;
      grid-template-columns: repeat(3, 1fr);
      gap: 15px;
      grid-template-rows: 1fr 1fr 1fr;
      margin-top: 20px;
      justify-items: center;
    }
    .controls button {
      width: 80px;
      height: 80px;
    }
    .up {
      grid-column: 2;
      grid-row: 1;
    }
    .down {
      grid-column: 2;
      grid-row: 3;
    }
    .left {
      grid-column: 1;
      grid-row: 2;
    }
    .right {
      grid-column: 3;
      grid-row: 2;
    }
  </style>
</head>
<body>
  <div class="container">
    <h1>Snake Game Controller</h1>
    <div class="controls">
      <button class="up" onclick="sendCommand('up')">&#8593;</button>
      <button class="left" onclick="sendCommand('left')">&#8592;</button>
      <button class="right" onclick="sendCommand('right')">&#8594;</button>
      <button class="down" onclick="sendCommand('down')">&#8595;</button>
    </div>
  </div>
  <script>
    const ws = new WebSocket('ws://192.168.4.1:81');
    function sendCommand(command) {
      ws.send(command);
    }
  </script>
</body>
</html>
)html";

ESP8266WebServer server(80);
WebSocketsServer webSocket(81);

struct SnakeSegment {
  int x, y;
};

SnakeSegment snake[100];
int snakeLength = 1;
int snakeX, snakeY;
int foodX, foodY;
int direction = 1;
int score = 0;
int highScore = 0;

const int ledPin = LED_BUILTIN;

void setup() {
  Serial.begin(115200);
  WiFi.softAP(ssid, password);
  pinMode(ledPin, OUTPUT);
  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html", index_html);
  });
  server.on("/controller", HTTP_GET, []() {
    server.send(200, "text/html", controller_html);
  });
  server.begin();
  webSocket.begin();
  webSocket.onEvent(onWebSocketEvent);
  snakeX = 10;
  snakeY = 10;
  generateFood();
}

void loop() {
  server.handleClient();
  webSocket.loop();
  unsigned long currentMillis = millis();
  static unsigned long previousMillis = 0;
  const long interval = 16;
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    updateGame();
    sendGameState();
  }
  blinkLED();
}

void updateGame() {
  static unsigned long lastMoveTime = 0;
  unsigned long currentMillis = millis();
  if (currentMillis - lastMoveTime >= 400) {
    lastMoveTime = currentMillis;
    for (int i = snakeLength - 1; i > 0; i--) {
      snake[i].x = snake[i - 1].x;
      snake[i].y = snake[i - 1].y;
    }
    switch (direction) {
      case 1:
        snake[0].x++;
        break;
      case 2:
        snake[0].y++;
        break;
      case 3:
        snake[0].x--;
        break;
      case 4:
        snake[0].y--;
        break;
    }
    if (snake[0].x == foodX && snake[0].y == foodY) {
      snakeLength++;
      score++;
      if (score > highScore) {
        highScore = score;
      }
      generateFood();
    }
    for (int i = 1; i < snakeLength; i++) {
      if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
        resetGame();
      }
    }
    if (snake[0].x < 0 || snake[0].x >= 32 || snake[0].y < 0 || snake[0].y >= 24) {
      resetGame();
    }
  }
}

void generateFood() {
  foodX = random(0, 32);
  foodY = random(0, 24);
}

void resetGame() {
  score = 0;
  snakeLength = 1;
  snake[0].x = 10;
  snake[0].y = 10;
  generateFood();
}

void sendGameState() {
  String gameState = "{\"snake\":[";
  for (int i = 0; i < snakeLength; i++) {
    gameState += "{\"x\":" + String(snake[i].x) + ",\"y\":" + String(snake[i].y) + "}";
    if (i < snakeLength - 1) {
      gameState += ",";
    }
  }
  gameState += "],\"food\":{\"x\":" + String(foodX) + ",\"y\":" + String(foodY) + "},";
  gameState += "\"score\":" + String(score) + ",";
  gameState += "\"highScore\":" + String(highScore) + "}";
  webSocket.broadcastTXT(gameState);
}

void blinkLED() {
  static unsigned long lastBlinkTime = 0;
  static bool ledState = LOW;
  unsigned long currentMillis = millis();
  if (currentMillis - lastBlinkTime >= 1000) {
    ledState = !ledState;
    digitalWrite(ledPin, ledState);
    lastBlinkTime = currentMillis;
  }
}

void onWebSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
  if (type == WStype_TEXT) {
    String command = String((char*)payload);
    if (command == "up") {
      direction = 4;
    } else if (command == "down") {
      direction = 2;
    } else if (command == "left") {
      direction = 3;
    } else if (command == "right") {
      direction = 1;
    }
  }
}
