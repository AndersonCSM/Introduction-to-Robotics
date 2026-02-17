// Bibliotecas padrão C
#include <stdio.h>  // entrada e saída
#include <stdlib.h> // utilitários gerais
#include <string.h> // manipulação de strings
// Bibliotecas do projeto
#include "patterns.h"   // declarações dos patterns comportamentais
#include "simulation.h" // módulo de simulação

int main(int argc, char *argv[])
{
    // Inicialização dos módulos Initialize simulation module
    simulation_init();

    int retorno = pattern1(); // arquivo de desenvolvimento

    simulation_cleanup(); // Limpeza para finalização

    return EXIT_SUCCESS; // finaliza o programa
}
