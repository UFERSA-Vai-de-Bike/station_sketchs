/*
Estação do UFERSA VAI DE BIKE 

Caso de uso: Empréstimo simples.

Periféricos simples:
    - Teclado matricial (Entrada)
    - Visor LCD (Saída)
    - Relé (Saída)

Periféricos Complexos:
    - Leitor rfid (Entrada)
    - ETHERNET Shield (Entrada/Saída)

*/


short state_op = 0;

void setup() {

    
}
void loop() {

    /* 
        Eis a descrição de todas as operações envolvendo um empréstimo simples.

        Atente-se:  A passagem das operações devem ser largamente debugadas com Serial.println()
                    ou lcd.print() no caso de visores LCD.

        Operação 0 - Teste de conexão.
            - Aviso de que irá testar a conexão com a UFERSA Vai de Bike API.
            - Testa conexão com o get padrão da API.
                - Get com sucesso vai para a operação 1
                - Get sem sucesso, alerta (Serial.prinln("Sem conexão com a UFERSA Vai de Bike - API"))
                - Tenta novamente depois de alguns minutos (Opcional).
        
        Operação 1 - Ócio
            - Aviso para que toquem em qualquer tecla.
            - Esperando por qualquer interação com o teclado matricial.
            - Se houve interação vai para a operação 2.
            - Se não houver interação depois de alguns minutos vai para a operação 0. (Opcional)

        Operação 2 - Leitura do cartão RFID

            (Como pular: coloque no código um caso que sempre retorne positivo para a leitura de ID e o valor da mesma)

            - Aviso para que aproximem o cartão RFID da área do leitor.
            - Tentar realizar a leitura de algum cartão por alguns segundos.
                - Se houve leitura
                    - Avisa que o cartão foi lido
                    - Vai para a operação 3
                - Se não houve leitura por alguns segundos
                    - Avise sobre a falha da leitura e volte para a operação 1.

        Operação 3 - Checar situação do usuário com a Central

            (Pular: coloque no código um caso que sempre retorne positivo para pegar bicicletas)

            - Aviso que irá checar a situação do usuário perante a API
            - Realiza a requisição para checar a situação do usuário passando o ID do cartão como parâmetro.
            - Independente do proximo passo mostre o resultado da requisição, será um código que indicara se:
                - Usuário está ativo no sistema e pode pegar bicicletas.
                - Capture informações necessárias como ID do usuário.
                - Segure a mensagem por pouco tempo e vá para a operação 4.
                    OU
                - Usuário está ativo no sistema, no entanto não pode pegar bicicletas por já ter uma em seu nome.
                - Segure a mensagem por um tempo e vá para operação 1
                    OU
                - Usuário não está ativo no sistema.
                - Segure a mensagem por um tempo e vá para operação 1

        Operação 4 - Leitura da senha do usuário

            (Pular: coloque a senha no código)

            - Mensagem para inserir senha e instruir sobre botões para cancelar e concluir a operação.
            - Se a senha não foi iniciada ou está vazia:
                - Se o teclado não receber alguma interação dentro de um período estabelecido ir para a operação 1.
            - Depois do inicio da senha:
                - Se botão de cancelar então torne vazia a senha que está sendo construída.
                - Se botão de confirmar ir para a operação 5.
                - Se o teclado não receber interação dentro de um período estabelecido ir para operação 5.
            - Para cada interação com o teclado:
                - Se botão normal exibir a construção da senha com *'s para cada caractere.

        Operação 5 - Checar acesso do usuário com a central

            (Pular: coloque no código um caso que sempre retorne um acesso positivo)

            - Aviso ao usuário sobre a operação.
            - Realiza a requisição de acesso usando o id do usuário e a senha como parâmetro.
            - Independente do proximo passo mostre o resultado da requisição, será um código que indicara se:
                - Está tudo certo, podemos liberar uma bike ao usuário.
                - Segura a mensagem por pouco tempo e siga para a operação 6.
                    OU
                - Infelizmente aconteceu algum dos erros a seguir:
                    - A senha esta errada.
                    - A situação do usuário foi atualizada e ele não pode pegar bicicletas.
                - Segura a mensagem por algum tempo e volte para a operação 1.

        Operação 6 - Empréstimo

            - Mensagem que um empréstimo pode ser realizado.
            - Ative o relé da trava.
    */



    /*  A transição de operações acontece quando dentro de algum fluxo
        troca-se o valor de state_op para a operação desejada
    
    
    */

    switch (state_op) {
        case 0:
            // operação 0

        case 1:
            // operação 1

        case 2:
            // operação 2

        case 3:
            // operação 3

        case 4:
            // operação 4

        case 5:
            // operação 5

        case 6:
            // operação 6

        default:
            
            // sem operação, mas pode-se realizar operações iguais a estação em geral
            // Calcular horário (?)
            
            
            state_op = 0; // voltando ao rumo
            break;
    }

}
