#include <LiquidCrystal.h>
// se precisar com LCD
 LiquidCrystal lcd(7, 6, 5, 4, 3, 2);


#define KEYBOARD_AWAIT 600000
#define INPUT_AWAIT 10000
#define PASSWORD_AWAIT 10000
#define PASSWORD_SIZE 10

unsigned long timestamp = 0; // para armazenar millis periodicamente

// byte rows[] = {5,4,3,2}; // linhas ARDUINO UNO
// byte columns[] = {A5,A4,A3,A2};// colunas ARDUINO UNO
byte rows[] = {44,45,46,47}; // linhas ARDUINO MEGA
byte columns[] = {40,41,42,43};// colunas ARDUINO MEGA
 
char passTemp[10]; // limite de alocação, senha de 10 digitos no máximo
char inputKeyBoard[20];

char pass[10] = {'5','4','2','1','\0'}; // SENHA TESTE

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

bool checkTimeAwait(unsigned long await) {
  if ((millis() - timestamp) > await) {
    Serial.println();
    Serial.println(String("Tempo de espera excedido (" + await  +')'));
    timestamp = millis();
    return true;
  } else {
    return false;
  }
}
 
void setup() {
  Serial.begin(9600);
  
   lcd.begin(16, 2);
   lcd.clear();
   lcd.setCursor(0, 0);
   lcd.print("Teclado Matricial - Simples");

  Serial.println("Teclado Matricial - Simples");
   
   // inicializando portas para uso
  for (int i=0; i<4; i++){
    pinMode(rows[i], OUTPUT);      // DEFININDO SAIDA
    pinMode(columns[i], INPUT_PULLUP); // PULLUP MELHORA A LEITURA
    digitalWrite(rows[i], HIGH); // levantando todas
  }

  // inicialização teste do cabo do teclado
 
   
  delay(3000);

  Serial.println("Teste do teclado - UFERSA Vai de bike");
   
//   lcd.clear();
//   lcd.setCursor(0, 0);
}

void setPinTrial(){
   for (int i = 0; i<4; ++i){
    pinMode(columns[i], INPUT_PULLUP); // PULLUP MELHORA A LEITURA
    pinMode(rows[i], INPUT_PULLUP);      // DEFININDO SAIDA
  }
}

void loop() {
//  runTryCableKeyboard();
//  execKeypad(20);
operation4();
}

// Teste rudimentar, aparecera valores de acordo com o INPUT_PULLUP definido
// Testem colocando o cabo diretamente no GND do arduino, logo saberá de qual porta vem o sinal diferente.
void runTryCableKeyboard(){
  Serial.println("Teste do teclado - UFERSA Vai de bike");
  Serial.println("2s para ler portas...");
  delay(1800);
  Serial.println("A Ler portas...");
  delay(200);
  for (int i = 0; i < 4; ++i) {  
    Serial.print("Linha ");
    Serial.print(i+1);
    Serial.print(": ");
    Serial.print(digitalRead(rows[i]));
    Serial.print("\tColuna ");
    Serial.print(i+1);
    Serial.print(": ");
    Serial.print(digitalRead(columns[i]));
    Serial.println();
    delay(1000);
  }
  Serial.println("Leitura terminada, saindo em 5s...");
  delay(5000);
  Serial.println();
}

void operation4(){
  Serial.println();    
  Serial.println("Operação 4: Leitura da senha");
  Serial.println("Aperta qualquer botão para");
  if (getKeyOnce() != '\0') { // enquanto não houver uma interação não entra nesse fluxo
    if (execKeypad(PASSWORD_SIZE)) {
        if (checkPass()) {
            Serial.println();    
            Serial.println("Senha correta, fluxo terminado.");
            // state_op = 2// proxima operação        
        } else {
            Serial.println("Senha incorreta, fluxo terminado.");        
            // state_op = 2// proxima operação
        }
    } else {
        Serial.println();    
        Serial.println("Operação de teclado cancelada.");    
    }
  } else {
    Serial.println(" Sem interação no teclado, tentando de novo...");
    if (checkTimeAwait(PASSWORD_AWAIT)) { // passado 10 minutos
      //state_op = 0; vai para operação de 
    }
    delay(1000); // espera um segundo para tentar de novo
  }
}

bool checkPass(){
    for (int i = 0; i < 10; ++i){
        if (inputKeyBoard[i] == '\0' && pass[i] == '\0') { // chega se chegou ao fim junto
            return true;
        }
        if ((inputKeyBoard[i] != pass[i])) { // se qualquer caractere for diferente retorna falso
            return false;
        }
    }
}
 
bool execKeypad(int input_size){
    short index = 0;
    timestamp = millis();
    char key;
    while (true) { // perigoso, esperamos que alguém quebre isso com um return true ou false
        key = getKeyOnce();
        switch (key) {
            case '*':
                if (index > 0) {
                    inputKeyBoard[index] = '\0';
                    Serial.println();
                    Serial.println("Botão * pressionado: Confirmar input!");
                    Serial.println();
                    // comparar senhas
                    return true;
                } else {
                    Serial.println();
                    Serial.println("Botão * pressionado: Cancelando (input vazio)!");
                    Serial.println();
                    return false;
                }
                break;
            case '#':
                timestamp = millis();
                if (index > 0) {
                    Serial.println();
                    Serial.println("Botão # pressionado: Zerando input!");
                    Serial.println();
                    index = 0;
                    inputKeyBoard[index] = '\0';
                } else {
                    Serial.println();
                    Serial.println("Botão # pressionado: Cancelando (input vazio)!");
                    Serial.println();
                    return false;
                }
                break;
            case '\0':
                if (checkTimeAwait(INPUT_AWAIT)){
                    return false;
                } 
                break;
            default:
                timestamp = millis();
                Serial.print(key);
                inputKeyBoard[index] = key;
                ++index;
                if (index > (input_size-1)) {
                    Serial.println();
                    Serial.println("Limite de input alcançado!");
                    Serial.println();
                    return true;
                }
                break;
        }
    }
}
