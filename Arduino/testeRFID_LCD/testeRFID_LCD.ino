/*
    Autor: Arthur Aleksandro @aretw0 (24/02/2018)    

*/

// Biblioteca para o arduino usar o protocolo SPI
#include <SPI.h>
// Biblioteca para manipulação do leitor RFID modelo MIFAREC522
#include <MFRC522.h>
// Biblioteca para utilizar um display LCD, aqui de 16x2
#include <LiquidCrystal.h>

// Configurações dos pinos do leitor RFID
#define SS_PIN 10
#define RST_PIN 9
// Construindo a variável que será a interface para utilizar o leitor RFID
MFRC522 mfrc522(SS_PIN, RST_PIN);

// Construindo a variável que será a interface para utilizar o display LCD
LiquidCrystal lcd(7, 6, 5, 4, 3, 2); 

bool readOk; // Variável para guardar o estado da leitura, se ok ou não;
// 0 ou 1 / LOW ou HIGH / false ou true

/*
 ASDASDASDASD
ASDASD

*/

// Função de configuração, será invocada apenas uma vez na inicialização do microcontrolador
void setup() 
{
  // Configurando portal serial do microcontrolador
  Serial.begin(9600); // taxa de transferencia
  SPI.begin();    // Inicia  SPI bus
  mfrc522.PCD_Init(); // Inicia MFRC522
  //Define o número de colunas e linhas do LCD:  
  lcd.begin(16, 2);  
  greetings();
}
// Função loop será executada eternamente
void loop() 
{
 
  // Procura por algum cartão presente na área
  if ( ! mfrc522.PICC_IsNewCardPresent())// função retorna boolean que é negado pela ! no começo
  {
    /*
     * Para evitar a função loop de se alongar pelo resto do programa
     * quebramos aqui o ciclo dando um return.
     * E assim a loop inicia outro ciclo.
    */
    return;
  }
  // Realiza leitura do cartão
  readOk = mfrc522.PICC_ReadCardSerial();
  infoLCD(readOk);
  if ( ! readOk ) 
  {
    return;
  }
  mostraUID(uidRead()); // Mostrando UID no LCD
} 

void greetings()
{
  Serial.println("Aproxime o seu cartao do leitor...");
  Serial.println();
  lcd.clear();
  lcd.print("Aproxime o seu");  
  lcd.setCursor(0,1);
  lcd.print("cartao do leitor...");  
}
void infoLCD(bool readed)
{
  lcd.clear();
  if (readed) {
    lcd.print("  Cartao Lido!");
    Serial.println("Cartão lido!");
  } else {
    lcd.print("  Cartao Não Lido!");
    Serial.println("Cartão não lido!");
  }
  delay(1000);   
}
void mostraUID(String id)
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("UID da tag");
  Serial.println("UID da tag");
  lcd.setCursor(0,1);
  lcd.print(id);
  Serial.println(id);
  delay(3000);
  greetings();
}

String uidRead()
{
   // Tratamento dos bytes do protocolo SPI
  String id = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     id.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     // é menor que 10? Se sim concatene " 0", senão concatene " "
     id.concat(String(mfrc522.uid.uidByte[i], HEX));
     // concatene o valor do byte transformado para HEXADECIMAL
  }
  /*Nesse momento temos o UID (UNIQUE IDENTIFIER)
    na variavel id, retornamos ela em CAIXA ALTA
  */
  id.toUpperCase();
  return id;
}

