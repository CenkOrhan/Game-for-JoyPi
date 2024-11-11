#!/bin/bash

# Updating and upgrading system packages
sudo apt-get update
sudo apt-get upgrade -y

# Function to install a package
install_package() {
    echo -n "Would you like to install $1? (Y/n): "
    read answer
    if [ "$answer" = "Y" ] || [ "$answer" = "y" ]; then
        sudo apt-get install -y "$1"
    else
        echo "Installation of $1 skipped."
    fi
}

# Function to install WiringPi from source
install_wiringpi() {
    echo "WiringPi is NOT installed. Installing from source now."
    sudo apt-get install -y git git-core
    git clone https://github.com/WiringPi/WiringPi
    cd WiringPi
    ./build
    echo "WiringPi installed successfully."
}

# Function to check if a package is installed
check_package() {
    if dpkg-query -W -f='${Status}' $1 2>/dev/null | grep -q "install ok installed"; then
        echo "Package $1 is installed."
    else
        echo "Package $1 is NOT installed."
        install_package "$1"
    fi
}

# Function to check WiringPi installation using gpio readall
check_wiringpi() {
    if gpio readall > /dev/null 2>&1; then
        echo "WiringPi is installed and functional."
    else
        echo "WiringPi is NOT installed or not functional."
        install_wiringpi
    fi
}

# Function to check and modify /boot/config.txt for specific settings
check_and_modify_config() {
    local config_file="/boot/config.txt"
    local settings=("device_tree_param=spi=on" "dtoverlay=spi-bcm2708")

    for setting in "${settings[@]}"; do
        if grep -q "^$setting" "$config_file"; then
            echo "$setting is already set."
        else
            echo "$setting was not found. Adding it to $config_file."
            echo "$setting" | sudo tee -a "$config_file"
        fi
    done
}

# Function to enable raspi-config settings
enable_raspi_config() {
    local interfaces=("spi" "i2c" "ssh" "vnc")
    for interface in "${interfaces[@]}"; do
        local status=$(sudo raspi-config nonint get_$interface)
        if [ "$status" -eq 1 ]; then
            echo "$interface is disabled, enabling now."
            sudo raspi-config nonint do_$interface 0
        else
            echo "$interface is already enabled."
        fi
    done
}

check_wiringpi
check_package "libi2c-dev"
check_package "code"
check_package "terminator"
check_and_modify_config
enable_raspi_config

echo "All checks and configurations are completed. Please verify any missing components are installed as suggested."
