#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SI5351.h>

// Instancia del SI5351
Adafruit_SI5351 clockgen = Adafruit_SI5351();

// Variables para el control de frecuencia
unsigned long freq_clk0 = 1000000ULL; // 1 MHz por defecto
unsigned long freq_clk1 = 2000000ULL; // 2 MHz por defecto
unsigned long freq_clk2 = 3000000ULL; // 3 MHz por defecto

// Declaraciones de funciones
void processSerialCommand();
void setClockFrequency(int clock, unsigned long frequency);
void showStatus();
void resetToDefaults();

void setup() {
  // Inicializar comunicación serie
  Serial.begin(115200);
  Serial.println("Iniciando control SI5351...");
  
  // Inicializar I2C
  Wire.begin();
  
  // Inicializar SI5351
  if (!clockgen.begin()) {
    Serial.println("Error: No se encontró el SI5351!");
    while(1) {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(500);
      digitalWrite(LED_BUILTIN, LOW);
      delay(500);
    }
  }
  
  Serial.println("SI5351 inicializado correctamente!");
  
  // Configurar LED integrado para indicación de estado
  pinMode(LED_BUILTIN, OUTPUT);
  
  // Configurar frecuencias iniciales
  clockgen.setupPLL(SI5351_PLL_A, 36, 0, 1);  // PLL A: 36 * 25MHz = 900MHz
  clockgen.setupPLL(SI5351_PLL_B, 32, 0, 1);  // PLL B: 32 * 25MHz = 800MHz
  
  // Configurar salidas
  clockgen.setupMultisynth(0, SI5351_PLL_A, 900, 0, 1); // CLK0: 900MHz/900 = 1MHz
  clockgen.setupMultisynth(1, SI5351_PLL_A, 450, 0, 1); // CLK1: 900MHz/450 = 2MHz  
  clockgen.setupMultisynth(2, SI5351_PLL_A, 300, 0, 1); // CLK2: 900MHz/300 = 3MHz
  
  // Habilitar salidas
  clockgen.enableOutputs(true);
  
  Serial.println("Configuración inicial completada:");
  Serial.print("CLK0: "); Serial.print(freq_clk0); Serial.println(" Hz");
  Serial.print("CLK1: "); Serial.print(freq_clk1); Serial.println(" Hz");
  Serial.print("CLK2: "); Serial.print(freq_clk2); Serial.println(" Hz");
  Serial.println("\nComandos disponibles:");
  Serial.println("0 <freq> - Configurar CLK0 (Hz)");
  Serial.println("1 <freq> - Configurar CLK1 (Hz)");
  Serial.println("2 <freq> - Configurar CLK2 (Hz)");
  Serial.println("s - Mostrar estado");
  Serial.println("r - Reset a valores por defecto");
}

void loop() {
  // Indicador de funcionamiento
  static unsigned long lastBlink = 0;
  static bool ledState = false;
  
  if (millis() - lastBlink > 1000) {
    ledState = !ledState;
    digitalWrite(LED_BUILTIN, ledState);
    lastBlink = millis();
  }
  
  // Procesar comandos serie
  if (Serial.available()) {
    processSerialCommand();
  }
  
  delay(10); // Pequeña pausa para evitar saturar el procesador
}

void processSerialCommand() {
  String command = Serial.readStringUntil('\n');
  command.trim();
  
  if (command.length() == 0) return;
  
  char cmd = command.charAt(0);
  
  switch (cmd) {
    case '0':
    case '1':
    case '2': {
      int clk = cmd - '0';
      unsigned long newFreq = command.substring(2).toInt();
      
      if (newFreq >= 8000 && newFreq <= 160000000) { // Rango válido del SI5351
        setClockFrequency(clk, newFreq);
      } else {
        Serial.println("Error: Frecuencia fuera de rango (8kHz - 160MHz)");
      }
      break;
    }
    
    case 's':
    case 'S':
      showStatus();
      break;
      
    case 'r':
    case 'R':
      resetToDefaults();
      break;
      
    default:
      Serial.println("Comando no reconocido");
      Serial.println("Comandos: 0/1/2 <freq>, s (status), r (reset)");
      break;
  }
}

void setClockFrequency(int clock, unsigned long frequency) {
  if (clock < 0 || clock > 2) {
    Serial.println("Error: Clock inválido (0-2)");
    return;
  }
  
  // Configurar frecuencia usando PLL A
  uint32_t pll_freq = 900000000UL; // 900 MHz
  uint32_t divider = pll_freq / frequency;
  
  // Verificar que el divisor esté en rango válido (6-1800)
  if (divider < 6 || divider > 1800) {
    // Usar PLL B con frecuencia diferente si es necesario
    pll_freq = 800000000UL; // 800 MHz
    divider = pll_freq / frequency;
    
    if (divider < 6 || divider > 1800) {
      Serial.println("Error: Frecuencia fuera de rango para los PLLs");
      return;
    }
    clockgen.setupMultisynth(clock, SI5351_PLL_B, divider, 0, 1);
  } else {
    clockgen.setupMultisynth(clock, SI5351_PLL_A, divider, 0, 1);
  }
  
  // Actualizar la variable correspondiente
  switch (clock) {
    case 0: freq_clk0 = frequency; break;
    case 1: freq_clk1 = frequency; break;
    case 2: freq_clk2 = frequency; break;
  }
  
  Serial.print("CLK");
  Serial.print(clock);
  Serial.print(" configurado a ");
  Serial.print(frequency);
  Serial.println(" Hz");
}

void showStatus() {
  Serial.println("\n=== Estado del SI5351 ===");
  Serial.print("CLK0: "); Serial.print(freq_clk0); Serial.println(" Hz");
  Serial.print("CLK1: "); Serial.print(freq_clk1); Serial.println(" Hz");
  Serial.print("CLK2: "); Serial.print(freq_clk2); Serial.println(" Hz");
  
  Serial.println("Sistema: OK");
  Serial.println("Salidas: Habilitadas");
  
  Serial.println("=========================\n");
}

void resetToDefaults() {
  freq_clk0 = 1000000ULL; // 1 MHz
  freq_clk1 = 2000000ULL; // 2 MHz
  freq_clk2 = 3000000ULL; // 3 MHz
  
  // Reconfigurar las salidas con valores por defecto
  clockgen.setupMultisynth(0, SI5351_PLL_A, 900, 0, 1); // CLK0: 1MHz
  clockgen.setupMultisynth(1, SI5351_PLL_A, 450, 0, 1); // CLK1: 2MHz
  clockgen.setupMultisynth(2, SI5351_PLL_A, 300, 0, 1); // CLK2: 3MHz
  
  Serial.println("Valores restaurados a los por defecto");
  showStatus();
}
