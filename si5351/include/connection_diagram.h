/*
 * Esquema de conexión detallado
 * =============================
 * 
 * ARDUINO UNO                    TXS0108E                    SI5351 MODULE
 * +-----------+                  +----------+                +-------------+
 * |           |                  |          |                |             |
 * |       5V  |----------------->| VCCA (1) |                |             |
 * |      GND  |----------------->| GND (10) |<-------------->| GND         |
 * |           |                  |          |                |             |
 * |   A4(SDA) |----------------->| A1   (2) |                |             |
 * |   A5(SCL) |----------------->| A2   (3) |                |             |
 * |           |                  |          |                |             |
 * |      3V3  |----------------------------------+----------->| VCC (3.3V)  |
 * |           |                  |          |    |            |             |
 * +-----------+                  |    B1(19)|----+----------->| SDA         |
 *                                |    B2(18)|---------------->| SCL         |
 *                                |  VCCB(20)|<---------------+             |
 *                                |    OE(11)|<---------------+             |
 *                                +----------+                |             |
 *                                                            | CLK0 ------>|
 *                                                            | CLK1 ------>|
 *                                                            | CLK2 ------>|
 *                                                            +-------------+
 * 
 * Notas:
 * - VCCA = 5V (lado Arduino)
 * - VCCB = 3.3V (lado SI5351)
 * - OE debe conectarse a VCCB para habilitar el level shifter
 * - CLK0, CLK1, CLK2 son las salidas de frecuencia del SI5351
 */

// Definiciones de pines para referencia
#define SI5351_SDA_PIN  A4  // Pin SDA del Arduino
#define SI5351_SCL_PIN  A5  // Pin SCL del Arduino
#define LED_STATUS_PIN  13  // LED integrado del Arduino

// Direcciones I2C posibles del SI5351
#define SI5351_I2C_ADDR_DEFAULT 0x60  // Dirección por defecto
#define SI5351_I2C_ADDR_ALT     0x61  // Dirección alternativa

/*
 * Configuración de pines del TXS0108E:
 * ===================================
 * 
 * Pin 1  (VCCA) -> 5V Arduino
 * Pin 2  (A1)   -> A4 Arduino (SDA)
 * Pin 3  (A2)   -> A5 Arduino (SCL)
 * Pin 4-9 (A3-A8) -> No conectado
 * Pin 10 (GND)  -> GND común
 * Pin 11 (OE)   -> 3.3V (habilitar)
 * Pin 12-17 (B8-B3) -> No conectado
 * Pin 18 (B2)   -> SCL SI5351
 * Pin 19 (B1)   -> SDA SI5351
 * Pin 20 (VCCB) -> 3.3V SI5351
 */
