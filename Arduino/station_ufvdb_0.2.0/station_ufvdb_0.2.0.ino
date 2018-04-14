// Estação do ufersa Vai de bike v0.0.1

// Primeiro passo, configurar periféricos.

// Complexos: 

// Ethernet SHIELD e RFID
#include <SPI.h>
//  O arduino faz uso do protocolo SPI para controlar sua porta ETHERNET
#include <Ethernet.h> // Biblioteca do ETHERNET
// #include <ArduinoJson.h> // PARA MANIPULAR AS RESPOSTAS
#define SERVER_TRY_CONNECTION 10
#define SERVER_PORT 3000
#define SERVER_1 192 // PRIMEIRA PARTE DO IP
#define SERVER_2 168 // SEGUNDA PARTE
#define SERVER_3 0 // TERCEIRA PARTE
#define SERVER_4 5 // QUARTA PARTE

// RFID
// Biblioteca para manipulação do leitor RFID modelo MIFAREC522
#include <MFRC522.h>
// Configurações dos pinos do leitor RFID
#define SS_PIN 53
#define RST_PIN 49
#define RFID_AWAIT 15000
#define UIDTEST "UIDTESTE" // TROQUE SUA UID DE TESTE AQUI

// Simples

// Teclado Matricial
#define KEYBOARD_AWAIT 600000
#define INPUT_AWAIT 10000
#define PASSWORD_AWAIT 10000
#define PASSWORD_SIZE 10

// TRAVA
#define LOCK_PORT 8

// Visor LCD
#include <LiquidCrystal.h>
LiquidCrystal lcd(7,6,5, 4, 3, 2);


// VARIÁVEIS DA ETHERNET
const byte mac[] = {0x00, 0xAB, 0xBC, 0xCD, 0xDF, 0x03}; // MAC DA ESTAÇÃO 0
//StaticJsonBuffer<200> jsonBuffer; //  PARA MANIPULAÇÃO DE JSON
boolean doRequest = true;
IPAddress server(SERVER_1,SERVER_2,SERVER_3,SERVER_4); //INICIALIZANDO IP DO SERVIDOR
EthernetClient client;
String serverIp = "192.168.0.5";//String(""+SERVER_1 + '.' + SERVER_2 + '.' + SERVER_3 + '.' + SERVER_4);

// VARIAVEIS PORTAS DO TECLADO
const byte rows[] = {44,45,46,47}; // linhas ARDUINO MEGA
const byte columns[] = {40,41,42,43};// colunas ARDUINO MEGA
const char keys[4][4] = {
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}
};
const char pass[5] = {'5','4','2','1','\0'}; // SENHA TESTE
char inputKeyBoard[20];

unsigned long timestamp = 0; // para armazenar millis periodicamente

// VARIAVEIS RFID
MFRC522 mfrc522(SS_PIN, RST_PIN);
String uidReaded;
bool readOk; // Variável para guardar o estado da leitura, se ok ou não;


short state_op = 0; // variável de operação

void setup(){

    SPI.begin();    // Inicia SPI bus
  mfrc522.PCD_Init(); // Inicia MFRC522

  setKeyPins(); // inicializando portas do teclado

  lcd.begin(16, 2);

  initEthernet();

  pinMode(LOCK_PORT,OUTPUT);
  digitalWrite(LOCK_PORT,HIGH);
  Serial.begin(9600); // 9600 taxa de transferencia inicializa SERIAL

  delay(1000);
  
  initializeMsg();
}

void loop(){

  
    switch (state_op) {
        case 0:
            // operação 0
           operation0();
          break;
        case 1:
            // operação 1
           operation1();
          break;
        case 2:
            // operação 2
           operation2();
          break;
        case 3:
            // operação 3
           operation3();
          break;
        case 4:
            // operação 4
            operation4();
          break;
        case 5:
            // operação 5
           operation5();
          break;
        case 6:
            // operação 6
           operation6();
          break;
        default:
            
            // sem operação, mas pode-se realizar operações iguais a estação em geral
            // Calcular horário (?)
            state_op = 0; // voltando ao rumo
            break;
    }
}

void operation0(){
    Serial.println("Conectando...");
      lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Conectando...");
  lcd.setCursor(0,1);
  lcd.print("Aguarde");
    // if you get a connection, report back via serial:
    if (openConnection()) {
      // Make a HTTP request:
      doGetAPI("api");

     // tratar conexão aqui
      while(client.connected()) {
        // if there are incoming bytes available
        // from the server, read them and print them:
        if (client.available()) {
          char c = client.read();
          Serial.print(c);
          //JsonObject& root = jsonBuffer.parse(client);
          //root.printTo(Serial);
        }
         
      }
      
     client.stop(); // parar conexão ao tratar
     state_op = 1; // trocar de operação

     Serial.println();
     Serial.println("Indo para operação 1");
      
    } else {
      // if you didn't get a connection to the server:
      Serial.println("connection failed");
      
    }
}

