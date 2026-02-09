#include <stdlib.h>
#include "cpu.h"
#include "control.h"
#include "datapath.h"

/**
 * DataPath - Criar instância
 */
DataPath *datapath_create(void)
{
    DataPath *dp = (DataPath *)malloc(sizeof(DataPath));
    if (dp)
    {
        datapath_reset(dp);
    }
    return dp;
}

/**
 * DataPath - Destruir instância
 */
void datapath_destroy(DataPath *dp)
{
    if (dp)
    {
        free(dp);
    }
}

/**
 * DataPath - Reset
 */
void datapath_reset(DataPath *dp)
{
    if (!dp)
        return;

    dp->accumulator = 0;
    dp->program_counter = 0;
    for (int i = 0; i < 8; i++)
    {
        dp->registers[i] = 0;
    }
}

/**
 * DataPath - Ler registrador
 */
uint32_t datapath_read_register(DataPath *dp, int reg)
{
    if (!dp || reg < 0 || reg >= 8)
    {
        return 0;
    }
    return dp->registers[reg];
}

/**
 * DataPath - Escrever registrador
 */
void datapath_write_register(DataPath *dp, int reg, uint32_t value)
{
    if (!dp || reg < 0 || reg >= 8)
    {
        return;
    }
    dp->registers[reg] = value;
}

/**
 * DataPath - Obter acumulador
 */
uint32_t datapath_get_accumulator(DataPath *dp)
{
    if (!dp)
        return 0;
    return dp->accumulator;
}

/**
 * DataPath - Definir acumulador
 */
void datapath_set_accumulator(DataPath *dp, uint32_t value)
{
    if (!dp)
        return;
    dp->accumulator = value;
}

/**
 * DataPath - Obter contador de programa
 */
uint32_t datapath_get_pc(DataPath *dp)
{
    if (!dp)
        return 0;
    return dp->program_counter;
}

/**
 * DataPath - Definir contador de programa
 */
void datapath_set_pc(DataPath *dp, uint32_t value)
{
    if (!dp)
        return;
    dp->program_counter = value;
}

// ========================================
// Control - Funções
// ========================================

/**
 * Control - Criar instância
 */
Control *control_create(DataPath *dp)
{
    Control *ctrl = (Control *)malloc(sizeof(Control));
    if (ctrl)
    {
        ctrl->datapath = dp;
    }
    return ctrl;
}

/**
 * Control - Destruir instância
 */
void control_destroy(Control *ctrl)
{
    if (ctrl)
    {
        free(ctrl);
    }
}

/**
 * Control - Executar instrução
 */
void control_execute_instruction(Control *ctrl, uint8_t opcode)
{
    if (!ctrl)
        return;

    switch (opcode)
    {
    case 0x00: // NOP
        break;
    case 0x01: // LOAD
        break;
    case 0x02: // STORE
        break;
    case 0x03: // ADD
        break;
    case 0x04: // SUB
        break;
    default:
        break;
    }
}

/**
 * Control - Carregar valor em registrador
 */
void control_load(Control *ctrl, int reg, uint32_t value)
{
    if (!ctrl || !ctrl->datapath)
        return;
    datapath_write_register(ctrl->datapath, reg, value);
}

/**
 * Control - Armazenar valor de registrador
 */
void control_store(Control *ctrl, int reg, uint32_t value)
{
    if (!ctrl || !ctrl->datapath)
        return;
    datapath_write_register(ctrl->datapath, reg, value);
}

/**
 * Control - Somar dois registradores
 */
void control_add(Control *ctrl, int reg1, int reg2)
{
    if (!ctrl || !ctrl->datapath)
        return;

    uint32_t value1 = datapath_read_register(ctrl->datapath, reg1);
    uint32_t value2 = datapath_read_register(ctrl->datapath, reg2);
    datapath_set_accumulator(ctrl->datapath, value1 + value2);
}

/**
 * Control - Subtrair dois registradores
 */
void control_subtract(Control *ctrl, int reg1, int reg2)
{
    if (!ctrl || !ctrl->datapath)
        return;

    uint32_t value1 = datapath_read_register(ctrl->datapath, reg1);
    uint32_t value2 = datapath_read_register(ctrl->datapath, reg2);
    datapath_set_accumulator(ctrl->datapath, value1 - value2);
}

// ========================================
// CPU - Funções
// ========================================

/**
 * CPU - Criar instância
 */
CPU *cpu_create(void)
{
    CPU *cpu = (CPU *)malloc(sizeof(CPU));
    if (cpu)
    {
        cpu->datapath = datapath_create();
        cpu->control = control_create(cpu->datapath);
    }
    return cpu;
}

/**
 * CPU - Destruir instância
 */
void cpu_destroy(CPU *cpu)
{
    if (!cpu)
        return;

    if (cpu->control)
    {
        control_destroy(cpu->control);
    }
    if (cpu->datapath)
    {
        datapath_destroy(cpu->datapath);
    }
    free(cpu);
}

/**
 * CPU - Inicializar
 * Inicializa a CPU e seus componentes
 */
void cpu_initialize(CPU *cpu)
{
    if (!cpu || !cpu->datapath)
        return;

    datapath_reset(cpu->datapath);

    // Inicializa funções do controle do robô
    control_init();
    datapath_init();
}

/**
 * CPU - Atualizar
 * Executa o ciclo de atualização da CPU
 * Coordena a leitura de sensores, processamento e atualização de atuadores
 */
void cpu_update(CPU *cpu)
{
    if (!cpu)
        return;

    // Atualiza o datapath (roteamento de dados)
    datapath_update();

    // Atualiza a lógica de controle
    control_update();
}

/**
 * CPU - Ciclo
 * Ciclo completo de processamento da CPU
 * Coordena todas as operações de controle e datapath
 */
void cpu_cycle(CPU *cpu)
{
    if (!cpu)
        return;

    // Executa apenas o ciclo generico da CPU
    // (controle e datapath, sem acoplamento a sensores/atuadores)
    cpu_update(cpu);
}

/**
 * CPU - Limpar
 * Limpa recursos e finaliza a CPU
 */
void cpu_cleanup(CPU *cpu)
{
    if (!cpu)
        return;

    datapath_cleanup();
    control_cleanup();
}

/**
 * CPU - Reset
 * Reseta o estado da CPU
 */
void cpu_reset(CPU *cpu)
{
    if (!cpu || !cpu->datapath)
        return;
    datapath_reset(cpu->datapath);
}

/**
 * CPU - Obter DataPath
 */
DataPath *cpu_get_datapath(CPU *cpu)
{
    if (!cpu)
        return NULL;
    return cpu->datapath;
}

/**
 * CPU - Obter Control
 */
Control *cpu_get_control(CPU *cpu)
{
    if (!cpu)
        return NULL;
    return cpu->control;
}
