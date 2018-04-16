#include <Wire.h>

#define RFID_READER_ADDRESS 1

void setup() {
 Serial.begin(9600); /* Inicia serial para debug */
 Wire.begin(); /* Entra na i2c */
}

void loop() {
 Wire.beginTransmission(8); /* begin with device address 8 */
 Wire.write("Hello Arduino");  /* sends hello string */
 Wire.endTransmission();    /* stop transmitting */

 Wire.requestFrom(8, 13); /* request & read data of size 13 from slave */
 while(Wire.available()){
    char c = Wire.read();
  Serial.print(c);
 }
 Serial.println();
 delay(1000);
}

void getRFID(){
    sendI2C("1",RFID_READER_ADDRESS);
    delay(700);
    Serial.println();
    Serial.println(" - Requisitando leitura RFID -");
    handleTrans

}
void sendI2C(String send, short address){
    Wire.beginTransmission(address); /* endereço de envio */
    Wire.write(send);  /* envia string */
    Wire.endTransmission();    /* stop transmitting */
}

String getI2C(short address,short size){
    String result = "";

    Wire.requestFrom(address, size);
    /* requisita e le dados do tamanho dado*/
    Serial.println();
    Serial.println("- Leitura I2C -");
    while(Wire.available()){
        char c = Wire.read();
        Serial.print(c);
        if (result.concat(c)) {
            Serial.println()
            Serial.println("Falha na concatenação!");
            Serial.println()
            Serial.print(String(result + c));
        }
    }
    Serial.println(String("Resultado final: " + result));
    return result;
}