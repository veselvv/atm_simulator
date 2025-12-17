#!/bin/bash
# Проверка и установка GCC и GTK4

# Цвета
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo "=== Установка зависимостей ==="

# Определение дистрибутива
if [ -f /etc/os-release ]; then
    . /etc/os-release
    OS=$ID
else
    OS=$(uname -s)
fi

# Функция установки для Debian/Ubuntu/AntiX
install_debian() {
    echo "Обновление пакетов..."
    sudo apt update
    
    echo "Установка GCC..."
    sudo apt install -y gcc
    
    echo "Установка GTK4..."
    sudo apt install -y libgtk-4-dev
    
    echo "Установка pkg-config..."
    sudo apt install -y pkg-config
}

# Функция установки для Fedora/RHEL
install_fedora() {
    echo "Установка GCC..."
    sudo dnf install -y gcc
    
    echo "Установка GTK4..."
    sudo dnf install -y gtk4-devel
    
    echo "Установка pkg-config..."
    sudo dnf install -y pkgconf-pkg-config
}

# Функция установки для Arch
install_arch() {
    echo "Установка GCC..."
    sudo pacman -S --noconfirm gcc
    
    echo "Установка GTK4..."
    sudo pacman -S --noconfirm gtk4
    
    echo "Установка pkg-config..."
    sudo pacman -S --noconfirm pkg-config
}

# Основная логика
main() {
    # Проверка GCC
    if ! command -v gcc >/dev/null 2>&1; then
        echo -e "${YELLOW}GCC не найден, устанавливаю...${NC}"
        
        case $OS in
            debian|ubuntu|linuxmint|antix)
                install_debian
                ;;
            fedora|rhel|centos)
                install_fedora
                ;;
            arch|manjaro)
                install_arch
                ;;
            *)
                echo -e "${RED}Неизвестный дистрибутив: $OS${NC}"
                echo "Установите вручную:"
                echo "  - GCC компилятор"
                echo "  - GTK4 development files"
                exit 1
                ;;
        esac
    else
        echo -e "${GREEN}GCC уже установлен${NC}"
    fi
    
    # Проверка GTK4
    if ! pkg-config --exists gtk4 2>/dev/null; then
        echo -e "${YELLOW}GTK4 не найден, устанавливаю...${NC}"
        
        case $OS in
            debian|ubuntu|linuxmint|antix)
                sudo apt install -y libgtk-4-dev
                ;;
            fedora|rhel|centos)
                sudo dnf install -y gtk4-devel
                ;;
            arch|manjaro)
                sudo pacman -S --noconfirm gtk4
                ;;
            *)
                echo -e "${RED}Установите GTK4 вручную для $OS${NC}"
                exit 1
                ;;
        esac
    else
        echo -e "${GREEN}GTK4 уже установлен${NC}"
    fi
    
    # Проверка pkg-config (нужен для проверки GTK4)
    if ! command -v pkg-config >/dev/null 2>&1; then
        echo -e "${YELLOW}pkg-config не найден, устанавливаю...${NC}"
        
        case $OS in
            debian|ubuntu|linuxmint|antix)
                sudo apt install -y pkg-config
                ;;
            fedora|rhel|centos)
                sudo dnf install -y pkgconf-pkg-config
                ;;
            arch|manjaro)
                sudo pacman -S --noconfirm pkg-config
                ;;
        esac
    fi
    
    echo -e "\n${GREEN}=== Все зависимости установлены! ===${NC}"
    echo "Теперь можно собрать проект:"
    echo "  make  или  ./build.sh"
}

# Запуск
main

