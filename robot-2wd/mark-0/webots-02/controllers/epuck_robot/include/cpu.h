#ifndef CPU_H
#define CPU_H

#include <stdint.h>

/**
 * DataPath - Caminho de dados genérico
 * Gerencia registradores, acumulador e contador de programa
 */
typedef struct {
    uint32_t registers[8];
    uint32_t accumulator;
    uint32_t program_counter;
} DataPath;

/**
 * Control - Controlador genérico
 * Executa instruções usando o DataPath
 */
typedef struct {
    DataPath* datapath;
} Control;

/**
 * CPU - Central Processing Unit
 * Abstração principal que encapsula a lógica de controle (Control)
 * e o caminho de dados (DataPath) do robô.
 * 
 * A CPU é responsável por coordenar as operações de controle
 * e gerenciar o flow de dados entre os componentes do robô.
 */
typedef struct {
    DataPath* datapath;
    Control* control;
} CPU;

// ========================================
// Funções da CPU
// ========================================
CPU* cpu_create(void);
void cpu_destroy(CPU* cpu);
void cpu_initialize(CPU* cpu);
void cpu_cleanup(CPU* cpu);
void cpu_update(CPU* cpu);
void cpu_cycle(CPU* cpu);
void cpu_reset(CPU* cpu);
DataPath* cpu_get_datapath(CPU* cpu);
Control* cpu_get_control(CPU* cpu);

// ========================================
// Funções do DataPath
// ========================================
DataPath* datapath_create(void);
void datapath_destroy(DataPath* dp);
void datapath_reset(DataPath* dp);
uint32_t datapath_read_register(DataPath* dp, int reg);
void datapath_write_register(DataPath* dp, int reg, uint32_t value);
uint32_t datapath_get_accumulator(DataPath* dp);
void datapath_set_accumulator(DataPath* dp, uint32_t value);
uint32_t datapath_get_pc(DataPath* dp);
void datapath_set_pc(DataPath* dp, uint32_t value);

// ========================================
// Funções do Control
// ========================================
Control* control_create(DataPath* dp);
void control_destroy(Control* ctrl);
void control_execute_instruction(Control* ctrl, uint8_t opcode);
void control_load(Control* ctrl, int reg, uint32_t value);
void control_store(Control* ctrl, int reg, uint32_t value);
void control_add(Control* ctrl, int reg1, int reg2);
void control_subtract(Control* ctrl, int reg1, int reg2);

#endif // CPU_H