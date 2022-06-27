# controleReleServerLocal
Este é um projeto simples que permite o controle via app web de equipamentos eletrônicos com módulo relé e chip esp32 através de um servidor local. 
Com este código é possivel ligar e desligar relés com um simples app web no celular ou desktop, e para isso é preciso ter uma rede wifi (Sem necessariamente ter acesso a intenet).

Para este projeto utilizei a plataforma PlatfotmIO no [Vs Code](https://code.visualstudio.com/), sendo assim é necessario instalar a plataforma [PlatformIO](https://platformio.org/platformio-ide) e clonar esse projeto, após baixar em seu computador abra diretamentamente pelo Vs Code ou outra ide que suporte a plataforma.
Caso utilize Arduino IDE basta copiar o codigo do arquivo [main.cpp](https://github.com/tiagocnha/controleReleServerLocal/blob/main/src/main.cpp) da pasta src na sua ide e altere o código de acordo com a sua necessidade.

#### Altere os dados necessários
Algumas parte do código devem ser modificadas de acordo com a sua necessidade.

Nas linhas abaixo mude ``NOME_WIFI`` e ``SENHA_WIFI`` pelo mesmo de seu roteador, obedecendo letras, símbolos e números, caso esteja diferente a placa não se conectará a rede.
````
const char* ssid = "NOME_WIFI";
const char* password = "SENHA_WIFI";
````

Nesta parte do código são as variáveis de auxílio de texto, que por padrão está em "OFF", este é o texto que vai aparecer no seu app indicando que os relés estão desligado, ao clicar nos botões esse "OFF" irá mudar para "ON". Essas variáveis são nomeadas de ``estadosaida1``(Rele1), ``estadosaida2``(Rele2), ``estadosaida3``(Rele3) e ``estadosaidaN``(ReleN), neste caso utilizei apenas três relés, então caso tenha necessidade de aumentar o número de relés adicione mais obedecendo a numeração ``estadosaidaN``.
````
String estadosaida1 = "OFF"; //Rele1
String estadosaida2 = "OFF"; //Rele2
String estadosaida3 = "OFF"; //Rele3
````

Nesta parte vamos definir as variáveis de saída, que são nomeadas com um mone e em seguida do simbolo de `=` temos o numero da GPIO da placa que é a numeração dos pinos de saída, consulte as especificações da placa e mude de acordo, neste caso são  ``saida1``(Rele1), ``saida2``(Rele2) e ``saida3``(Rele3), seguindo sempre a ideia inicial, caso seja necessário adicionar mais relés, adicione obedecendo a numeração ``saidaN``(ReleN). Como complemento, mas opcional, foi adicionado um led na saída 16 para indicar que a placa está conectada a rede. 
````
const int saida1 = 18; //Rele1
const int saida2 = 19; //Rele2
const int saida3 = 23; //Rele3
const int ledIndicador = 16; //Led indicador de rede, deve acender ao conectar a rede.
````

Anteriomente definimos os pinos de saída, agora vamos definir como pinos de saída ``OUTPUT``, isso quer dizer que todos os pinos aqui funcionarão como saída, segue a ideia anterior, declare todos que forem adicionados. 
````
  pinMode (saida1, OUTPUT); //Pino 18 do esp32 Rele1
  pinMode (saida2, OUTPUT); //Pino 19 do esp32 Rele2
  pinMode (saida3, OUTPUT); //Pino 23 do esp32 Rele3
  pinMode(ledIndicador, OUTPUT); //Led indicador da placa, acender ao conectar a rede.
````

Já nesta parte define se as saídas vão iniciar ligados ou desligados, por se tratar de módulo relé deve iniciar ligados ``HIGH``, já o led deve iniciar desligado ``LOW``.
````
  digitalWrite(saida1, HIGH);
  digitalWrite(saida2, HIGH);
  digitalWrite(saida3, HIGH);
  digitalWrite(ledIndicador, LOW);
 ````
 
 Nesta parte requer pouco de ateção, pois aqui é a parte de comandos de liga/desliga e alteração de estados de texto de "OFF" para "ON", ou seja ao clicar no botão RELE1 será enviado um comando para ligar o relé e ao mesmo tempo muda a variável ``stadosaida`` declarada no incio do código como "OFF" passa a ser "ON", ao clicar no mesmo botão esse processo se inverte. na parte ``if(header.indexOf("GET /18/on") >=0)`` e ``else if (header.indexOf("GET /18/off")>=0)`` mude o `18` pelo numero da sua GPIO, repita isso para cada relé.
 ````
 //Controlar as entradas em Ligado e Desligado
            //Rele1
            if (header.indexOf("GET /18/on") >= 0) {
              Serial.println("Rele1 Ligada");
              estadosaida1 = "ON";
              digitalWrite(saida1, LOW);
            }else if (header.indexOf("GET /18/off") >= 0) {
              Serial.println("Rele1 Desligada");
              estadosaida1 = "OFF";
              digitalWrite(saida1, HIGH);
  ````
  



# Controle de relés no servidor local
Fazendo o controle de relés através de um servidor local utilizando o o chip Esp32 e app web.

![App-Web](App-Web.png)
