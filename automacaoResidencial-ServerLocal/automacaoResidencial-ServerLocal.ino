#include <Arduino.h>
#include <WiFi.h>

//Definir senha e nome de rede Wifi Local
const char* ssid = "NOME_WIFI"; //Mude NOME_WIFI pelo nome da sua rede.
const char* password = "SENHA_WIFI";  //Mude SENHA_WIFI pela senha da sua rede.

WiFiServer server(80);

String header;

//Auxiliar as variaveis
String estadosaida18 = "OFF"; //Iluminação 1  
String estadosaida19 = "OFF"; //iluminação 2
String estadosaida23 = "OFF"; //Ventilador

//Definir pinos GPIO
const int saida18 = 18; //Iluminação 1
const int saida19 = 19; //Iluminação 2
const int saida23 = 23; //Ventilador
const int ledIndicador = 16; //Led indicador de rede, deve acender ao conectar a rede.

void setup() {
  Serial.begin(9600);

  //Definir as variaveis como saída
  pinMode (saida18, OUTPUT); //Saída 18 do esp32 Iluminação 1
  pinMode (saida19, OUTPUT); //Saída 29 do esp32 Iluminação2
  pinMode (saida23, OUTPUT); //Saída 23 do esp32 Tomadas
  pinMode(ledIndicador, OUTPUT); //Led indicador da placa, acender ao conectar a rede. 



  //Iniciar em HIGH, ou seja, Deslidados por ser módulos relés
  digitalWrite(saida18, HIGH);
  digitalWrite(saida19, HIGH);
  digitalWrite(saida23, HIGH);
  //definir led indicador de rede inicialmente deslidado
  digitalWrite(ledIndicador, LOW);

  //Conectar a rede Wifi fornecida inicialmente
  Serial.print("Conectando a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  //Acender o led indicador da placa ao conectar a Internet
  if(WiFi.status() == WL_CONNECTED){
      digitalWrite(ledIndicador, HIGH);
      }else{
        digitalWrite(ledIndicador, LOW);
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
            //Iluminação
            if (header.indexOf("GET /18/on") >= 0) {
              Serial.println("Iluminação 1 Ligada");
              estadosaida18 = "ON";
              digitalWrite(saida18, LOW);
            }else if (header.indexOf("GET /18/off") >= 0) {
              Serial.println("Iluminação 1 Desligada");
              estadosaida18 = "OFF";
              digitalWrite(saida18, HIGH);

            //Iluminação 2
            }else if (header.indexOf("GET /19/on") >= 0) {
              Serial.println("Iluminação 2 Ligado");
              estadosaida19 = "ON";
              digitalWrite(saida19, LOW);
            }else if (header.indexOf("GET /19/off") >= 0) {
              Serial.println("Iluminação 2 Desligado");
              estadosaida19 = "OFF";
              digitalWrite(saida19, HIGH);

            //Ventilador 
            }else if (header.indexOf("GET /23/on") >= 0) {
              Serial.println("Ventilador Ligado");
              estadosaida23 = "ON";
              digitalWrite(saida23, LOW);
            }else if (header.indexOf("GET /23/off") >= 0) {
              Serial.println("Ventilador Desligado");
              estadosaida23 = "OFF";
              digitalWrite(saida23, HIGH);
           
            }

            //Interface pagina web Html
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #1E90FF; border-radius: 20px; color: white; padding: 4px 100px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            client.println("<body><h1>TCASTROCMA SH</h1>");

            // Mostrar na tela o estado da saida 18 entre Ligado e Deslidado (Iluminação 1)
            client.println("<h3>ILUMINACAO 1 -" + estadosaida18 + "</h3>");
            if (estadosaida18=="OFF") {
              client.println("<p><a href=\"/18/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/18/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            // Mostrar na tela o estado da saida 19 entre Ligado e Deslidado (Iluminação 2)
            client.println("<h3>ILUMINAÇÃO 2 -" + estadosaida19 + "</h3>");
            if (estadosaida19=="OFF") {
              client.println("<p><a href=\"/19/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/19/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            // Mostrar na tela o estado da saida 23 entre Ligado e Deslidado (Ventilador)
            client.println("<h3>TOMADAS -" + estadosaida23 + "</h3>");
            if (estadosaida23=="OFF") {
              client.println("<p><a href=\"/23/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/23/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
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