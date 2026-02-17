// Bibliotecas do C
#include <stdio.h>  // entrada e saída
#include <stdlib.h> // utilitários gerais
#include <string.h> // manipulação de strings
// Arquitetura em módulos do robô
#include "simulation.h" // módulo de simulação
#include "locomotion.h" // módulo de locomoção
#include "sensors.h"    // módulo de dsensores
// #include "features.h"   // módulo de recursos extras do robô
// #include "cpu.h"        // módulo de CPU

int main(int argc, char *argv[])
{
    // Inicialização dos módulos Initialize simulation module
    simulation_init();
    locomotion_init();
    sensors_init(0);

    // Loop do Main - TODO: adicionar paralelismo:"o beta sabe fazer?"
    while (1)
    {
        // Variaveis do projeto

        // Leitura dos sensores
        sensors_read();
        sensors_update();

        // Atuadores

        // Atualização da simulação
        simulation_update();
    }

    // finalização e limpeza de recursos dos módulos
    sensors_cleanup();
    locomotion_cleanup();
    simulation_cleanup();

    return EXIT_SUCCESS; // finaliza o programa
}