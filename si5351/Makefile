# Makefile para proyecto SI5351 con PlatformIO

.PHONY: all build upload monitor clean test format

# Comando por defecto
all: build

# Compilar el proyecto
build:
	@echo "Compilando proyecto SI5351..."
	pio run

# Subir al Arduino
upload:
	@echo "Subiendo código al Arduino Uno..."
	pio run --target upload

# Monitor serie
monitor:
	@echo "Iniciando monitor serie a 115200 baudios..."
	pio device monitor --baud 115200

# Compilar y subir en un solo comando
deploy: build upload

# Limpiar archivos de compilación
clean:
	@echo "Limpiando archivos de compilación..."
	pio run --target clean

# Ejecutar tests (si existen)
test:
	@echo "Ejecutando tests..."
	pio test

# Formatear código
format:
	@echo "Formateando código..."
	find src include -name "*.cpp" -o -name "*.h" | xargs clang-format -i

# Información del proyecto
info:
	@echo "=== Información del proyecto SI5351 ==="
	@echo "Plataforma: Arduino Uno (ATmega328P)"
	@echo "Framework: Arduino"
	@echo "Librerías: Si5351Arduino, Adafruit BusIO"
	@echo "Baudios: 115200"
	@echo ""
	@echo "Comandos disponibles:"
	@echo "  make build    - Compilar"
	@echo "  make upload   - Subir código"
	@echo "  make monitor  - Monitor serie"
	@echo "  make deploy   - Compilar y subir"
	@echo "  make clean    - Limpiar"
	@echo "  make info     - Esta información"

# Verificar instalación de PlatformIO
check:
	@echo "Verificando instalación de PlatformIO..."
	@which pio > /dev/null || (echo "Error: PlatformIO no encontrado. Instálalo con: pip install platformio" && exit 1)
	@echo "PlatformIO encontrado correctamente"
	@pio --version

# Instalar dependencias del proyecto
install:
	@echo "Instalando dependencias..."
	pio lib install

# Ayuda
help: info
