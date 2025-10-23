# Instalar pigpio
sudo apt install pigpio pigpio-tools -y 

# Habilitar daemon
sudo systemctl enable pigpiod
sudo systemctl start pigpiod
