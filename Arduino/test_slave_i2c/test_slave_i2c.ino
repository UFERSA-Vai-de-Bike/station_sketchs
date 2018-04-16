// Biblioteca para o arduino usar o protocolo SPI
#include <SPI.h>
// Biblioteca para manipulação do leitor RFID modelo MIFAREC522
#include <MFRC522.h>

// Configurações dos pinos do leitor RFID
#define SS_PIN 10
#define RST_PIN 9

// Definição de endereço
#define RFID_READER_ADDRESS 1

#include <Wire.h>


short state_op = 0;

void setup() {
  Serial.begin(9600);           /* Inicia serial para debug */
  
  Wire.begin(RFID_READER_ADDRESS); /* Entra na i2c com Endereço 1 */
  Wire.onRequest(requestEvent); /* register request event */
  Wire.onReceive(receiveEvent); // registra o evento de recebimento
  SPI.begin();    // Inicia  SPI bus
  mfrc522.PCD_Init(); // Inicia MFRC522
}

void loop() {
 delay(100);
}

// função que executa quando é recebido do mestre
void receiveEvent(int howMany) {

 while (0 < Wire.available()) {
    char c = Wire.read();      /* receive byte as a character */
    Serial.println();
    Serial.print(c);
    Serial.print(" Recebido");
    if (c == '1') {
      state_op = 1; // PROXIMO EVENTO DE REQUEST JÁ SABE PARA ONDE IR
      Serial.println(" - Proximo evento operamos o leitor RFID");
    }
    Serial.println();
  }
}

// função que executa quando dados são requisitados do mestre
void requestEvent() {
  switch (state_op) {
    case 1:
      // Faça o que é preciso pra ler um cartão RFID
      Wire.write(readRFID());  /* Envia 15 bytes*/
      state_op = 0;
      break;
    case 0:
      Wire.write("TESTE I2C");  /* Envia 9 bytes*/
      break;
    default:
      Wire.write("TESTE I2C");  /* Envia 9 bytes*/
      break;
  }
}

String readRFID() {

  String result = "0 - 00 00 00 00";
  unsigned long timeStamp = millis();
   // Procura por algum cartão presente na área
  while( ! mfrc522.PICC_IsNewCardPresent())// função retorna boolean que é negado pela ! no começo
  {
    Serial.println();
    if ((millis()- timeStamp) > 5000) {
      Serial.println("Tempo expirado - Cartão não presente!");
      return "8 - 00 00 00 00";
    }
    Serial.println(" - Cartão não presente!");
  }
  timeStamp = millis();

  while( ! mfrc522.PICC_ReadCardSerial())// função retorna boolean que é negado pela ! no começo
  {
    Serial.println();
    if ((millis()- timeStamp) > 5000) {
      Serial.println("Tempo expirado - Falha na leitura!");
      return "9 - 00 00 00 00";
    }
    Serial.println(" - Falha na leitura!");
  }
  timeStamp = millis();
  Serial.println();
  Serial.println("- Leitura realizada -");
  result = "1 - ";
  if (result.concat(uidRead()) {
    Serial.println();
    Serial.println("- Retornando resultado - ");
    return result;
  } else {
    Serial.println();
    Serial.println("Falha na concatenação do resultado!");
    return "5 - 00 00 00 00";
  }
}

String uidRead()
{
  // Tratamento dos bytes do protocolo SPI
  String id = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
    if (i != 0) {
      id.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    } else {
      id.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
    }
     // é menor que 10? Se sim concatene " 0", senão concatene " "
     id.concat(String(mfrc522.uid.uidByte[i], HEX));
     // concatene o valor do byte transformado para HEXADECIMAL
  }
  /*Nesse momento temos o UID (UNIQUE IDENTIFIER)
    na variavel id, retornamos ela em CAIXA ALTA
  */
  id.toUpperCase();

  Serial.println();
  Serial.print("UID - ");
  Serial.println(id);

  return id;
}
