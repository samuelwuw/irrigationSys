#include <WiFi.h> 
#include <PubSubClient.h>

//WiFi
const char* SSID = "Brancadeneve";                // SSID / nome da rede WiFi que deseja se conectar
const char* PASSWORD = "SantoTeres17";   // Senha da rede WiFi que deseja se conectar
WiFiClient wifiClient;                        
 
//MQTT Server
const char* BROKER_MQTT = "mqtt.eclipse.org"; //URL do broker MQTT que se deseja utilizar
int BROKER_PORT = 1883;                      // Porta do Broker MQTT

#define ID_MQTT  "TESTE_MQTT_YAYAYA"             //Informe um ID unico e seu. Caso sejam usados IDs repetidos a ultima conexão irá sobrepor a anterior. 
#define TOPIC_SUBSCRIBE "TOPIC_TESTE_NOOBY"   //Informe um Tópico único. Caso sejam usados tópicos em duplicidade, o último irá eliminar o anterior.
#define TOPIC_PUBLISH "TOPIC_TESTE_YAYAYA"
PubSubClient MQTT(wifiClient);        // Instancia o Cliente MQTT passando o objeto espClient

//Declaração das Funções
void mantemConexoes();  //Garante que as conexoes com WiFi e MQTT Broker se mantenham ativas
void conectaWiFi();     //Faz conexão com WiFi
void conectaMQTT();     //Faz conexão com Broker MQTT
void recebePacote(char* topic, byte* payload, unsigned int length);
void enviaPacote();

#define pinLED1 32  //Rele
int sensorPin = 14; //sensor
bool leituraSensor;
bool leituraSensorAut;
bool isAutomatic = false;

void setup() {
  pinMode(pinLED1, OUTPUT);
  pinMode(sensorPin, INPUT);         

  Serial.begin(115200);

  conectaWiFi();
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);  
  MQTT.setCallback(recebePacote); 
}

void loop() {
  if(isAutomatic == true){
    automatic();
  }
  
  mantemConexoes();
  enviaPacote();
  MQTT.loop();
  delay(1000);
}

void automatic(){
  //se leitura=HIGH, umidade baixa
  leituraSensorAut = digitalRead(sensorPin);

  if(leituraSensorAut == HIGH){
    digitalWrite(pinLED1, HIGH);
    delay(800);
    digitalWrite(pinLED1, LOW);
    Serial.println("irrigação feita");
  }
  if(leituraSensorAut == LOW){
    digitalWrite(pinLED1, LOW);
    Serial.println("desligado");
  }
}

void mantemConexoes() {
    if (!MQTT.connected()) {
       conectaMQTT(); 
    }
    
    conectaWiFi(); //se não há conexão com o WiFI, a conexão é refeita
}

void conectaWiFi() {

  if (WiFi.status() == WL_CONNECTED) {
     return;
  }
        
  Serial.print("Conectando-se na rede: ");
  Serial.print(SSID);
  Serial.println("  Aguarde!");

  WiFi.begin(SSID, PASSWORD); // Conecta na rede WI-FI  
  while (WiFi.status() != WL_CONNECTED) {
      delay(100);
      Serial.print(".");
  }
  
  Serial.println();
  Serial.print("Conectado com sucesso, na rede: ");
  Serial.print(SSID);  
  Serial.print("  IP obtido: ");
  Serial.println(WiFi.localIP()); 
}

void conectaMQTT() { 
    while (!MQTT.connected()) {
        Serial.print("Conectando ao Broker MQTT: ");
        Serial.println(BROKER_MQTT);
        if (MQTT.connect(ID_MQTT)) {
            Serial.println("Conectado ao Broker com sucesso!");
            MQTT.subscribe(TOPIC_SUBSCRIBE);
        } 
        else {
            Serial.println("Noo foi possivel se conectar ao broker.");
            Serial.println("Nova tentatica de conexao em 10s");
            delay(10000);
        }
    }
}

void recebePacote(char* topic, byte* payload, unsigned int length) 
{
    String msg;

    //obtem a string do payload recebido
    for(int i = 0; i < length; i++) 
    {
       char c = (char)payload[i];
       msg += c;
    }

    if (msg == "0") {
       digitalWrite(pinLED1, LOW);
    }

    if (msg == "1") {
       digitalWrite(pinLED1, HIGH);
       delay(2000);
       digitalWrite(pinLED1,LOW);
    }

    if (msg == "3"){
      Serial.println("AUTOMATICO DESLIGADO");
      isAutomatic = false;
    }

    if (msg == "4"){
      Serial.println("AUTOMATICO LIGADO");
      isAutomatic = true;
    }
}

void enviaPacote()
{
  leituraSensor = digitalRead(sensorPin);
  
  if(leituraSensor==HIGH) {
    MQTT.publish(TOPIC_PUBLISH, "baixa");
  } 
  else {
    MQTT.publish(TOPIC_PUBLISH, "alta");
  }
  
}
