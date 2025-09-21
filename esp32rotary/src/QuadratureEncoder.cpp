// Conditional implementation: simple single-interrupt (KY-040) or advanced
#include "QuadratureEncoder.h"

static QuadratureEncoder *s_instance = nullptr;

#ifdef USE_SIMPLE_ENCODER

// Implementación simple con doble interrupción y máquina de estados mínima.
// Garantiza 1 incremento/decremento por detent estable ignorando rebotes breves.

void QuadratureEncoder::begin() {
    s_instance = this;
    pinMode(PIN_ENC_CLK, INPUT);
    pinMode(PIN_ENC_DT,  INPUT);
    int a = digitalRead(PIN_ENC_CLK);
    int b = digitalRead(PIN_ENC_DT);
    _lastState = (a << 1) | b; // 0..3
    attachInterrupt(digitalPinToInterrupt(PIN_ENC_CLK), encoderIsrCLK, CHANGE);
    attachInterrupt(digitalPinToInterrupt(PIN_ENC_DT),  encoderIsrDT,  CHANGE);
}

void QuadratureEncoder::setValue(int v) {
    if (v < VALUE_MIN) v = VALUE_MIN;
    if (v > VALUE_MAX) v = VALUE_MAX;
    _value = v;
}

// Tabla de transiciones válidas (Gray). Índice = oldState*4 + newState.
// Valores: 0 = sin movimiento / rebote, +1 = avance, -1 = retroceso.
static const int8_t SIMPLE_QTABLE[16] = {
    // new: 00 01 10 11
       0, +1, -1,  0,   // old 00
      -1,  0,  0, +1,   // old 01
      +1,  0,  0, -1,   // old 10
       0, -1, +1,  0    // old 11
};

// Estados estables (detents) típicos: 00 y 11. Contamos sólo al llegar a uno distinto del anterior estable.
static inline bool isStable(uint8_t s) { return (s == 0x0) || (s == 0x3); }

void IRAM_ATTR processSimpleISR() {
    uint32_t now = micros();
    static uint32_t lastUs = 0;
    if (now - lastUs < ENCODER_DEBOUNCE_US) return; // filtro tiempo mínimo global
    lastUs = now;
    if (!s_instance) return;

    int a = digitalRead(PIN_ENC_CLK);
    int b = digitalRead(PIN_ENC_DT);
    uint8_t newState = (a << 1) | b;
    uint8_t oldState = s_instance->_lastState & 0x3;
    if (newState == oldState) return;

    int8_t step = SIMPLE_QTABLE[(oldState << 2) | newState];
    if (step != 0) {
        // Acumular medio-pasos, sólo confirmar al llegar a estado estable distinto
        s_instance->_subAccum += step; // _subAccum actúa como acumulador +-2
        if (isStable(newState)) {
            int detentDir = 0;
            if (s_instance->_subAccum >= 2) detentDir = +1;
            else if (s_instance->_subAccum <= -2) detentDir = -1;
            if (detentDir != 0) {
                s_instance->_subAccum = 0;
                if (ENCODER_REVERSE) detentDir = -detentDir;
                int nv = s_instance->_value + detentDir;
                if (nv < VALUE_MIN) nv = VALUE_MIN;
                if (nv > VALUE_MAX) nv = VALUE_MAX;
                s_instance->_value = nv;
                if (s_instance->_cb) s_instance->_cb(detentDir, false);
            }
        }
    }
    s_instance->_lastState = newState;
}

void IRAM_ATTR encoderIsrCLK() { processSimpleISR(); }
void IRAM_ATTR encoderIsrDT()  { processSimpleISR(); }

#else // Advanced implementation retained

// Tabla de transición (oldA oldB newA newB)
static const int8_t QTABLE[16] = {
  0, +1, -1, 0,
 -1, 0,  0,+1,
 +1, 0,  0,-1,
  0,-1, +1, 0
};

void QuadratureEncoder::begin() {
    s_instance = this;
    pinMode(PIN_ENC_CLK, INPUT);
    pinMode(PIN_ENC_DT,  INPUT);
    int a = digitalRead(PIN_ENC_CLK);
    int b = digitalRead(PIN_ENC_DT);
    _lastState = (a << 1) | b;
    attachInterrupt(digitalPinToInterrupt(PIN_ENC_CLK), encoderIsrCLK, CHANGE);
    attachInterrupt(digitalPinToInterrupt(PIN_ENC_DT),  encoderIsrDT, CHANGE);
}

void QuadratureEncoder::setValue(int v) {
    if (v < VALUE_MIN) v = VALUE_MIN;
    if (v > VALUE_MAX) v = VALUE_MAX;
    _value = v;
}

void QuadratureEncoder::handleISR() {
    int a = digitalRead(PIN_ENC_CLK);
    int b = digitalRead(PIN_ENC_DT);

    if (ENCODER_COUNT_MODE == EncoderCountMode::DETENT_ONLY) {
        uint8_t state = (a << 1) | b; // 0..3
        uint8_t prev = _lastState & 0x03;
        if ((state == 0x0 || state == 0x3) && (prev != state)) {
            int dir = 0;
            if (state == 0x0) {
                if      (prev == 0x1) dir = -1;
                else if (prev == 0x2) dir = +1;
            } else {
                if      (prev == 0x1) dir = +1;
                else if (prev == 0x2) dir = -1;
            }
            if (dir != 0) {
                int nv = _value + dir;
                if (nv < VALUE_MIN) nv = VALUE_MIN;
                if (nv > VALUE_MAX) nv = VALUE_MAX;
                _value = nv;
                if (_cb) _cb(dir, false);
            }
        }
        _lastState = state;
        return;
    }

    uint8_t idx = (_lastState << 2) | (a << 1) | b;
    int8_t dir = QTABLE[idx & 0x0F];
    if (dir) {
        _subAccum += dir;
        if (_subAccum >= SUBSTEPS_PER_NOTCH || _subAccum <= -SUBSTEPS_PER_NOTCH) {
            int steps = _subAccum / SUBSTEPS_PER_NOTCH;
            _subAccum = 0;
            uint32_t now = micros();
            uint32_t interval = now - _lastNotchUs;
            _lastNotchUs = now;
            bool accel = false;
            int inc = steps;
            if (ENABLE_ACCEL && interval > 0 && interval < FAST_THRESHOLD_US) {
                inc *= ACCEL_FACTOR;
                accel = true;
            }
            int nv = _value + inc;
            if (nv < VALUE_MIN) nv = VALUE_MIN;
            if (nv > VALUE_MAX) nv = VALUE_MAX;
            _value = nv;
            if (_cb) _cb(inc, accel);
        }
    }
    _lastState = (a << 1) | b;
}

void IRAM_ATTR encoderIsrCLK() { if (s_instance) s_instance->handleISR(); }
void IRAM_ATTR encoderIsrDT()  { if (s_instance) s_instance->handleISR(); }

#endif