void operation1(){
  Serial.println();    
  Serial.println("Operação 1: Ócio");
  Serial.println("Aperte qualquer tecla e aguarde");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Aperta qualquer");
  lcd.setCursor(0,1);
  lcd.print("tecla e aguarde");
  if (getKeyOnce() != '\0') { // enquanto não houver uma interação não entra nesse fluxo
    state_op = 2;
    Serial.println();
    Serial.println("Teclado matricial excitado!");
    Serial.println();
    Serial.println("A Estacao 0 te saúda!");
    Serial.println("Indo para operação 2");
    Serial.println("Aguarde.........");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Ola! Bem vindo!");
    lcd.setCursor(0,1);
    lcd.print("Aguarde......");
    delay(2000);
  } else {
    Serial.println(" Sem interação no teclado!");
    if (checkTimeAwait(KEYBOARD_AWAIT)) { // passado 10 minutos
      state_op = 0; // vai para operação de 
      Serial.println("Indo para operação 0");
      Serial.println("Aguarde.........");
    } else {
      Serial.println("Tentando novamente...");
    }
    delay(1000); // espera um segundo para tentar de novo
  }
}

void operation2(){
  // Procura por algum cartão presente na área
  Serial.println();
  Serial.println("Aproxime seu cartão do teclado...");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Ponha o cartao");
  lcd.setCursor(0,1);
  lcd.print("perto do teclado");
  timestamp = millis();
  while ( ! mfrc522.PICC_IsNewCardPresent())// função retorna boolean que é negado pela ! no começo
  {
    /*
     * Para evitar a função de se alongar pelo resto do programa
     * quebramos aqui o ciclo dando um return.
     * E assim a loop inicia outro ciclo.
    */
    Serial.println("Cartão não detectado!");
    if (checkTimeAwait(RFID_AWAIT)) {
      state_op = 1;
      Serial.println("Indo para operação 1");
      Serial.println("Aguarde.........");
      return;  
    }
  }
  while ( ! mfrc522.PICC_ReadCardSerial())// função retorna boolean que é negado pela ! no começo
  {
    /*
     * Para evitar a função de se alongar pelo resto do programa
     * quebramos aqui o ciclo dando um return.
     * E assim a loop inicia outro ciclo.
    */
    Serial.println("Cartão não lido!");
    if (checkTimeAwait(RFID_AWAIT)) {
      state_op = 1;
      Serial.println("Indo para operação 1");
      Serial.println("Aguarde.........");
      return;  
    }
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Cartao lido!");
  delay(1500);
  uidReaded = uidRead();
  mostraUID(uidReaded); // Mostrando UID no LCD
  state_op = 3;
  delay(2000);
}
void operation3(){

//  if (uidReaded == UIDTEST) { 
//    state_op = 4;
//  }
  state_op = 4;
  
}
void operation4(){
  Serial.println();    
  Serial.println("Operação 4: Leitura da senha");
  Serial.println("Aperta qualquer botão para");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Digite sua senha");
  lcd.setCursor(0,1);
  if (execKeypad(PASSWORD_SIZE,false)) {
    if (checkPass()) {
      Serial.println();    
      Serial.println("Senha correta, fluxo terminado.");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Senha correta!");
      state_op = 5;// proxima operação        
    } else {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Senha incorreta!");
      Serial.println("Senha incorreta, fluxo terminado.");        
      state_op = 1;// proxima operação
    }
  } else {
    lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Cancelado");
    Serial.println();    
    Serial.println("Operação de teclado cancelada.");
    state_op = 1;// proxima operação    
  }
}

void operation5(){
  state_op = 6;
}
void operation6(){
  Serial.println();
  Serial.println("Operação 6 - Empréstimo");
  // ADICIONAR PARTE DO LCD
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Liberando bike");  
  lcd.setCursor(0,1);
  digitalWrite(LOCK_PORT,LOW);
  lcd.print("Trava aberta!");  
  delay(5000);
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print("Trava fechada!");  
  digitalWrite(LOCK_PORT,HIGH);
  state_op = 1;
  delay(2000);
}
void initializeMsg()
{
  Serial.println("UFERSA VAI DE BIKE - ESTACAO 0 - INICIANDO...");
  Serial.println();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("UFVDB ESTACAO 0");  
  lcd.setCursor(0,1);
  lcd.print("Iniciando...");  
}

// ethernet
void initEthernet(){
  // iniciando a conexão ethernet:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Falha na configuração usando DHCP");
    // no point in carrying on, so do nothing forevermore:
    for (;;)
      ;
  }
  // de um segundo para o shield ethernet inicializar
  delay(1000);
  // imprima seu endereço ip
  printIPAddress();
}
bool openConnection(){
  Serial.println(String("Abrindo conexão com " + serverIp));
  Serial.println();
  Serial.print(".");  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Abrindo conexao...");  
  lcd.setCursor(0,1);
  lcd.print(".");
  // tente conectar 5 vezes
  short count = 1;
  while (!client.connect(server, 3000)) {
    lcd.print(".");
    Serial.print("."); 
    ++count;    
    if (count == SERVER_TRY_CONNECTION) {
      Serial.println(String(serverIp + " não alcançado :("));  
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(serverIp);
      lcd.setCursor(0,1);
      lcd.print("nao alcancado :(");
      delay(2000);
      return false;
    }
  }
  Serial.println(String(serverIp + " conectado :)"));  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(serverIp); 
  lcd.setCursor(0,1);
  lcd.print("Conectado :)"); 
  delay(2000);
  return true;
}
// ethernet imprimir ip
void printIPAddress()
{
  Serial.print("Meu endereço IP: ");
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print(".");
  }

  Serial.println();
}

