#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define PIN_LDR   34
#define PIN_BOTAO 23
#define PIN_LED   18
#define LIMIAR     30

LiquidCrystal_I2C lcd(0x27, 16, 2);

bool modoPorcentagem = true;
bool estadoAnteriorBotao = HIGH;
bool ledAtivo = false;

int  lerLDR();
void controlarLED(int porcentagem);
void verificarBotao();
void atualizarDisplay(int valorBruto, int porcentagem);
void setup() {
  Serial.begin(115200);
  pinMode(PIN_BOTAO, INPUT_PULLUP);
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Iniciando...");
  delay(1500);
  lcd.clear();
}

void loop() {
  verificarBotao();
  int valorBruto =lerLDR();
  int porcentagem= map(valorBruto, 0, 4095, 100, 0);
  porcentagem  = constrain(porcentagem, 0, 100);
  controlarLED(porcentagem);
  atualizarDisplay(valorBruto, porcentagem);
  Serial.printf("Bruto: %d | Pct: %d | LED: %s\n", valorBruto, porcentagem, ledAtivo ? "SIM" : "NAO");
  delay(200);
}

int lerLDR() {
  long soma = 0;
  for (int i = 0; i < 5; i++) {
    soma += analogRead(PIN_LDR);
    delay(5);
  }
  return soma / 5;
}
void controlarLED(int porcentagem) {
  if (porcentagem < LIMIAR) {
    digitalWrite(PIN_LED, HIGH);
    ledAtivo = true;
  } else {
    digitalWrite(PIN_LED, LOW);
    ledAtivo = false;
  }
}
void verificarBotao() {
  bool estadoAtual = digitalRead(PIN_BOTAO);
  if (estadoAnteriorBotao == HIGH && estadoAtual == LOW) {
    modoPorcentagem = !modoPorcentagem;
    lcd.clear();
  }
  estadoAnteriorBotao = estadoAtual;
}

void atualizarDisplay(int valorBruto, int porcentagem) {
  lcd.setCursor(0, 0);
  if (modoPorcentagem) {
    lcd.print("Luz: ");
    lcd.print(porcentagem);
    lcd.print("%          ");
  } else {
    lcd.print("ADC: ");
    lcd.print(valorBruto);
    lcd.print("       ");
  }
  lcd.setCursor(0, 1);
  if (ledAtivo) {
    lcd.print("Lampada: Ligada ");
  } else {
    lcd.print("Lampada: Deslig.");
  }
}