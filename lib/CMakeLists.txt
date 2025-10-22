cmake_minimum_required(VERSION 3.10)
project(I2c)

# Supondo que seus arquivos fonte são: modulo1.c, modulo2.c, modulo.h
set(SRC ./srcs/I2c.cpp ./srcs/I2c_PcA9685.cpp ./srcs/I2c_INA219.cpp)

# Biblioteca estática (libmeu_modulo.a)
add_library(lib_i2c STATIC ${SRC})

# Target 'lib' para rodar: make lib (gera libmeu_modulo.a)
add_custom_target(lib DEPENDS meu_modulo)

# Opcional: se quiser incluir testes (supondo que test.cpp tem um main)

    add_executable(testes ./test/main.cpp ${SRC})
    add_custom_target(test DEPENDS testes)
