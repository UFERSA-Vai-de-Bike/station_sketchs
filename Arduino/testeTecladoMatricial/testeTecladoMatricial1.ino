#define linha1 3 //123A
#define linha2 4 //456B
#define linha3 5 //789C
#define linha4 6 //x0#D

#define coluna1 7 // teste de leitura
#define coluna2 8 // teste de leitura
#define coluna3 9 // teste de leitura
#define coluna4 10 // teste de leitura

/*

        A lógica desse código é:

        - Desliga-se todas as linhas e liga-se uma por vez
        - Para cada linha ligada realiza-se a leitura da coluna
        - Como apenas uma linha está ligada, a leitura da coluna indica qual número foi apertado
        

*/

int state = 0;
String pass = "1234ab";
String passTemp = "";

unsigned long timeStamp;
unsigned long timeNow = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(linha1,OUTPUT);
  pinMode(linha2,OUTPUT);
  pinMode(linha3,OUTPUT);
  pinMode(linha4,OUTPUT);

  pinMode(coluna1,INPUT);
  pinMode(coluna2,INPUT);
  pinMode(coluna3,INPUT);
  pinMode(coluna4,INPUT);
  
  shutdownBoard();

  Serial.begin(9600);
  Serial.println("UFERSA Vai de Bike - Teste do teclado Matricial");
}

void loop() {
  // put your main code here, to run repeatedly:
  greetingsPassword();
  while (readBoard()){
    timeStamp = millis();
  };
  resetDatapath();
  delay(2000);
}


bool readBoard() {
  int li;
  int col = 0;
  for (li = 3; li < 7; ++li ) {
    shutdownBoard();
    digitalWrite(li,HIGH);

     //Verifica se alguma tecla da coluna 1 foi pressionada
    if (digitalRead(coluna1))
    {
      col = 1;
      while(digitalRead(coluna1)){};
      break;
    }
    //Verifica se alguma tecla da coluna 2 foi pressionada
    if (digitalRead(coluna2))
    {
      delay(3000);
      col = 2;
      while(digitalRead(coluna2)){};
      break;
    }

    //Verifica se alguma tecla da coluna 1 foi pressionada
    if (digitalRead(coluna3))
    {
      col = 3;
      while(digitalRead(coluna3)){};
      break;
    }

    if (digitalRead(coluna4))
    {
      col = 4;
      while(digitalRead(coluna4)){};
      break;
    }
  }
  return whatDo(li,col);
}

bool whatDo(int l, int c) {
  switch (l) {
    case 3:
      switch (c) {
        case 1:
          Serial.print("1");
          passTemp.concat('1');
          break;
        case 2:
          Serial.print("2");
          passTemp.concat('2');
          break;
        case 3:
          Serial.print("3");
          passTemp.concat('3');
          break;
        case 4:
          Serial.print("a");
          passTemp.concat('a');
          break;
        default: 
          return hasTime();
      }
    case 4:
      switch (c) {
        case 1:
          Serial.print("4");
          passTemp.concat('4');
          break;
        case 2:
          Serial.print("5");
          passTemp.concat('5');
          break;
        case 3:
          Serial.print("6");
          passTemp.concat('6');
          break;
        case 4:
          Serial.print("b");
          passTemp.concat('b');
          break;
        default: 
          return hasTime();
          break;
      }
    case 5:
      switch (c) {
        case 1:
          Serial.print("7");
          passTemp.concat('7');
          break;
        case 2:
          Serial.print("8");
          passTemp.concat('8');
          break;
        case 3:
          Serial.print("9");
          passTemp.concat('9');
          break;
        case 4: // c
          Serial.print("c");
          passTemp.concat('c');
          break;
        default:
          return hasTime();
          break;
      }
    case 6: // linha 4
      switch (c) {
        case 1: // *
          Serial.println();
          Serial.println("Senha recebida, a comparar");
          if (passTemp == pass) {
            Serial.println("Senha correta!");
            // digitalWrite(releTrava,HIGH);
          } else {
            Serial.println("Senha incorreta!");
          }
          state = 2;
          return false;
        case 2: // 0
          Serial.print("0");
          passTemp.concat('0');
          break;
        case 3: // #
//          Serial.println("#");
          Serial.println();
          Serial.println("Resetando!");
          state = 2;
          return false;
        case 4: // D
          Serial.print("d");
          passTemp.concat('d');
          break;
        default: 
          return hasTime();
          break;
      }
    default: 
      return hasTime();
      break;
  }
  return true;  
}

bool hasTime() {
  timeNow = millis() - timeStamp;
  if (timeNow > 5000){
      Serial.println();
      Serial.println("Tempo de espera excedido!!!");
      return false;
  }
  return true;
}

void shutdownBoard() {
  digitalWrite(linha1,LOW);
  digitalWrite(linha2,LOW);
  digitalWrite(linha3,LOW);
  digitalWrite(linha4,LOW);
}
void resetDatapath() {
  passTemp = "";
  timeNow = 0;
}

void greetingsPassword() {
  Serial.println();
  Serial.println("Digite sua senha (1234AB): ");
}