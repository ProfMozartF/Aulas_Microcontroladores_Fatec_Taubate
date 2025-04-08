// Definição dos coeficientes do filtro IIR
float alpha = 0.05;  // Fator de suavização (entre 0 e 1), controle da resposta do filtro
float y_ant = 0;    // Variável para armazenar o valor anterior da saída do filtro

// Definição do pino analógico
const int sensorPin = A0;  // Pino onde o sensor está conectado

void setup() {
  Serial.begin(9600);  // Inicializa a comunicação serial para visualizar os dados no monitor serial
}

void loop() {
  // Leitura analógica do sensor
  int leituraAnalogica = analogRead(sensorPin);
  
  // Converte a leitura analógica para um valor de tensão (0 a 5V)
  float entrada = leituraAnalogica;
  int entrada1= entrada;
  // Implementação do filtro IIR passa-baixa
  // Fórmula: y[n] = alpha * x[n] + (1 - alpha) * y[n-1]
  float y_atual = alpha * entrada + (1 - alpha) * y_ant;
  int entrada_filt = y_atual;
  // Atualiza o valor anterior da saída
  y_ant = y_atual;

  // Exibe o valor filtrado no monitor serial
 // Serial.print("Entrada: ");
  Serial.print(entrada1);
  Serial.print(",");
  Serial.println(entrada_filt);
  //Serial.println(" V");

  delay(100);  // Atraso de 100ms para facilitar a visualização dos dados
}

