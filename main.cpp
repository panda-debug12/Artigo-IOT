#include <WiFi.h>
#include <ESPAsyncWebServer.h>

// Defina o SSID e senha do seu Wi-Fi
const char* ssid = "SEU_SSID";
const char* password = "SUA_SENHA";

// Defina o pino do relé
const int relePin = 23;

// Criação de um servidor assíncrono na porta 80
AsyncWebServer server(80);

void setup() {
  // Inicializa a comunicação serial
  Serial.begin(115200);

  // Configura o pino do relé como saída
  pinMode(relePin, OUTPUT);
  digitalWrite(relePin, LOW);  // Inicialmente a lâmpada está apagada

  // Conecta-se ao Wi-Fi
  WiFi.begin(ssid, password);

  // Espera pela conexão Wi-Fi
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando-se ao WiFi...");
  }
  Serial.println("Conectado ao WiFi!");

  // Imprime o endereço IP do ESP32
  Serial.println(WiFi.localIP());

  // Serve a página web
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    String html = "<html><body><h1>Controle de Iluminação</h1>";
    html += "<button onclick=\"window.location.href='/on'\">Ligar</button><br><br>";
    html += "<button onclick=\"window.location.href='/off'\">Desligar</button><br><br>";
    html += "<button onclick=\"window.location.href='/schedule'\">Agendar</button><br><br>";
    html += "</body></html>";
    request->send(200, "text/html", html);
  });

  // Rota para ligar a lâmpada
  server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(relePin, HIGH);  // Liga a lâmpada
    request->send(200, "text/html", "Lâmpada ligada! <a href='/'>Voltar</a>");
  });

  // Rota para desligar a lâmpada
  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(relePin, LOW);  // Desliga a lâmpada
    request->send(200, "text/html", "Lâmpada desligada! <a href='/'>Voltar</a>");
  });

  // Rota para agendar
  server.on("/schedule", HTTP_GET, [](AsyncWebServerRequest *request){
    String html = "<html><body><h1>Agendar Lâmpada</h1>";
    html += "<form action='/schedule_time' method='get'>";
    html += "Hora (HH:MM): <input type='text' name='time'><br><br>";
    html += "<input type='submit' value='Agendar'>";
    html += "</form></body></html>";
    request->send(200, "text/html", html);
  });

  // Processamento do agendamento
  server.on("/schedule_time", HTTP_GET, [](AsyncWebServerRequest *request){
    String time = request->getParam("time")->value();
    // Aqui você implementaria a lógica para agendar a lâmpada
    // Exemplo: parse da hora e implementação de agendamento
    request->send(200, "text/html", "Agendamento feito para: " + time + " <a href='/'>Voltar</a>");
  });

  // Inicia o servidor
  server.begin();
}

void loop() {
  // O loop é vazio, pois o servidor é assíncrono
}