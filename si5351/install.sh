#!/bin/bash

# Script de instalación para el proyecto SI5351
# =============================================

echo "=== Instalación del proyecto SI5351 con Arduino Uno ==="
echo ""

# Verificar si PlatformIO está instalado
if ! command -v pio &> /dev/null; then
    echo "❌ PlatformIO no está instalado."
    echo "Instálalo con: pip install platformio"
    echo "O visita: https://platformio.org/install"
    exit 1
fi

echo "✅ PlatformIO encontrado"

# Verificar si estamos en el directorio correcto
if [ ! -f "platformio.ini" ]; then
    echo "❌ No se encontró platformio.ini"
    echo "Ejecuta este script desde el directorio del proyecto"
    exit 1
fi

echo "✅ Archivo platformio.ini encontrado"

# Limpiar build anterior
echo ""
echo "🧹 Limpiando build anterior..."
pio run --target clean

# Instalar dependencias
echo ""
echo "📦 Instalando dependencias..."
pio lib install

# Compilar el proyecto
echo ""
echo "🔨 Compilando proyecto..."
if pio run; then
    echo ""
    echo "✅ ¡Compilación exitosa!"
    echo ""
    echo "Siguiente pasos:"
    echo "1. Conecta tu Arduino Uno al puerto USB"
    echo "2. Verifica las conexiones según el esquema en README.md"
    echo "3. Ejecuta: make upload (o pio run --target upload)"
    echo "4. Ejecuta: make monitor (o pio device monitor)"
    echo ""
    echo "Comandos útiles:"
    echo "  make build    - Compilar"
    echo "  make upload   - Subir código al Arduino"
    echo "  make monitor  - Ver salida serie"
    echo "  make deploy   - Compilar y subir"
    echo ""
else
    echo ""
    echo "❌ Error en la compilación"
    echo "Revisa los errores mostrados arriba"
    exit 1
fi
