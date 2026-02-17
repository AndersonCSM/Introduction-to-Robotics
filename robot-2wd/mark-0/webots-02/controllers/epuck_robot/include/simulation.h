/**
 * @file simulation.h
 * @brief Interface do módulo de controle e gerenciamento da simulação Webots
 *
 * Este header define a interface pública para o módulo de simulação, que fornece
 * abstração para operações básicas do Webots como gerenciamento de time steps,
 * execução de passos de simulação, e espera passiva.
 *
 * FUNÇÕES PRINCIPAIS:
 * - simulation_init(): Inicializa o robô Webots
 * - simulation_get_time_step(): Obtém time step da simulação
 * - simulation_get_camera_time_step(): Obtém time step específico da câmera
 * - step(): Executa um passo de simulação
 * - passive_wait(): Aguarda passivamente por um período
 * - simulation_cleanup(): Libera recursos do robô
 *
 * @see simulation.c para implementação detalhada
 */

#ifndef SIMULATION_H
#define SIMULATION_H

/**
 * @brief Inicializa o robô e o sistema de simulação Webots
 *
 * Deve ser a primeira função chamada em qualquer controlador.
 * Estabelece conexão com o supervisor e prepara o ambiente.
 *
 * @note Deve ser chamada antes de qualquer outra função do Webots
 */
void simulation_init(void);

/**
 * @brief Atualiza o estado da simulação (placeholder)
 *
 * Função reservada para futuras atualizações de estado do módulo de simulação.
 * Atualmente não implementada mas mantida para compatibilidade de interface.
 */
void simulation_update(void);

/**
 * @brief Limpa e libera recursos do robô e da simulação
 *
 * Desabilita dispositivos, fecha conexões e libera memória alocada.
 * Deve ser chamada antes de encerrar o controlador.
 */
void simulation_cleanup(void);

/**
 * @brief Obtém o time step básico da simulação
 *
 * Retorna o time step (em ms) configurado no arquivo de mundo Webots.
 * Usa lazy initialization - calcula apenas na primeira chamada.
 *
 * @return Time step da simulação em milissegundos (tipicamente 32, 64, etc.)
 */
int simulation_get_time_step(void);

/**
 * @brief Obtém o time step otimizado para câmera baseado no modelo
 *
 * Detecta automaticamente o modelo do robô e retorna time step apropriado:
 * - e-puck2: 64ms (câmera rápida)
 * - e-puck original: 1024ms (câmera lenta)
 *
 * @return Time step da câmera em milissegundos
 */
int simulation_get_camera_time_step(void);

/**
 * @brief Executa um único passo de simulação com tratamento de finalização
 *
 * Avança a simulação por um time step e detecta quando o controlador
 * deve ser finalizado, executando limpeza automática se necessário.
 *
 * @note Chama exit(EXIT_SUCCESS) internamente quando o controlador é terminado
 */
void step(void);

/**
 * @brief Aguarda passivamente por um período especificado
 *
 * Implementa delay não-bloqueante executando passos de simulação
 * repetidamente até que o tempo especificado tenha decorrido.
 * Mantém a simulação ativa durante a espera.
 *
 * @param sec Tempo de espera em segundos (aceita fracionários, ex: 0.5)
 *
 * @note A precisão depende do time_step da simulação
 */
void passive_wait(double sec);

#endif // SIMULATION_H
