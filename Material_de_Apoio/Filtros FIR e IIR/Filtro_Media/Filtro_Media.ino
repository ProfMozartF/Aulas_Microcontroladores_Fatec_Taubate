// Definição dos coeficientes do filtro IIR
float alpha = 0.05;  // Fator de suavização (entre 0 e 1), controle da resposta do filtro
float y_ant = 0;    // Variável para armazenar o valor anterior da saída do filtro

// Definição do pino analógico
const int sensorPin = A0;  // Pino onde o sensor está conectado
//int leitura_media(int pinADC);
int leitura_media(void);
void setup() {
  Serial.begin(9600);  // Inicializa a comunicação serial para visualizar os dados no monitor serial
}

void loop() {
  // Leitura analógica do sensor
  int leituraAnalogica = analogRead(sensorPin);
  
  // Converte a leitura analógica para um valor de tensão (0 a 5V)
  int entrada = leituraAnalogica;
  int entrada_filt = MeasureADC(sensorPin);


  // Exibe o valor filtrado no monitor serial
 
  Serial.print(entrada);
  Serial.print(",");
  Serial.println(entrada_filt);
  //Serial.println(" V");

  delay(100);  // Atraso de 100ms para facilitar a visualização dos dados
}

int MeasureADC(uint8_t PinoADC)
 {
  uint8_t cont=0;
  float valor=0;
  float media =0;
    while(cont<100)
    {
      valor+=analogRead(PinoADC);
      cont+=1;
    }
   media=valor/100;
   int resultado = media;
   return resultado;
 }