#include <Arduino.h>
#include <WiFi.h>

//Definir senha e nome de rede Wifi Local
const char* NOME_WIFI = "rede-wifi"; //Digite entre aspas o nome da sua rede.
const char* SENHA_WIFI = "senha-wifi";  //Digite entre  aspas a senha da sua rede.

WiFiServer server(80);

String header;

//Auxiliar as variaveis
String ESTADO_SAIDA_1 = "OFF"; //Relé 1  
String ESTADO_SAIDA_2 = "OFF"; //Relé 2
String ESTADO_SAIDA_3 = "OFF"; //Relé 3
String ESTADO_SAIDA_4 = "OFF"; //Relé 4

//Definir pinos GPIO
const int RELE_1 = 18; //relé 1
const int RELE_2 = 19; //relé 2
const int RELE_3 = 23; //relé 3
const int RELE_4 = 05; //relé 4
const int LED_WIFI = 26; //Led indicador de rede.

void setup() {
  Serial.begin(9600);

  //Definir as variaveis como saída
  pinMode(RELE_1, OUTPUT); //Define o pino do relé 1 como saída.
  pinMode(RELE_2, OUTPUT); //Define o pino do relé 2 como saída.
  pinMode(RELE_3, OUTPUT); //Define o pino do relé 3 como saída.
  pinMode(RELE_4, OUTPUT); //Define o pino do relé 3 como saída.
  pinMode(LED_WIFI, OUTPUT); //Definir o pino do led como saída. 

  //Iniciar em HIGH, ou seja, Deslidados por ser módulos relés
  digitalWrite(RELE_1, HIGH);
  digitalWrite(RELE_2, HIGH);
  digitalWrite(RELE_3, HIGH);
  digitalWrite(RELE_4, HIGH);

  //definir led indicador de rede inicialmente deslidado
  digitalWrite(LED_WIFI, LOW);

  //Conectar a rede Wifi fornecida inicialmente
  Serial.print("Conectando a ");
  Serial.println(NOME_WIFI);
  WiFi.begin(NOME_WIFI, SENHA_WIFI);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    digitalWrite(LED_WIFI, HIGH);
    delay(500);
    digitalWrite(LED_WIFI, LOW);

  }  
  if(WiFi.status() == WL_CONNECTED){
      digitalWrite(LED_WIFI, HIGH);
      }
  
  //Imprimir o endereço IP da Rede Local na qual vai ser utilizado para conectar no navegador web. 
  Serial.println("");
  Serial.println("WiFi Conectado.");
  Serial.println("Endereco IP: ");
  Serial.println(WiFi.localIP());
  server.begin();
}
void loop(){
  WiFiClient client = server.available();
  if(client){
    Serial.println("Novo Cliente.");
    String currentLine = "";
    while (client.connected()) {
      if(client.available()) {
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
            //relé 1
            if (header.indexOf("GET /rele1/on") >= 0) {
              Serial.println("Relé 1 Ligado");
              ESTADO_SAIDA_1 = "ON";
              digitalWrite(RELE_1, LOW);
            }else if (header.indexOf("GET /rele1/off") >= 0) {
              Serial.println("Relé 1 Desligado");
              ESTADO_SAIDA_1 = "OFF";
              digitalWrite(RELE_1, HIGH);
            }

            //relé 2
            else if (header.indexOf("GET /rele2/on") >= 0) {
              Serial.println("Relé 2 Ligado");
              ESTADO_SAIDA_2 = "ON";
              digitalWrite(RELE_2, LOW);
            }else if (header.indexOf("GET /rele2/off") >= 0) {
              Serial.println("Relé 2 Desligado");
              ESTADO_SAIDA_2 = "OFF";
              digitalWrite(RELE_2, HIGH);
            }

            //relé 3 
            else if (header.indexOf("GET /rele3/on") >= 0) {
              Serial.println("Relé 3 Ligado");
              ESTADO_SAIDA_3 = "ON";
              digitalWrite(RELE_3, LOW);
            }else if (header.indexOf("GET /rele3/off") >= 0) {
              Serial.println("Relé 3 Desligado");
              ESTADO_SAIDA_3 = "OFF";
              digitalWrite(RELE_3, HIGH);
            }

            //relé 4
            else if (header.indexOf("GET /rele4/on") >= 0) {
              Serial.println("Relé 4 Ligado");
              ESTADO_SAIDA_4 = "ON";
              digitalWrite(RELE_4, LOW);
            }else if (header.indexOf("GET /rele4/off") >= 0) {
              Serial.println("Relé 4 Desligado");
              ESTADO_SAIDA_4 = "OFF";
              digitalWrite(RELE_4, HIGH);
            }

            //Desligar Todos
            else if (header.indexOf("GET /off-all") >= 0) {
              Serial.println("Todos os relés desligados");
              ESTADO_SAIDA_1 = "OFF";
              ESTADO_SAIDA_2 = "OFF";
              ESTADO_SAIDA_3 = "OFF";
              ESTADO_SAIDA_4 = "OFF";

              digitalWrite(RELE_1, HIGH);
              digitalWrite(RELE_2, HIGH);
              digitalWrite(RELE_3, HIGH);
              digitalWrite(RELE_4, HIGH);
            }

            //Interface pagina web Html
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #1E90FF; border-radius: 20px; color: white; padding: 4px 100px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            client.println("<body><h1>App Web &#128241;</h1>");

            //  Mostrar na tela o estado do relé 1 entre Ligado e Deslidado
            client.println("<h4>RELE 1 - " + ESTADO_SAIDA_1 + "</h4>");
            if (ESTADO_SAIDA_1 == "OFF") {
              client.println("<p><a href=\"/rele1/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/rele1/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            // Mostrar na tela o estado do relé 2 entre Ligado e Deslidado
            client.println("<h4>RELE 2 - " + ESTADO_SAIDA_2 + "</h4>");
            if (ESTADO_SAIDA_2 == "OFF") {
              client.println("<p><a href=\"/rele2/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/rele2/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            //  Mostrar na tela o estado do relé 3 entre Ligado e Deslidado
            client.println("<h4>RELE 3 -" + ESTADO_SAIDA_3 + "</h4>");
            if (ESTADO_SAIDA_3 == "OFF") {
              client.println("<p><a href=\"/rele3/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/rele3/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            // Mostrar na tela o estado do relé 4 entre Ligado e Deslidado
            client.println("<h4>RELE 4 -" + ESTADO_SAIDA_4 + "</h4>");
            if (ESTADO_SAIDA_4 == "OFF") {
              client.println("<p><a href=\"/rele4/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/rele4/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            //Botão desligar todos
            client.println("<h4>DESLIGAR  TODOS<h4>");
            client.println("<p><a href=\"/off-all\"><button class=\"button\">ALL</button></a></p>");

             //Fim Html
            client.println("</body></html>");
            client.println();
            break;
          }else {
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