# Installation Guide - Cat Clicker 🐱

Guide complet pour installer Cat Clicker sur votre système.

## 📋 Prérequis Généraux

- **C Compiler** (GCC, Clang, MSVC)
- **CMake** (optionnel, mais recommandé)
- **SDL2** (2.0 ou supérieur)
- **Git** (pour cloner)

## 🖥️ Par Système d'Exploitation

### Windows

#### Option 1: MinGW + MSYS2
```bash
# 1. Installer MSYS2 (https://www.msys2.org/)
# 2. Ouvrir MSYS2 Terminal
pacman -S mingw-w64-x86_64-toolchain
pacman -S mingw-w64-x86_64-SDL2
pacman -S mingw-w64-x86_64-cmake

# 3. Compiler
cd cat-clicker
cmake -B build
cd build
cmake --build .

# 4. Lancer
./cat_clicker.exe
```

#### Option 2: Visual Studio
```bash
# 1. Installer Visual Studio 2019+ avec support C
# 2. Installer SDL2 depuis https://www.libsdl.org/download-2.0.php
# 3. Définir SDL2_PATH en variable d'environnement
# 4. Compiler avec CMake
cmake -G "Visual Studio 16 2019" -B build
cd build
cmake --build . --config Release
```

#### Option 3: Direct (Sans CMake)
```bash
# Si SDL2 est dans le PATH:
gcc -o cat_clicker cat_clicker.c -lSDL2 -lm
./cat_clicker.exe
```

---

### macOS

#### Avec Homebrew (Recommandé)
```bash
# 1. Installer Homebrew (https://brew.sh/)

# 2. Installer dépendances
brew install sdl2
brew install cmake

# 3. Compiler
cd cat-clicker
cmake -B build
cd build
cmake --build .

# 4. Lancer
./cat_clicker
```

#### Avec MacPorts
```bash
sudo port install libsdl2
gcc -o cat_clicker cat_clicker.c -lSDL2 -lm
./cat_clicker
```

---

### Linux

#### Debian/Ubuntu
```bash
# 1. Mettre à jour le système
sudo apt update

# 2. Installer dépendances
sudo apt install build-essential cmake git
sudo apt install libsdl2-dev libsdl2-mixer-dev

# 3. Compiler
cd cat-clicker
cmake -B build
cd build
cmake --build .

# 4. Lancer
./cat_clicker
```

#### Fedora/RHEL
```bash
# 1. Installer dépendances
sudo dnf install gcc cmake git
sudo dnf install SDL2-devel SDL2_mixer-devel

# 2. Compiler
cd cat-clicker
cmake -B build && cd build && cmake --build .

# 3. Lancer
./cat_clicker
```

#### Arch Linux
```bash
# 1. Installer dépendances
sudo pacman -S base-devel cmake sdl2

# 2. Compiler
cd cat-clicker
cmake -B build && cd build && cmake --build .

# 3. Lancer
./cat_clicker
```

---

## 🔧 Dépannage

### Problème: "SDL2 not found"

**Solution:**
```bash
# 1. Vérifier l'installation
sdl2-config --version

# 2. Si SDL2 n'est pas installé, suivre le guide OS
# 3. Si installé, définir SDL2_DIR
export SDL2_DIR=/path/to/sdl2
cmake -B build
```

### Problème: "error: 'SDL.h' file not found"

**Windows:** Vérifier les variables d'environnement
**Mac/Linux:** Installer les headers dev
```bash
# Ubuntu/Debian
sudo apt install libsdl2-dev

# macOS
brew install sdl2
```

### Problème: "undefined reference to 'SDL_Init'"

**Cause:** Manque de linking
**Solution:**
```bash
# S'assurer de lier SDL2:
gcc cat_clicker.c -lSDL2 -lm -o cat_clicker
# OU
cmake -DSDL2_LIBRARY=/path/to/libSDL2.a -B build
```

---

## 🚀 Compilation Optimisée

### Pour Performance Maximum
```bash
# GCC/Clang
gcc -O3 -march=native cat_clicker.c -lSDL2 -lm -o cat_clicker

# CMake
cmake -DCMAKE_BUILD_TYPE=Release -B build
cd build
cmake --build .
```

### Avec Flags de Sécurité
```bash
gcc -O2 -D_FORTIFY_SOURCE=2 -fstack-protector-all cat_clicker.c -lSDL2 -lm -o cat_clicker
```

---

## 📦 Packages Docker (Avancé)

```dockerfile
FROM ubuntu:20.04

RUN apt update && apt install -y \
    build-essential cmake git \
    libsdl2-dev libsdl2-mixer-dev

WORKDIR /app
COPY . .

RUN cmake -B build && cd build && cmake --build .

CMD ["./cat_clicker"]
```

```bash
# Build et run
docker build -t cat-clicker .
docker run -it cat-clicker
```

---

## ✅ Vérification d'Installation

```bash
# 1. Vérifier compilateur
gcc --version

# 2. Vérifier SDL2
sdl2-config --version

# 3. Vérifier CMake
cmake --version

# 4. Compiler un test simple
gcc --version > /dev/null && echo "✓ GCC OK"
sdl2-config --version > /dev/null && echo "✓ SDL2 OK"
cmake --version > /dev/null && echo "✓ CMake OK"
```

---

## 🎮 Premier Lancement

```bash
# Compiler
./compile.sh  # ou cmake build

# Lancer
./cat_clicker

# Profiter!
# - Clic souris: Cliquer le chat
# - T: Changer thème
# - ESC: Quitter
```

---

## 📞 Besoin d'Aide?

- 📧 Email: contact@catclicker.dev
- 📚 Wiki: https://github.com/maryambenzekri/cat-clicker/wiki
- 💬 Issues: https://github.com/maryambenzekri/cat-clicker/issues

**Happy Gaming!** 🐱✨
