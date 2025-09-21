#pragma once
// Config.h - Pines y constantes globales de la aplicación

// Pines I2C
constexpr int PIN_I2C_SDA = 21;
constexpr int PIN_I2C_SCL = 22;

// Pines Encoder
constexpr int PIN_ENC_CLK = 34; // requiere pull-up externo
constexpr int PIN_ENC_DT  = 35; // requiere pull-up externo
constexpr int PIN_ENC_BTN = 32; // con pull-up interno

// Rango de valor
constexpr int VALUE_MIN = 0;
constexpr int VALUE_MAX = 9999;

// ================= Encoder configuration =================
// Selecciona implementación:
//  - Definir USE_SIMPLE_ENCODER para modo simple optimizado KY-040 con DOBLE interrupción (CLK y DT),
//    sin aceleración y conteo 1:1 por detent (usa tabla mínima de transición interna).
//  - Comentar para usar la implementación avanzada (RAW / DETENT) ya existente (más opciones).
#define USE_SIMPLE_ENCODER 1

#ifndef USE_SIMPLE_ENCODER
	// ---- Implementación avanzada (pre-existente) ----
	enum class EncoderCountMode { RAW_SUBSTEPS, DETENT_ONLY };
	constexpr EncoderCountMode ENCODER_COUNT_MODE = EncoderCountMode::DETENT_ONLY;
	constexpr int SUBSTEPS_PER_NOTCH = 4;          // sólo usado en RAW_SUBSTEPS
	constexpr bool ENABLE_ACCEL = false;           // desactivado para 1:1
	constexpr uint32_t FAST_THRESHOLD_US = 1800;   // umbral aceleración
	constexpr int ACCEL_FACTOR = 2;                // multiplicador
#else
	// ---- Implementación simple KY-040 (dual ISR) ----
	// Invertir dirección si gira "al revés" respecto a lo esperado.
	constexpr bool ENCODER_REVERSE = false;
	// Filtro de glitches (tiempo mínimo entre interrupciones procesadas) en microsegundos.
	// Ajusta 100..600 según ruido mecánico. Si pierdes pasos, sube un poco; si rebota, bájalo.
	constexpr uint32_t ENCODER_DEBOUNCE_US = 150;
#endif

// Botón
auto constexpr BUTTON_DEBOUNCE_MS = 25u;
auto constexpr BUTTON_LONGPRESS_MS = 800u;