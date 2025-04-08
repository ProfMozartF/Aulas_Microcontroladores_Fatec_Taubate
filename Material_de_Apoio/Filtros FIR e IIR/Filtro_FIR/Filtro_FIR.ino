// Definir o número de coeficientes do filtro FIR
#define NUM_TAPS 5

// Definir os coeficientes do filtro FIR (ajuste conforme necessário)
float coef[NUM_TAPS] = {0.2, 0.2, 0.2, 0.2, 0.2};  // Exemplo: média simples

// Array para armazenar os valores de entrada anteriores
float buffer[NUM_TAPS] = {0};

// Pino de entrada e saída
int pinEntrada = A0;  // Sinal analógico
int pinSaida = 9;     // Saída PWM ou outra função desejada

void setup() {
  Serial.begin(9600);
}

void loop() {
  // Ler o valor analógico de entrada
  int leituraAnalogica = analogRead(pinEntrada);

  // Converter a leitura para uma escala de 0 a 1
  float entrada = leituraAnalogica / 1023.0;

  // Aplicar o filtro FIR
  float resultado = aplicarFiltroFIR(entrada);
  int analogFilt = resultado*1023;

  // Converter o resultado de volta para 0-255 para uma saída PWM (se necessário)
  int saidaPWM = int(resultado * 255);

  // Enviar o valor filtrado à saída PWM
  analogWrite(pinSaida, saidaPWM);

  // Exibir o resultado no monitor serial
 // Serial.print("Entrada: ");
  Serial.print(leituraAnalogica);
  Serial.print(",");
  Serial.println(analogFilt);
  //Serial.print(" | Saída filtrada: ");
 // Serial.println(analogFilt);

  // Pequeno atraso antes da próxima leitura
  delay(100);
}

float aplicarFiltroFIR(float entrada) {
  // Deslocar os valores anteriores no buffer
  for (int i = NUM_TAPS - 1; i > 0; i--) {
    buffer[i] = buffer[i - 1];
  }

  // Inserir o novo valor de entrada no buffer
  buffer[0] = entrada;

  // Aplicar o filtro FIR
  float resultado = 0;
  for (int i = 0; i < NUM_TAPS; i++) {
    resultado += buffer[i] * coef[i];
  }

  return resultado;
}
