//Incluindo a biblioteca para o lcd
#include <LiquidCrystal.h>

//Declarando as variáveis
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int LDR_PIN = A0;
int Temp_PIN = A1;
int Hum_Pin = A2;
int Quant_Liquido_Pin = A3;
const int BUZZER_PIN = 9;

int luminosity;
float tempC;
int sensorhumidade;
float Armazen_Solu_Nutri;


// Variáveis para calcular a temperatura 
int RawValue = 0;
double Voltage = 0;



/* Define os limiares dos niveis do ambiente
(luminosidade, umidade, temperatura e reserva)
*/
int Lumi_OK = 870;    // Nível de luminosidade considerado OK
int Lumi_ALERTA = 950;  // Nível de luminosidade considerado em ALERTA
int humi_OK = 30; // Nível de umidade considerado OK
int humi_ALERTA = 70; // Nível de umidade considerado ALERTA
int temp_OK = 15 ;  // Nível de temperatura considerado OK  
int temp_ALERTA = 25; // Nível de temperatura considerado ALERTA
int Solu_Nutri_ALERTA = 1000; // Nível de reserva considerado ALERTA



/*Definindo saída ou entrada de informações, inicio do lcd
e início do Serial
*/
void setup() {
  lcd.begin(16, 2);
  lcd.clear();
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LDR_PIN, INPUT);
  pinMode(Temp_PIN, INPUT);
  pinMode(Hum_Pin, INPUT);
  pinMode(Quant_Liquido_Pin, INPUT);
  Serial.begin(9600);
   
}
 

//Função para controle de luminosidade
  void Lumi_Rod() {
    if(luminosity <= Lumi_OK){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Pouca Luz");
      noTone(BUZZER_PIN);
    }else if(luminosity <= Lumi_ALERTA){      	
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Luz OK");
      noTone(BUZZER_PIN);
    }else{
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Muita Luz");
      tone(BUZZER_PIN,1000,3000);
    }
  }
      

//Função para controle da quantidade de um liquido 
void Solu_Nutri_Rod() {
  if (map(Armazen_Solu_Nutri,0,1023,0,2000) <= Solu_Nutri_ALERTA) {
  	lcd.clear();
  	lcd.setCursor(0, 0);
  	lcd.print("Reserva BAIXA");
 	  lcd.setCursor(0, 1);
    lcd.print(map(Armazen_Solu_Nutri,0,1023,0,2000)); 
    lcd.print("L"); 
    tone(BUZZER_PIN,1000,3000); 	
    } else {
    lcd.clear();
  	lcd.setCursor(0, 0);
    lcd.print("Reserva  OK");
  	lcd.setCursor(0, 1);
    lcd.print(map(Armazen_Solu_Nutri,0,1023,0,2000)); 
    lcd.print("L"); 
    noTone(BUZZER_PIN);
  }
}


//Função para controle de temperatura
void Temp_Rod() {
  if (tempC<temp_OK) {
 	  lcd.clear();
  	lcd.setCursor(0, 0);
  	lcd.print("Temp. Baixa ");
  	lcd.setCursor(0, 1);
    lcd.print("Temp. = ");
  	lcd.println(tempC);
    lcd.setCursor(14, 1);
    lcd.print("C"); 
    tone(BUZZER_PIN,1000,3000);
    }else if (tempC<temp_ALERTA){
    lcd.clear();
  	lcd.setCursor(0, 0);
  	lcd.print("Temp. OK ");
  	lcd.setCursor(0, 1);
  	lcd.print("Temp. = ");
  	lcd.println(tempC);
    lcd.setCursor(14, 1);
    lcd.print("C"); 
    } else {
    lcd.clear();
  	lcd.setCursor(0, 0);
    lcd.print("Temp. ALTA ");
  	lcd.setCursor(0, 1);
  	lcd.print("Temp. = ");
  	lcd.println(tempC);
    lcd.setCursor(14, 1);
    lcd.print("C"); 
    tone(BUZZER_PIN,1000,3000);
  }
}


/*Função para controle de umidade. Com base em porcentagem, para 
menor que 30% seja umidade BAIXA; para menor que 70% seja OK;
e para maior que 70% esteja ALERTA.*/
void Humi_Rod() {
  if (map(sensorhumidade, 0, 1023, 0, 100)<humi_OK) {
  	lcd.clear();
  	lcd.setCursor(0, 0);
  	lcd.print("Umid. Baixa ");
  	lcd.setCursor(0, 1);
  	lcd.print("Umid. = ");
    lcd.setCursor(8, 1);
  	lcd.print(map(sensorhumidade, 0, 1023, 0, 100));
    lcd.println("%");
    tone(BUZZER_PIN,1000,3000);    
    }else if (map(sensorhumidade, 0, 1023, 0, 100)<humi_ALERTA){
    lcd.clear();
  	lcd.setCursor(0, 0);
  	lcd.print("Umid. OK ");
  	lcd.setCursor(0, 1);
  	lcd.print("Umid. = ");
    lcd.setCursor(8, 1);
  	lcd.print(map(sensorhumidade, 0, 1023, 0, 100)); 
    lcd.println("%");
    } else {
    lcd.clear();
  	lcd.setCursor(0, 0);
    lcd.print("Umid. ALTA ");
  	lcd.setCursor(0, 1);
  	lcd.print("Umid. =");
    lcd.setCursor(8, 1);
  	lcd.print(map(sensorhumidade, 0, 1023, 0, 100));
    lcd.println("%");
    tone(BUZZER_PIN,1000,3000);
  }
}

// Começo que rodará o código
void loop() {
  luminosity = analogRead(LDR_PIN); // Lê o valor do LDR
  sensorhumidade = analogRead(A2); // Lê o valor simulado da porta de umidade
  Armazen_Solu_Nutri = analogRead(A3);
  RawValue = analogRead(Temp_PIN); // Valor "cru" da temperatura
  Voltage = (RawValue / 1023.0) * 5000; // Cálcula para a voltagem 
  tempC = (Voltage-500) * 0.1; //Cálculo para a temperatura, com 500 de diferença  
  
  
  // Monitoração no monitor serial
  Serial.print("\n LDR  =  ");
  Serial.print(luminosity);
  Serial.print("\n Temperatura em C =  ");
  Serial.print(tempC);
  Serial.print("\n Humidade: "); 
  Serial.print(map(sensorhumidade, 0, 1023, 0, 100));
  Serial.println("%");
  Serial.print("\n Reserva: ");
  Serial.print(map(Armazen_Solu_Nutri, 0, 1023, 0, 2000));
  Serial.println("L");
  delay(1000);
    
 
  Humi_Rod();
  delay(2000);
  Temp_Rod();
  delay(2000);
  Solu_Nutri_Rod();
  delay(2000);
  Lumi_Rod();
  
}
  

      


  


  
    


  
  
  
   
 
