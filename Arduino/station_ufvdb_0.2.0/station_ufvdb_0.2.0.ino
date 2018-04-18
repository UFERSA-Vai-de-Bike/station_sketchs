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
IPAddress server(SERVER_1,SERVER_2,SERVER_3,SERVER_4); //INICIALIZANDO IP DO SERVIDOR
EthernetClient client;
String serverIp = String(String(SERVER_1) + '.' + String(SERVER_2) + '.' + String(SERVER_3) + '.' + String(SERVER_4));

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

  delay(500);
  
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
  Serial.println(F("Conectando..."));
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("Conectando..."));
  lcd.setCursor(0,1);
  lcd.print(F("Aguarde"));
    // if you get a connection, report back via serial:
    if (openConnection()) {
      // Make a HTTP request:
      if (manageRequest(F("GET"),F("/api"))){
       // tratar conexão aqui
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(F("GET API 200 OK"));
        lcd.setCursor(0,1);
        while(client.connected()) {
          // if there are incoming bytes available
          // from the server, read them and print them:
          if (client.available()) {
            char c = client.read();
            Serial.print(c);
            lcd.print(c);
            //JsonObject& root = jsonBuffer.parse(client);
            //root.printTo(Serial);
          }
           
        }
        state_op = 1; // trocar de operação
        Serial.println();
        Serial.println(F("Indo para operação 1"));
        delay(1500);
        
      } else {
        Serial.println();
        Serial.println(F("Falha no GET principal!"));
      }

      
     client.stop(); // parar conexão ao tratar
      
    } else {
      // if you didn't get a connection to the server:
      Serial.println(F("Falha de conexão"));
      
    }
}

void operation1(){
  Serial.println();    
  Serial.println(F("Operação 1: Ócio"));
  Serial.println(F("Aperte qualquer tecla e aguarde"));
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("Aperte qualquer"));
  lcd.setCursor(0,1);
  lcd.print(F("tecla e aguarde"));
  timestamp = millis();
  while(getKeyOnce() == '\0') {
    Serial.println(F(" Sem interação no teclado!"));
    if (checkTimeAwait(KEYBOARD_AWAIT)) { // passado 10 minutos
      state_op = 0; // vai para operação de 
      Serial.println(F("Indo para operação 0"));
      Serial.println(F("Aguarde........."));
      delay(1000);
      return;
    } 
    Serial.println(F("Tentando novamente..."));
    delay(700); // espera um pouco para tentar de novo
  }
  state_op = 2;

  Serial.println();
  Serial.println(F("Teclado matricial excitado!"));
  Serial.println();
  Serial.println(F("A Estacao 0 te saúda!"));
  Serial.println(F("Indo para operação 2"));
  Serial.println(F("Aguarde........."));
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("Ola! Bem vindo!"));
  lcd.setCursor(0,1);
  lcd.print(F("Aguarde......"));
  delay(2000);
}

void operation2(){
  // Procura por algum cartão presente na área
  Serial.println();
  Serial.println(F("Aproxime seu cartão do teclado..."));
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("Ponha o cartao"));
  lcd.setCursor(0,1);
  lcd.print(F("perto do teclado"));
  timestamp = millis();
  while ( ! mfrc522.PICC_IsNewCardPresent())// função retorna boolean que é negado pela ! no começo
  {
    /*
     * Para evitar a função de se alongar pelo resto do programa
     * quebramos aqui o ciclo dando um return.
     * E assim a loop inicia outro ciclo.
    */
    Serial.println(F("Cartão não detectado!"));
    if (checkTimeAwait(RFID_AWAIT)) {
      state_op = 1;
      Serial.println(F("Indo para operação 1"));
      Serial.println(F("Aguarde........."));
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
    Serial.println(F("Cartão não lido!"));
    if (checkTimeAwait(RFID_AWAIT)) {
      state_op = 1;
      Serial.println(F("Indo para operação 1"));
      Serial.println(F("Aguarde........."));
      return;  
    }
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("Cartao lido!"));
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
  Serial.println(F("Operação 4: Leitura da senha"));
  Serial.println(F("Use o teclado numérico"));
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("Digite sua senha"));
  lcd.setCursor(0,1);
  if (execKeypad(F("Digite sua senha"),PASSWORD_SIZE,true)) {
    if (checkPass()) {
      Serial.println();    
      Serial.println(F("Senha correta, fluxo terminado."));
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(F("Senha correta!"));
      state_op = 5;// proxima operação        
    } else {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(F("Senha incorreta!"));
      Serial.println(F("Senha incorreta, fluxo terminado."));        
      state_op = 1;// proxima operação
    }
  } else {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(F("Cancelado"));
    Serial.println();    
    Serial.println(F("Operação de teclado cancelada."));
    state_op = 1;// proxima operação    
  }
}

