#include <WiFi.h>
#include <WebServer.h>

const char *ssid = "********88";
const char *password = "**********";

WebServer server(80);

// Pinos dos LEDs
const int LED1_PIN = 13;  // D13
const int LED2_PIN = 12;  // D12
const int LED3_PIN = 14;  // D14
const int LED4_PIN = 27;  // D17

// Função para manipular a rota '/'
void handleRoot()
{
  // Responde à solicitação com uma página HTML com botões de ação para cada LED
  String response = R"html(
    <!DOCTYPE html>
    <html lang="en">
    <head>
      <meta charset="UTF-8">
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
      <title>ESP32 LEDs Control</title>
      <style>
        body {
          font-family: Arial, sans-serif;
          text-align: center;
          margin: 50px;
        }
        h1 {
          color: #333;
        }
        .button {
          display: inline-block;
          padding: 10px 20px;
          font-size: 16px;
          margin: 10px;
          cursor: pointer;
          text-align: center;
          text-decoration: none;
          outline: none;
          color: #fff;
          background-color: #4CAF50;
          border: none;
          border-radius: 15px;
          box-shadow: 0 9px #999;
        }
        .button:hover {background-color: #45a049}
        .button:active {
          background-color: #3e8e41;
          box-shadow: 0 5px #666;
          transform: translateY(4px);
        }
      </style>
      <script>
        function toggleLED(ledNumber) {
          // Enviar uma solicitação para o servidor indicando a ação
          var xhr = new XMLHttpRequest();
          xhr.open("GET", "/toggleLED?led=" + ledNumber, true);
          xhr.send();
        }
      </script>
    </head>
    <body>
      <h1>ESP32 LEDs Control</h1>
      <button class="button" onclick="toggleLED(1)">Toggle LED 1</button>
      <button class="button" onclick="toggleLED(2)">Toggle LED 2</button>
      <button class="button" onclick="toggleLED(3)">Toggle LED 3</button>
      <button class="button" onclick="toggleLED(4)">Toggle LED 4</button>
    </body>
    </html>
  )html";

  server.send(200, "text/html", response);
}

// Função para lidar com ação de toggle nos LEDs
void handleLEDsToggle()
{
  // Obter o número do LED a partir dos parâmetros da URL
  int ledNumber = server.arg("led").toInt();

  // Verificar se o número do LED é válido
  if (ledNumber >= 1 && ledNumber <= 4)
  {
    // Alternar o estado do LED correspondente
    int ledPin;
    switch (ledNumber)
    {
      case 1:
        ledPin = LED1_PIN;
        break;
      case 2:
        ledPin = LED2_PIN;
        break;
      case 3:
        ledPin = LED3_PIN;
        break;
      case 4:
        ledPin = LED4_PIN;
        break;
    }
    digitalWrite(ledPin, !digitalRead(ledPin));
  }

  // Redirecionar de volta à página principal após o toggle
  handleRoot();
}

void setup()
{
  Serial.begin(9600);

  // Conectar-se à rede WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Conectando ao WiFi...");
  }
  Serial.println("Conectado ao WiFi.");

  // Configurar pinos dos LEDs como saídas
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  pinMode(LED4_PIN, OUTPUT);

  // Definir o manipulador para a rota '/'
  server.on("/", HTTP_GET, handleRoot);
  // Definir o manipulador para a rota '/toggleLED'
  server.on("/toggleLED", HTTP_GET, handleLEDsToggle);

  Serial.println("Conectado ao WiFi");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());

  // Iniciar o servidor
  server.begin();
  Serial.println("Servidor iniciado");
}

void loop()
{
  // Lidar com as solicitações do cliente
  server.handleClient();
}
