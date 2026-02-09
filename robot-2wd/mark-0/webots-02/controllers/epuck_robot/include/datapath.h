#ifndef DATAPATH_H
#define DATAPATH_H

/**
 * Datapath Module
 * Este módulo gerencia a comunicação e roteamento de dados entre os componentes do robô.
 * Responsável por coordenar o fluxo de informações de sensores para atuadores.
 * 
 * Note: As estruturas e funções genéricas do DataPath estão em cpu.h
 * Este arquivo contém as funções específicas para inicialização e atualização
 * do roteamento de dados do robô.
 */

void datapath_init(void);
void datapath_update(void);
void datapath_cleanup(void);

#endif // DATAPATH_H
