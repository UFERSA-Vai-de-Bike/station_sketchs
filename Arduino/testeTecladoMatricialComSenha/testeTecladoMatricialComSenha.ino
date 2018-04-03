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

int leitura = 0;
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
  
  Serial.println();
  Serial.println("Digite sua senha (1234AB): ");

  timeStamp = millis();
  char temp = ' ';
  while(((temp != '*') || (temp != '#')) && (timeNow < 5000) )  {
    
    temp = readBoard();

    switch (temp) {
      case '*':
        
        break;
      case '#':
        Serial.println();
        Serial.println("Resetando!");
        break;
      case ' ':
        timeNow = millis() - timeStamp;
        if (timeNow > 5000){
            Serial.println();
            Serial.println("Tempo de espera excedido!!!");
        } 
        break;
      default:
        passTemp.concat(temp);
        timeStamp = millis();
        break;
    }
  }
 
  delay(1000);
  resetDatapath();

}


void readBoard() {
  int li;
  int col = 0;
  for (li = 3; li < 7; ++li ) {
    shutdownBoard();
    digitalWrite(li,HIGH);

     //Verifica se alguma tecla da coluna 1 foi pressionada
    if (digitalRead(coluna1))
    {
//      Serial.println("Coluna 1");
      col = 1;
      while(digitalRead(coluna1)){};
      break;
    }
    //Verifica se alguma tecla da coluna 2 foi pressionada
    if (digitalRead(coluna2))
    {
//      Serial.println("Coluna 2");
      delay(3000);
      col = 2;
      while(digitalRead(coluna2)){};
      break;
    }

    //Verifica se alguma tecla da coluna 1 foi pressionada
    if (digitalRead(coluna3))
    {
//      Serial.println("Coluna 3");

      col = 3;
      while(digitalRead(coluna3)){};
      break;
    }

    if (digitalRead(coluna4))
    {
//      Serial.println("Coluna 4");
      col = 4;
      while(digitalRead(coluna4)){};
      break;
    }
  }
//  delay(10);
  whatDo(li,col);
}

void whatDo(int l, int c) {
  switch (l) {
    case 3:
      switch (c) {
        case 1:
          Serial.print("1");
          return '1';
        case 2:
          Serial.print("2");
          return '2';
        case 3:
          Serial.print("3");
          return '3';
        case 4:
          Serial.print("a");
          return 'a';
        default: 
          return ' ';
      }
    case 4:
      switch (c) {
        case 1:
          Serial.print("4");
          return '4';
        case 2:
          Serial.print("5");
          return '5';
        case 3:
          Serial.print("6");
          return '6';
        case 4:
          Serial.print("b");
          return 'b';
        default: 
          return ' ';
      }
    case 5:
      switch (c) {
        case 1:
          Serial.print("7");
          return '7';
        case 2:
          Serial.print("8");
          return '8';
        case 3:
          Serial.print("9");
          return '9';
        case 4:
          Serial.print("c");
          return 'c';
        default: 
          return ' ';
      }
    case 6:
      switch (c) {
        case 1:
          Serial.println();
          Serial.println("Senha recebida, a comparar");
          if (passTemp == pass) {
            Serial.println("Senha correta!");
            // digitalWrite(releTrava,HIGH);
          } else {
            Serial.println("Senha incorreta!");
          }
          break;
        case 2:
          Serial.print("0");
          return '0';
        case 3:
//          Serial.println("#");
          return '#';
        case 4:
          Serial.print("D");
          return 'D';
        default: 
          return ' ';
      }
    default: 
      hasTime();
      break;
  }
  
}

void hasTime() {
  timeNow = millis() - timeStamp;
        if (timeNow > 5000){
            Serial.println();
            Serial.println("Tempo de espera excedido!!!");
        } 
}

void shutdownBoard() {
//  Serial.println("Desligando linhas!");
  digitalWrite(linha1,LOW);
  digitalWrite(linha2,LOW);
  digitalWrite(linha3,LOW);
  digitalWrite(linha4,LOW);
}
void resetDatapath() {
  passTemp = "";
  timeNow = 0;
}