// ethernet get
void doGetAPI(String path) {
    client.println(String("GET " +path+ " HTTP/1.1"));
    client.println(String("Host: " + serverIp));
    client.println("Cache-Control: no-cache");
    client.println("Connection: close");
//    client.println("Connection: keep-alive");
    client.println();
}
// ethernet post
void doPostAPI(String path) {
    client.println(String("POST "+path+" HTTP/1.1"));
    client.println(String("Host: " + serverIp));
    client.println("Cache-Control: no-cache");
    client.println("Connection: close");
//    client.println("Connection: keep-alive");
    client.println();
}



// função para inicializar portas
void setKeyPins(){
   // inicializando portas para uso
  for (int i=0; i<4; i++){
    pinMode(rows[i], OUTPUT);      // DEFININDO SAIDA
    pinMode(columns[i], INPUT_PULLUP); // PULLUP MELHORA A LEITURA
    digitalWrite(rows[i], HIGH); // levantando todas
  }
}

// função teclado pegar tecla
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

// função para pegar qualquer input (maximo de 20)
bool execKeypad(int input_size, bool hide){
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
                    lcd.clear();
                    lcd.setCursor(0,0);
                    lcd.print("* pressionado");
                    lcd.setCursor(0,1);
                    lcd.print("Confirmando...");
                    // comparar senhas
                    return true;
                } else {
                    Serial.println();
                    Serial.println("Botão * pressionado: Cancelando (input vazio)!");
                    Serial.println();
                    lcd.clear();
                    lcd.setCursor(0,0);
                    lcd.print("* pressionado");
                    lcd.setCursor(0,1);
                    lcd.print("Cancelando...");
                    return false;
                }
                break;
            case '#':
                timestamp = millis();
                if (index > 0) {
                    lcd.clear();
                    lcd.setCursor(0,0);
                    lcd.print("# pressionado");
                    lcd.setCursor(0,1);
                    lcd.print("Resetando...");
                    Serial.println();
                    Serial.println("Botão # pressionado: Zerando input!");
                    Serial.println();
                    index = 0;
                    inputKeyBoard[index] = '\0';
                } else {
                    lcd.clear();
                    lcd.setCursor(0,0);
                    lcd.print("# pressionado");
                    lcd.setCursor(0,1);
                    lcd.print("Cancelando...");
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
                lcd.setCursor(index,1);
                if (hide) {
                  lcd.print("*");
                } else {
                  lcd.print(key);  
                }
                inputKeyBoard[index] = key;
                ++index;
                if (index > (input_size-1)) {
                    lcd.clear();
                    lcd.setCursor(0,0);
                    lcd.print("    Limite");
                    lcd.setCursor(0,1);
                    lcd.print("   Alcancado");
                    Serial.println();
                    Serial.println("Limite de input alcançado!");
                    Serial.println();
                    return true;
                }
                break;
        }
    }
}

// função teclado pegar tecla uma vez
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

// função para o RFID
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
  return id;
}

// CHECAR TEMPO DE ESPERA
bool checkTimeAwait(unsigned long await) {
  if ((millis() - timestamp) > await) {
    Serial.println();
    Serial.println(String("Tempo de espera excedido (" + await  +')'));
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Tempo de espera");
    lcd.setCursor(0,1);
    lcd.print("   excedido");
    delay(2000);
    timestamp = millis();
    return true;
  } else {
    return false;
  }
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
}

