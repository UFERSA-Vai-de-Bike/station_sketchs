//#include <LiquidCrystal.h>
// se precisar com LCD
// LiquidCrystal lcd(13, 12, 11, 10, 9, 8);
 
byte rows[] = {5,4,3,2}; // linhas
byte columns[] = {A5,A4,A3,A2};// colunas
 
char passTemp[10]; // limite de alocação, senha de 10 digitos no máximo

char pass[10] = {'B','A','N','A','N','A','\0'};

char operacao = '\0';

const char keys[4][4] = {
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}
};
 
char getKey(){
    char k = '\0';
    for (int r=0; r<4; r++){
        digitalWrite(rows[r], LOW);
        for (int c=0; c<4; c++){
        if (digitalRead(columns[c]) == LOW){
            k = keys[r][c];
            break;
        }
        }
        digitalWrite(rows[r], HIGH);
        if (k) { break; }
    }
    return k;
}
 
char getKeyOnce(){
    // variaveis estáticas persistem de uma chamada a outra
    static char lastVal = '\0';
    static unsigned long m = 0;
    char val = getKey();
    if (lastVal != val && millis() > (m+100) ) { //M+100 -->DEBOUNCING 100ms
        lastVal = val;
        m = millis();
        return lastVal;
    } 
    return '\0';
}
 
void setup() {
  Serial.begin(9600);
  
//   lcd.begin(16, 2);
//   lcd.clear();
//   lcd.setCursor(0, 0);
//   lcd.print("Teclado Matricial - Simples");

  Serial.println("Teclado Matricial - Simples");
   
   // inicializando portas
  for (int i=0; i<4; i++){
    pinMode(rows[i], OUTPUT);      // DEFININDO SAIDA
    pinMode(columns[i], INPUT_PULLUP); // PULLUP MELHORA A LEITURA
    digitalWrite(rows[i], HIGH); // levantando todas
  }
   
  delay(3000);
   
//   lcd.clear();
//   lcd.setCursor(0, 0);
}
 
void loop() {
  
  if (getKeyOnce() != '\0') { // enquanto não houver uma interação não entra nesse fluxo
    if (execKeypad()) {
        if (checkPass()) {
            Serial.println();    
            Serial.println("Senha correta, fluxo terminado.");        
        } else {
            Serial.println("Senha incorreta, fluxo terminado.");        
        }
    } else {
        Serial.println();    
        Serial.println("Operação de teclado cancelada.");    
    }
  } else {
    Serial.println(" Sem interação no teclado, tentando de novo...");
    delay(1000); // espera um minuto para tentar de novo
  }
}

bool checkPass(){
    for (int i = 0; i < 10; ++i){
        if (passTemp[i] == '\0' && pass[i] == '\0') { // chega se chegou ao fim junto
            return true;
        }
        if ((passTemp[i] != pass[i])) { // se qualquer caractere for diferente retorna falso
            return false;
        }
    }
}
 
bool execKeypad(){
    int index = 0;
    unsigned long timeStamp = millis();
    char key;
    while (true) { // perigoso, esperamos que alguém quebre isso com um return true ou false
        key = getKeyOnce();
        switch (key) {
            case '*':
                if (index > 0) {
                    passTemp[index] = "\0";
                    Serial.println();
                    Serial.println("Comparar senhas!");
                    // comparar senhas
                    return true;
                } else {
                    Serial.println();
                    Serial.println("Input vazio!");
                    return false;
                }
                break;
            case '#':
                if (index > 0) {
                    Serial.println();
                    Serial.println("Resetando!");
                    Serial.println();
                    index = 0;
                    passTemp[index] = "\0";
                } else {
                    return false;
                }
                timeStamp = millis();
                break;
            case '\0':
                if ((millis() - timeStamp) > 5000){
                    Serial.println();
                    Serial.println("Tempo de espera excedido!!!");
                    return false;
                } 
                break;
            default:
                Serial.print(key);
                passTemp[index] = key;
                ++index;
                timeStamp = millis();
                if (index > 9) {
                    Serial.println();
                    Serial.println("Limite de senha alcançado!");
                    Serial.println();
                    return true;
                }
                break;
        }
    }
}