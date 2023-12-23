#include <WiFi.h>
#include <WebServer.h>

const char *ssid = "*********";
const char *password = "********";

WebServer server(80);

// Função para manipular a rota '/'
void handleRoot()
{
  // Responde à solicitação com uma página HTML simples
  server.send(200, "text/html", "<h1>Hello, ESP32!</h1>");
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
  Serial.println("Conectado ao WiFi");

  // Definir o manipulador para a rota '/'
  server.on("/", HTTP_GET, handleRoot);

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
