#define linha1 3//123A
#define linha2 4 //456B
#define linha3 5 //789C
#define linha4 6 //x0#D

#define coluna1 7// teste de leitura
#define coluna2 8// teste de leitura
#define coluna3 9// teste de leitura
#define coluna4 10// teste de leitura

/*

        A lógica desse código é:

        - Desliga-se todas as linhas e liga-se uma por vez
        - Para cada linha ligada realiza-se a leitura da coluna
        - Como apenas uma linha está ligada, a leitura da coluna indica qual número foi apertado
        

*/

int leitura = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(linha1,OUTPUT);
  pinMode(linha2,OUTPUT);
  pinMode(linha3,OUTPUT);
  pinMode(linha4,OUTPUT);

  pinMode(linha1,OUTPUT);
  pinMode(linha2,OUTPUT);
  pinMode(linha3,OUTPUT);
  pinMode(linha4,OUTPUT);

  pinMode(coluna1,INPUT);
  pinMode(coluna2,INPUT);
  pinMode(coluna3,INPUT);
  pinMode(coluna4,INPUT);
  
  digitalWrite(linha1,HIGH);
  digitalWrite(linha2,LOW);
  digitalWrite(linha3,LOW);
  digitalWrite(linha4,LOW);

  Serial.begin(9600);
  Serial.println("UFERSA Vai de Bike - Teste do teclado Matricial");
}

void loop() {
  // put your main code here, to run repeatedly:
  leitura = digitalRead(coluna1);

  if (leitura) {
    Serial.println("Algum botão apertado");
  } else {
    Serial.println("Nada foi apertado");
  }
  delay(500);
}
