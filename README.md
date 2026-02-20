# DALI Standalone Editor

DALI 1.5.8 — Editor de texto multi-ventana para terminal, extraído del
framework IdeaFix 6.1.0 (InterSoft) para funcionar como ejecutable
independiente en Linux.

## Requisitos

- Linux (cualquier distribución moderna)
- GCC/G++ 5 o superior
- CMake 3.10+
- make
- libtinfo (parte de ncurses)

### Debian / Ubuntu

```bash
sudo apt install build-essential cmake libncurses-dev
```

### RHEL / Fedora

```bash
sudo dnf install gcc gcc-c++ cmake make ncurses-devel
```

## Compilación

```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
```

El ejecutable se genera en `build/dali`.

También se puede usar el script de conveniencia:

```bash
./compilar.sh
```

## Instalación

```bash
sudo cp build/dali /usr/local/bin/
```

O con CMake:

```bash
cd build && sudo make install
```

## Uso

```bash
dali archivo.txt                  # Abrir un archivo
dali archivo1.c archivo2.c        # Abrir múltiples archivos
dali -t nombre_tag                # Abrir por tag
dali -w workspace.wsp             # Abrir workspace
dali --help                       # Ver todas las opciones
```

### Opciones

| Opción             | Descripción                          |
|--------------------|--------------------------------------|
| `-b`, `--hide-banner` | No mostrar el banner de inicio    |
| `-d`, `--default`     | Ignorar archivo de configuración dalirc |
| `-t`, `--tag STR`     | Abrir un tag                      |
| `-s`, `--shell STR`   | Ejecutar comando shell            |
| `-w`, `--wsp STR`     | Abrir ambiente de trabajo         |
| `-V`, `--version`     | Mostrar versión                   |
| `-H`, `--help`        | Mostrar ayuda                     |

## Arquitectura

El IdeaFix original usa una arquitectura cliente-servidor donde un proceso
Window Manager (WM) maneja la terminal y DALI se conecta vía IPC (System V
message queues + semaphores).

Esta versión standalone embebe el WM directamente en el proceso DALI usando
`wmalone.cc`, eliminando la necesidad de IPC y del servidor WM separado.

```
dali (ejecutable)
├── dali_editor    Editor: comandos, sensores, texto, vistas, ayuda
├── wapp           WinApp: menús, diálogos, botones, vistas editables
├── wm             Window Manager embebido (termios + termcap)
├── ixwi           Interfaz WI: wrappers de funciones de ventana
└── idea           Biblioteca base: String, List, RegExp, OsFile, etc.
```

## Estructura del proyecto

```
dali-standalone/
├── src/
│   ├── dali/          Editor principal (38 archivos)
│   ├── winapp/        Framework de ventanas (53 archivos)
│   ├── wm/lib/        Window Manager embebido (22 archivos)
│   ├── ixwi/          Interfaz WI (20 archivos)
│   └── lib/           Biblioteca base (208 archivos)
├── include/           Headers (416 archivos)
├── data/
│   ├── spanish/       Mensajes en español
│   ├── english/       Mensajes en inglés
│   └── map/           Mapas de teclado por terminal (30+)
├── CMakeLists.txt     Sistema de build
├── compilar.sh        Script de compilación
└── .gitignore
```

## Licencia

Copyright InterSoft (c) 1988-2015. Todos los derechos reservados.
