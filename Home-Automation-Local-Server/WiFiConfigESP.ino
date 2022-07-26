//Configurações da rede Wifi

WiFiManager wm;
WiFiManagerParameter custom_field;
int customFieldLength = 40;
bool wm_nonblocking = false;
bool res;

String getParam(String name){
  String value;
  if(wm.server->hasArg(name)) {
    value = wm.server->arg(name);
  }
  return value;
}

void saveParamCallback(){
  Serial.println("[CALLBACK] saveParamCallback fired");
  Serial.println("PARAM customfieldid = " + getParam("customfieldid"));
}

//Função principal de configuração da rede.
void wifiConfigESP(){
  WiFi.mode(WIFI_STA);
  Serial.setDebugOutput(true);  
  delay(3000);
  Serial.println("\n Iniciando");

  if(wm_nonblocking) wm.setConfigPortalBlocking(false);

  const char* custom_radio_str = "<br/><label for='customfieldid'>Custom Field Label</label><input type='radio' name='customfieldid' value='1' checked> One<br><input type='radio' name='customfieldid' value='2'> Two<br><input type='radio' name='customfieldid' value='3'> Three";
  new (&custom_field) WiFiManagerParameter(custom_radio_str);
  std::vector<const char *> menu = {"wifi","info","param","sep","restart","exit"};

  wm.addParameter(&custom_field);
  wm.setSaveParamsCallback(saveParamCallback);
  wm.setMenu(menu);
  wm.setClass("invert");
  wm.setConfigPortalTimeout(60); 
  res = wm.autoConnect("ESP32_CONFIG","1A2b3c4d");

  if(!res) {
    Serial.println("Falha ao conectar ou o tempo limite esgotou :(");
  } 
  else {
    Serial.println("Conectado...yeey :)");
  }

}

//Função de resetar a rede, ao precionar o botão o acesso point é ativado.
void mudarRede(){
    if( digitalRead(BUTTON_RESET_WIFI) == LOW ){
      Serial.println("Botão Precionado");
      delay(180);
      if( digitalRead(BUTTON_RESET_WIFI) == LOW ){
        Serial.println("Botão retido");
        Serial.println("Apagando Configurações, restaurando ...");
        wm.resetSettings();
        ESP.restart();
      }

    }

}
