# ESP32 Rotary + LCD UI

Arquitectura modular para un proyecto con:
- Encoder rotatorio (cuadratura) con aceleración opcional
- Botón con debounce y detección de pulsación larga
- Pantalla LCD I2C 16x2

## Estructura de módulos

```
include/
  Config.h              # Pines y parámetros globales
  QuadratureEncoder.h   # Clase encoder (ISR + decodificación cuadratura)
  DebouncedButton.h     # Clase botón con debounce
  LcdUi.h               # UI mínima sobre LiquidCrystal_I2C
src/
  QuadratureEncoder.cpp
  DebouncedButton.cpp
  LcdUi.cpp             # (placeholder, lógica inline en header por ahora)
  main.cpp              # Orquestación
```

## Flujo principal
`main.cpp` inicializa I2C, LCD, botón y encoder. Cada iteración del loop:
1. Actualiza estado del botón (`button.update()`).
2. Aplica acciones: reset (long press) o incremento rápido (+10 click corto).
3. Refresca la UI con `ui.showValue()` y `ui.showStatus()`.
4. La lógica de conteo del encoder ocurre en interrupciones (dos líneas CHANGE).

## Encoder
- Tabla de transición de 16 entradas para cuadratura robusta.
- Acumula subpasos (`SUBSTEPS_PER_NOTCH` = 4 típico) y convierte en un notch lógico.
- Aceleración: si el tiempo entre notches < `FAST_THRESHOLD_US`, multiplica por `ACCEL_FACTOR`.
- Callback opcional (`setCallback`) para reaccionar a cada notch (ej. navegación de menús futura).

## Botón
- Lectura activa en LOW (`INPUT_PULLUP`).
- Debounce tiempo (`BUTTON_DEBOUNCE_MS`).
- Clasifica liberación en click o long press según `BUTTON_LONGPRESS_MS`.
- Consumo: polling simple en loop (sin interrupciones para simplicidad).

## UI
- Encapsula `LiquidCrystal_I2C`.
- Redibuja línea de valor solo si cambia (reducción flicker y coste I2C).
- Segunda línea muestra estado de botón + indicador de aceleración (FAST).

## Parámetros ajustables (`Config.h`)
| Parámetro | Descripción |
|-----------|-------------|
| `SUBSTEPS_PER_NOTCH` | Nº de transiciones de cuadratura por notch físico (4 típico). |
| `ENABLE_ACCEL` | Activa aceleración. |
| `FAST_THRESHOLD_US` | Umbral de microsegundos para considerar giro rápido. |
| `ACCEL_FACTOR` | Multiplicador aplicado en giro rápido. |
| `VALUE_MIN`, `VALUE_MAX` | Rango de valor permitido. |
| `BUTTON_DEBOUNCE_MS` | Debounce del botón. |
| `BUTTON_LONGPRESS_MS` | Umbral de pulsación larga. |

## Extensiones futuras
1. Menú: usar callback del encoder para mover índice y el botón para seleccionar.
2. Persistencia: almacenar `value` en NVS (Preferences) al cambiar.
3. PCNT: reemplazar ISR por periférico de conteo hardware en ESP32 (
   - Permite conteo de alta velocidad sin carga de CPU
   - Se añadiría `#define USE_PCNT` y una clase alternativa `PcntEncoder`).
4. Barra de progreso: mapear `value` a 0..16 caracteres.
5. Modo bajo consumo: apagar retroiluminación tras N segundos sin actividad.

## Ejemplo de integración de menú (conceptual)
```cpp
struct MenuItem { const char* label; int* ref; };
MenuItem items[] = {
  {"Umbral", &threshold},
  {"Brillo", &brightness},
  {"Modo", &mode}
};
int index = 0;
encoder.setCallback([](int delta, bool accel){
  index = constrain(index + delta, 0, (int)(sizeof(items)/sizeof(items[0]))-1);
});
```

## Buenas prácticas
- Mantener las ISR lo más cortas posible (actual algoritmo ya es O(1)).
- Evitar prints dentro de ISR.
- Documentar cualquier cambio en parámetros en `Config.h`.

## Troubleshooting rápido
| Síntoma | Posible causa | Solución |
|--------|---------------|----------|
| Saltos de 2+ | `SUBSTEPS_PER_NOTCH` incorrecto | Ajustar a 2 o 4 según encoder |
| Valor retrocede al final | Rebotes residuales | Desactivar aceleración / revisar cables |
| Flicker LCD | Redibujado demasiado frecuente | Confirmar que UI solo redibuja on-change |
| Pérdida de pasos muy rápidos | Limite físico ISR | Implementar PCNT |

---
Listo para expansión sin “spaghetti”.