void operation5(){
  state_op = 6;
}
void operation6(){
  Serial.println();
  Serial.println(F("Operação 6 - Empréstimo"));
  // ADICIONAR PARTE DO LCD
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("Liberando bike"));  
  lcd.setCursor(0,1);
  digitalWrite(LOCK_PORT,LOW);
  lcd.print(F("Trava aberta!"));  
  delay(5000);
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print(F("Trava fechada!"));  
  digitalWrite(LOCK_PORT,HIGH);
  state_op = 1;
  delay(2000);
}
void initializeMsg()
{
  Serial.println(F("UFERSA VAI DE BIKE - ESTACAO 0 - INICIANDO..."));
  Serial.println();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("UFVDB ESTACAO 0"));  
  lcd.setCursor(0,1);
  lcd.print(F("Iniciando..."));
  delay(1000); 
}

// ethernet
void initEthernet(){
  // iniciando a conexão ethernet:
  if (Ethernet.begin(mac) == 0) {
    Serial.println(F("Falha na configuração usando DHCP"));
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
  Serial.print(F("."));  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("Abrindo conexao..."));  
  lcd.setCursor(0,1);
  lcd.print(F("."));
  // tente conectar 5 vezes
  short count = 1;
  while (!client.connect(server, 3000)) {
    lcd.print(F("."));
    Serial.print(F(".")); 
    ++count;    
    if (count == SERVER_TRY_CONNECTION) {
      Serial.println(String(serverIp + " não alcançado :("));  
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(serverIp);
      lcd.setCursor(0,1);
      lcd.print(F("nao alcancado :("));
      delay(2000);
      return false;
    }
  }
  Serial.println(String(serverIp + " conectado :)"));  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(serverIp); 
  lcd.setCursor(0,1);
  lcd.print(F("Conectado :)")); 
  delay(2000);
  return true;
}
// ethernet imprimir ip
void printIPAddress()
{
  Serial.print(F("Meu endereço IP: "));
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print(F("."));
  }

  Serial.println();
}

//ethernet utility to do request
bool manageRequest(String type, String path) {
  client.println(String(type + ' ' + path + " HTTP/1.1"));
  client.println(String("Host: " + serverIp));
  client.println(F("Cache-Control: no-cache"));
  client.println(F("Connection: close"));
//client.println("Connection: keep-alive");

  if (client.println() == 0) {
    Serial.println(F("Falha no envio da requisição"));
    return false;
  }
   // Check HTTP status
  char status[32] = {0};
  client.readBytesUntil('\r', status, sizeof(status));
  if (strcmp(status, "HTTP/1.1 200 OK") != 0) {
    Serial.print(F("Resposta inesperada: "));
    Serial.println(status);
    return false;
  }

  // Skip HTTP headers
  char endOfHeaders[] = "\r\n\r\n";
  if (!client.find(endOfHeaders)) {
    Serial.println(F("Resposta inválida!"));
    return false;
  }

  return true;
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
bool execKeypad(String msg,int input_size, bool hide){
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
                    Serial.println(F("Botão * pressionado: Confirmar input!"));
                    Serial.println();
                    lcd.clear();
                    lcd.setCursor(0,0);
                    lcd.print(F("* pressionado"));
                    lcd.setCursor(0,1);
                    lcd.print(F("Confirmando..."));
                    // comparar senhas
                    return true;
                } else {
                    Serial.println();
                    Serial.println(F("Botão * pressionado: Cancelando (input vazio)!"));
                    Serial.println();
                    lcd.clear();
                    lcd.setCursor(0,0);
                    lcd.print(F("* pressionado"));
                    lcd.setCursor(0,1);
                    lcd.print(F("Cancelando..."));
                    return false;
                }
                break;
            case '#':
                timestamp = millis();
                if (index > 0) {
                    lcd.clear();
                    lcd.setCursor(0,0);
                    lcd.print(F("# pressionado"));
                    lcd.setCursor(0,1);
                    lcd.print(F("Resetando..."));
                    Serial.println();
                    Serial.println(F("Botão # pressionado: Zerando input!"));
                    Serial.println();
                    index = 0;
                    inputKeyBoard[index] = '\0';
                    delay(2000);
                    lcd.clear();
                    lcd.setCursor(0,0);
                    lcd.print(msg);
                    lcd.setCursor(0,1);

                } else {
                    lcd.clear();
                    lcd.setCursor(0,0);
                    lcd.print(F("# pressionado"));
                    lcd.setCursor(0,1);
                    lcd.print(F("Cancelando..."));
                    Serial.println();
                    Serial.println(F("Botão # pressionado: Cancelando (input vazio)!"));
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
                  lcd.print(F("*"));
                } else {
                  lcd.print(key);  
                }
                inputKeyBoard[index] = key;
                ++index;
                if (index > (input_size-1)) {
                    lcd.clear();
                    lcd.setCursor(0,0);
                    lcd.print(F("    Limite"));
                    lcd.setCursor(0,1);
                    lcd.print(F("   Alcancado"));
                    Serial.println();
                    Serial.println(F("Limite de input alcançado!"));
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
    lcd.print(F("Tempo de espera"));
    lcd.setCursor(0,1);
    lcd.print(F("   excedido"));
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
  lcd.print(F("UID da tag"));
  Serial.println(F("UID da tag"));
  lcd.setCursor(0,1);
  lcd.print(id);
  Serial.println(id);
  delay(3000);
}