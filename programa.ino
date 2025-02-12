//Definição dos pinos
#define pinBotaoVerde 2
#define pinBotaoAmarelo 3
#define pinBotaoVermelho 4
#define pinBotaoBranco 5
#define pinLedVerde 11
#define pinLedAmarelo 10
#define pinLedVermelho 9
#define pinLedBranco 8
#define pinBuzzer 13

#define indefinido -1
#define tamanhoSequencia 4


//Declaração variáveis
int sequenciaLuzes[tamanhoSequencia]; //Array tamanho 4: inicialmente serão 4 rodadas
int rodada = 0;
int leds_respondidos = 0;

enum estados { //Enumeração de estados do jogo
  PRONTO_PARA_PROX_RODADA, //Item 0
  USUARIO_RESPONDENDO, //Item 1
  JOGO_FINALIZADO_SUCESSO, //Item 2
  JOGO_FINALIZADO_FALHA //Item 3
};


void setup() {
  Serial.begin(9600);
  pinMode(pinBotaoVerde, INPUT);
  pinMode(pinBotaoAmarelo, INPUT);
  pinMode(pinBotaoVermelho, INPUT);
  pinMode(pinBotaoBranco, INPUT);
  pinMode(pinLedVerde, OUTPUT);
  pinMode(pinLedAmarelo, OUTPUT);
  pinMode(pinLedVermelho, OUTPUT);
  pinMode(pinLedBranco, OUTPUT);
  pinMode(pinBuzzer, OUTPUT);
  
  iniciaJogo();
}


void loop() {
  switch (estadoAtual()) {
    case PRONTO_PARA_PROX_RODADA:
      Serial.println("Pronto para proxima rodada");
      preparaNovaRodada();
      break;
    case USUARIO_RESPONDENDO:
      Serial.println("Usuario respondendo");
      processaRespostaJogador();
      break;
    case JOGO_FINALIZADO_SUCESSO:
      ganhou();
      Serial.println("Jogo finalizado com sucesso");
      break;
    case JOGO_FINALIZADO_FALHA:
      perdeu();
      Serial.println("Jogo finalizado com falha");
      break;
  }
  delay(1000);
}


//----------------------------------------------------------

//FUNÇÃO QUE PREPARA NOVA RODADA
void preparaNovaRodada() {
  rodada++;
  leds_respondidos = 0;
  if (rodada <= tamanhoSequencia) {
    tocaLedsRodada();
  }
}

//FUNÇÃO QUE PROCESSA A RESPOSTA DO JOGADOR
void processaRespostaJogador() {
  int resposta = checaRespostaJogador();
  if (resposta == indefinido) {
    return;
  }
  if (resposta == sequenciaLuzes[leds_respondidos]) {
    leds_respondidos++;
  } else { //Respondeu errado
    Serial.println("Resposta errada!");
    rodada = tamanhoSequencia + 2;
  }
}

//FUNÇÃO QUE CHECA O ESTADO ATUAL DO JOGO
int estadoAtual() {
  //Computar estado atual
  if (rodada <= tamanhoSequencia) {
    if (leds_respondidos == rodada) {
      return PRONTO_PARA_PROX_RODADA;
    } else {
      return USUARIO_RESPONDENDO;
    }
  } else if (rodada == tamanhoSequencia + 1) {
    return JOGO_FINALIZADO_SUCESSO;
  } else {
    return JOGO_FINALIZADO_FALHA;
  }
}

//FUNÇÃO QUE TOCA A SEQUÊNCIA DE LEDS CORRESPONDENTES DA RODADA
void tocaLedsRodada() {
  for (int i = 0; i < rodada ; i = i + 1) {
    piscaLed(sequenciaLuzes[i], 500);
  }
}

//FUNÇÃO CHECA RESPOSTA DO JOGADOR
int checaRespostaJogador() {
  if (digitalRead(pinBotaoVerde) == HIGH) {
    piscaLed(pinLedVerde, 500);
    return pinLedVerde;
  }
  if (digitalRead(pinBotaoAmarelo) == HIGH) {
    piscaLed(pinLedAmarelo, 500);
    return pinLedAmarelo;
  }
  if (digitalRead(pinBotaoVermelho) == HIGH) {
    piscaLed(pinLedVermelho, 500);
    return pinLedVermelho;
  }
  if (digitalRead(pinBotaoBranco) == HIGH) {
    piscaLed(pinLedBranco, 500);
    return pinLedBranco;
  }
  //Condição de retorno indefinido: caso o jogador ainda não tenha apertado o botão
  return indefinido;
}

//FUNÇÃO USUÁRIO PERDEU
void perdeu() {
  tone(pinBuzzer, 200, 200);
  piscaTodosSimultaneo(700);
  noTone(pinBuzzer);
  perdeu();
}

//FUNÇÃO USUÁRIO GANHOU
void ganhou() {
  piscaLed(pinLedVerde, 500);
  piscaLed(pinLedAmarelo, 500);
  piscaLed(pinLedVermelho, 500);
  piscaLed(pinLedBranco, 500);
  piscaTodosSimultaneo(300);
  ganhou();
}

void piscaLed(int pinoLed, int intervalo) { //INDIVIDUAL
  tocaSom(2000);
  digitalWrite(pinoLed, HIGH);
  delay(intervalo);
  digitalWrite(pinoLed, LOW);
  delay(intervalo);
}

void piscaTodosSimultaneo(int intervalo) { //COLETIVO SIMULTÂNEO
  tocaSom(2000);
  digitalWrite(pinLedVerde, HIGH);
  digitalWrite(pinLedAmarelo, HIGH);
  digitalWrite(pinLedVermelho, HIGH);
  digitalWrite(pinLedBranco, HIGH);
  delay(intervalo);
  digitalWrite(pinLedVerde, LOW);
  digitalWrite(pinLedAmarelo, LOW);
  digitalWrite(pinLedVermelho, LOW);
  digitalWrite(pinLedBranco, LOW);
  delay(intervalo);
}

void piscaTodosIndividualmente(int intervalo) { //COLETIVO EM PARTES
  piscaLed(pinLedVerde, intervalo);
  piscaLed(pinLedAmarelo, intervalo);
  piscaLed(pinLedVermelho, intervalo);
  piscaLed(pinLedBranco, intervalo);

  piscaLed(pinLedBranco, intervalo);
  piscaLed(pinLedVermelho, intervalo);
  piscaLed(pinLedAmarelo, intervalo);
  piscaLed(pinLedVerde, intervalo);
}

void iniciaJogo() {
  piscaTodosIndividualmente(100);
  piscaTodosSimultaneo(400);
  delay(800);
  int jogo = analogRead(A0); //Faz leitura do pino analógico A0 - possui variações
  randomSeed(jogo);
  //Preenchendo vetor sequenciaLuzes
  for (int i = 0; i < tamanhoSequencia; i = i + 1) {
    sequenciaLuzes[i] = sorteiaCor();
  }
}

int sorteiaCor() {
  return random(pinLedBranco, pinLedVerde + 1); //8-11
}

int tocaSom(int frequencia){
  tone(pinBuzzer, frequencia, 100);
}