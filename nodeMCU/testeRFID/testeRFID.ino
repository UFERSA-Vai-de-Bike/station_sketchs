/*
    Autor: Arthur Aleksandro @aretw0 (01/03/2018)

    IMPORTANTE: https://github.com/miguelbalboa/rfid#pin-layout
*/
// Biblioteca para o arduino usar o protocolo SPI
#include <SPI.h>
// Biblioteca para manipulação do leitor RFID modelo MIFAREC522
#include <MFRC522.h>

// Configurações dos pinos do leitor RFID no nodemcu
#define SS_PIN D8
#define RST_PIN D3
// Construindo a variável que será a interface para utilizar o leitor RFID
MFRC522 mfrc522(SS_PIN, RST_PIN);

bool readOk; // Variável para guardar o estado da leitura, se ok ou não;

// Função de configuração, será invocada apenas uma vez na inicialização do microcontrolador
void setup() 
{
  // Configurando portal serial do microcontrolador
  Serial.begin(9600);
  SPI.begin();    // Inicia  SPI bus
  mfrc522.PCD_Init(); // Inicia MFRC522
  greetings();
}
// Função loop será executada eternamente
void loop() 
{
  // Procura por algum cartão presente na área
  if ( ! mfrc522.PICC_IsNewCardPresent()) // função retorna boolean que é negado pela ! no começo
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
}
void infoLCD(bool readed)
{
  if (readed) {
    Serial.println("Cartão lido!");
  } else {
    Serial.println("Cartão não lido!");
  }
  delay(1000);   
}
void mostraUID(String id)
{
  Serial.println("UID da tag");
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
