// Prototipo de irrigacao automatica
// Autor: Narciso
// Orientacao: Prof. Fabio Rolim e Robson Freitas

// Carrega as bibliotecas do LCD e do Sensor de temperatura
#include <LiquidCrystal.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Porta do pino de sinal do DS18B20
#define ONE_WIRE_BUS 6

// Define uma instancia do oneWire para comunicacao com o sensor
OneWire oneWire(ONE_WIRE_BUS);

// Inicializa o LCD
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int rele1 = 7;


//Variaveis para sensores
int irrigado = 0;
float temperatura = 0;
int umidade;
String indice_umidade = "";
int int_umidade;

//Objetos do sensor de temperatura
DallasTemperature sensors(&oneWire);
DeviceAddress sensor1;

void setup(){
  // Define o LCD com 20 colunas e 4 linhas
  lcd.begin(20, 4);
  Serial.begin(9600);
  // Define pinos do reley e LEDS
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(rele1, OUTPUT);

  // Localiza e mostra enderecos do sensore
  Serial.println("Localizando sensore DS18B20...");
  Serial.print("Foram encontrados ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" sensores.");
  if (!sensors.getAddress(sensor1, 0))
    Serial.println("Sensores nao encontrados !");
  // Mostra o endereco do sensor encontrado no barramento
  Serial.print("Endereco sensor: ");
  mostra_endereco_sensor(sensor1);
  Serial.println();
  Serial.println();
}

void mostra_endereco_sensor(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    // Adiciona zeros se necessário
    if (deviceAddress[i] < 16) Serial.print("0");
      Serial.print(deviceAddress[i], HEX);
  }
}
   
  

void loop(){
  lcd.noDisplay();
  lcd.display();
  digitalWrite(rele1, HIGH);

  //umidade
  umidade = analogRead(A1);
  umidade = map(umidade, 1023, 0, 0, 200);

  //temperatura
  sensors.requestTemperatures();
  float temp = sensors.getTempC(sensor1);

 //Recebendo dados do bluetooth
 //String dados;  
    
    while(Serial.available() > 0 ){
      char caractere = char(Serial.read());
       if (caractere == 'i'){
          indice_umidade = "";
        }
       if (caractere != 't' && caractere !='i'){
        indice_umidade += caractere;
      }
    }
 

  int_umidade = (indice_umidade.toInt());
  /*if (int_umidade == 0){
     int_umidade = 30;  
  }*/

  /*if (dados == "irrigar"){
    digitalWrite(rele1, LOW);
    irrigado++;
    lcd.noDisplay();
    lcd.display();
  }*/
  
  //Enviando dados para bluetooth
  Serial.print("Umidade: ");
  Serial.print(umidade);
  Serial.print("%");
  Serial.println();
  Serial.print("Temperatura: ");
  Serial.print(temp);
  //Serial.write(223);
  Serial.print("C");
  Serial.println();
  Serial.print("Irrigado: ");
  Serial.println(irrigado);
  //Serial.println(int_umidade);
  Serial.println("f");

  while (umidade < int_umidade) {
    digitalWrite(rele1, LOW);
    lcd.clear();
    lcd.noDisplay();
    lcd.display();
    lcd.setCursor(4, 1);
    lcd.print("Irrigando...");
    Serial.print("Irrigando...");
    Serial.print("f");
    lcd.setCursor(8, 2);
    lcd.print(":)");
    delay(2000);
    umidade = analogRead(A1);
    umidade = map(umidade, 1023, 0, 0, 200);
    if (umidade > int_umidade) {
      digitalWrite(rele1, HIGH);
      irrigado++;
      lcd.noDisplay();
      lcd.display();
      break;
    }
  }

  // Tela de dados
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("IFPI - OEIRAS");
  lcd.setCursor(0, 1);
  lcd.print("TEMP:");
  lcd.setCursor(13, 1);
  lcd.print(temp);
  lcd.setCursor(18, 1);
  lcd.write(223);
  lcd.print("C");
  lcd.setCursor(0, 2);
  lcd.print("UMIDADE:");
  lcd.setCursor(16, 2);
  lcd.print(umidade);
  lcd.setCursor(19, 2);
  lcd.print("%");
  lcd.setCursor(0, 3);
  lcd.print("IRRIGADO:");
  lcd.setCursor(16, 3);
  lcd.print(irrigado);
  //indice_umidade = "";
  lcd.setCursor(19, 3);
  lcd.print("X");
  lcd.noDisplay();
  lcd.display();
  delay(4000);
}
