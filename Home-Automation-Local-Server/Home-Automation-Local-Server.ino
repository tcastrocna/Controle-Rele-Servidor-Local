#include <Arduino.h>
#include <WiFi.h>
#include <WiFiManager.h>

//Definir os pinos de saídas e entrada da placa.
#define RELAY_PIN_1             18       //Relé Iluminação 1.
#define RELAY_PIN_2             19       //Relé Iluminação 2.
#define RELAY_PIN_3             23       //Relé Ventilador.
#define BUTTON_RESET_WIFI       17       //Botão Reset rede WiFi.
#define WIFI_LED                25       //Led indicador de rede.

#define BAUD_RATE  9600

//Variaveis de Auxilio no app Web
String TEXT_STATE_1 = "OFF";
String TEXT_STATE_2 = "OFF";
String TEXT_STATE_3 = "OFF";

WiFiServer server(80);
String header;

void appWebCliente() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("Novo Cliente.");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        header += c;
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            //Controlar as entradas em Ligado e Desligado
            //Iluminação 1
            if (header.indexOf("GET /iluminacao1/on") >= 0) {
              Serial.println("Iluminação 1 Ligado");
              TEXT_STATE_1 = "ON";
              digitalWrite(RELAY_PIN_1, LOW);
            } else if (header.indexOf("GET /iluminacao1/off") >= 0) {
              Serial.println("Iluminação 1 Desligado");
              TEXT_STATE_1 = "OFF";
              digitalWrite(RELAY_PIN_1, HIGH);
            }

            //Iluminação 2
            else if (header.indexOf("GET /iluminacao2/on") >= 0) {
              Serial.println("Iluminação 2 Ligado");
              TEXT_STATE_2 = "ON";
              digitalWrite(RELAY_PIN_2, LOW);
            } else if (header.indexOf("GET /iluminacao2/off") >= 0) {
              Serial.println("Iluminação 2 Desligado");
              TEXT_STATE_2 = "OFF";
              digitalWrite(RELAY_PIN_2, HIGH);
            }

            //Ventilador
            else if (header.indexOf("GET /ventilador/on") >= 0) {
              Serial.println("Ventilador Ligado");
              TEXT_STATE_3 = "ON";
              digitalWrite(RELAY_PIN_3, LOW);
            } else if (header.indexOf("GET /ventilador/off") >= 0) {
              Serial.println("Ventilador Desligado");
              TEXT_STATE_3 = "OFF";
              digitalWrite(RELAY_PIN_3, HIGH);
            }

            //Desligar Todos
            else if (header.indexOf("GET /off-all") >= 0) {
              Serial.println("Todos os equipamentos desligados");
              TEXT_STATE_1 = "OFF";
              TEXT_STATE_2 = "OFF";
              TEXT_STATE_3 = "OFF";

              digitalWrite(RELAY_PIN_1, HIGH);
              digitalWrite(RELAY_PIN_2, HIGH);
              digitalWrite(RELAY_PIN_3, HIGH);
            }

            //Interface pagina web Html
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #00BFFF; border: none; color: white; padding: 10px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            client.println("<body><h1>Home Automation</h1>");

            //Iluminação 1
            client.println("<h4>ILUMINACAO 1</h4>");
            if (TEXT_STATE_1 == "OFF") {
              client.println("<p><a href=\"/iluminacao1/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/iluminacao1/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            //Iluminação 2
            client.println("<h4>ILUMINACAO 2</h4>");
            if (TEXT_STATE_2 == "OFF") {
              client.println("<p><a href=\"/iluminacao2/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/iluminacao2/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            //Ventilador
            client.println("<h4>VENTILADOR</h4>");
            if (TEXT_STATE_3 == "OFF") {
              client.println("<p><a href=\"/ventilador/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/ventilador/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            client.println("<h4>DESLIGAR  TODOS<h4>");
            client.println("<p><a href=\"/off-all\"><button class=\"button\">ALL</button></a></p>");

            //Fim Html
            client.println("</body></html>");
            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    header = "";

    //Fechar Conexão
    client.stop();
    Serial.println("Cliente Desconectado");
    Serial.println("");
  }

}

//Configuração principal
void setup() {
  Serial.begin(BAUD_RATE);
  wifiConfigESP();

  Serial.println("");
  Serial.println("WiFi Conectado.");
  Serial.println("Endereco IP: ");
  Serial.println(WiFi.localIP());
  server.begin();

  pinMode(RELAY_PIN_1, OUTPUT);
  pinMode(RELAY_PIN_2, OUTPUT);
  pinMode(RELAY_PIN_3, OUTPUT);
  pinMode(WIFI_LED, OUTPUT);

  pinMode(BUTTON_RESET_WIFI, INPUT_PULLUP);

  //Durante a partida todos os relés devem DESLIGAR
  digitalWrite(RELAY_PIN_1, HIGH);
  digitalWrite(RELAY_PIN_2, HIGH);
  digitalWrite(RELAY_PIN_3, HIGH);
  digitalWrite(WIFI_LED, LOW);

}

//Loop Principal
void loop() {
  mudarRede();
  appWebCliente();
}
