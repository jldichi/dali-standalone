#!/bin/bash
# compilar.sh - Compila DALI standalone e instala en /usr/bin
set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
BUILD_DIR="$SCRIPT_DIR/build"

echo "=== DALI Standalone - Compilación ==="
echo ""

# Verificar dependencias
for cmd in cmake g++ make; do
    if ! command -v "$cmd" &>/dev/null; then
        echo "ERROR: '$cmd' no está instalado."
        echo ""
        echo "En Debian/Ubuntu:  sudo apt install build-essential cmake"
        echo "En RHEL/Fedora:    sudo dnf install gcc gcc-c++ cmake make"
        exit 1
    fi
done

# Verificar que existen los fuentes
if [ ! -d "$SCRIPT_DIR/src" ] || [ ! -d "$SCRIPT_DIR/include" ]; then
    echo "ERROR: No se encontraron los directorios src/ e include/."
    echo "Ejecutá primero:  ./setup.sh /ruta/a/ideafix610"
    exit 1
fi

# Limpiar build anterior si existe
if [ -d "$BUILD_DIR" ]; then
    echo "Limpiando build anterior..."
    rm -rf "$BUILD_DIR"
fi

# Crear directorio de build
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Configurar con CMake
echo ""
echo "[1/2] Configurando con CMake..."
cmake "$SCRIPT_DIR"

# Compilar
echo ""
echo "[2/2] Compilando..."
NPROC=$(nproc 2>/dev/null || echo 2)
make -j"$NPROC"

# Verificar que se generó el ejecutable
if [ ! -f "$BUILD_DIR/dali" ]; then
    echo ""
    echo "ERROR: La compilación no generó el ejecutable."
    exit 1
fi

echo ""
echo "=== Compilación exitosa ==="
echo ""

# Instalar en /usr/bin
echo "Instalando dali en /usr/bin..."
if [ "$(id -u)" -eq 0 ]; then
    cp "$BUILD_DIR/dali" /usr/bin/dali
    chmod 755 /usr/bin/dali
else
    sudo cp "$BUILD_DIR/dali" /usr/bin/dali
    sudo chmod 755 /usr/bin/dali
fi

echo "Instalado: /usr/bin/dali"
echo ""
echo "Listo. Ejecutá 'dali' para iniciar el editor."
