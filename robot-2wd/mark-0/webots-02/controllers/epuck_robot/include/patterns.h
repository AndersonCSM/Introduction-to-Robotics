/**
 * @file patterns.h
 * @brief Declarações das funções de padrões comportamentais do robô e-puck
 *
 * Este header centraliza as declarações de todos os patterns (padrões de comportamento)
 * implementados para o robô e-puck. Cada pattern representa uma estratégia de controle
 * diferente que pode ser selecionada no main.c.
 *
 * PATTERNS DISPONÍVEIS:
 * - pattern1: Comportamento Braitenberg (obstacle avoidance)
 * - pattern2: Servidor TCP/IP para comunicação remota
 * - pattern3: Braitenberg com detecção de precipícios (ground sensors)
 * - pattern4: Demonstração multi-sensorial (câmera, acelerômetro, luz)
 * - pattern5: Line following com subsumption architecture (standalone)
 * - pattern6: Line following completo com obstacle avoidance (standalone)
 * - default:  Template base para novos patterns (standalone)
 *
 * @note pattern5, pattern6 e default possuem int main() próprio e são programas
 *       completos que devem ser compilados separadamente, não são funções chamáveis.
 *
 * @see patterns/pattern1.c, patterns/pattern2.c, patterns/pattern3.c, patterns/pattern4.c
 */

#ifndef PATTERNS_H
#define PATTERNS_H

/**
 * @brief Pattern 1 - Comportamento Braitenberg para evitar obstáculos
 *
 * Implementa algoritmo reativo usando matriz de coeficientes de Braitenberg.
 * O robô lê 8 sensores de proximidade e calcula velocidades dos motores
 * para evitar obstáculos de forma suave e natural.
 *
 * CARACTERÍSTICAS:
 * - Usa 8 sensores de distância (ps0-ps7)
 * - Matriz de pesos 8x2 (sensor x motor)
 * - Comportamento "fóbico" (evita obstáculos)
 * - Acelerômetro para monitoramento
 * - Odometria para tracking de posição
 *
 * @return 0 quando o pattern é finalizado
 */
int pattern1(void);

/**
 * @brief Pattern 2 - Servidor TCP/IP para controle remoto
 *
 * Implementa servidor TCP/IP que aguarda conexões de clientes e processa
 * comandos remotos para controlar o robô. Usa funções da API em features.c.
 *
 * CARACTERÍSTICAS:
 * - Servidor TCP na porta 1000 (padrão)
 * - Comandos: sensores, imagem, movimento
 * - Socket não-bloqueante
 * - Comunicação bidirecional
 *
 * COMANDOS DISPONÍVEIS:
 * - "S": Envia valores dos sensores
 * - "I": Envia imagem da câmera
 * - "M": Define velocidades dos motores
 *
 * @return 0 quando o servidor é finalizado
 */
int pattern2(void);

/**
 * @brief Pattern 3 - Braitenberg com detecção de precipícios
 *
 * Estende o comportamento Braitenberg adicionando detecção de precipícios
 * usando sensores de chão. Se detecta cliff, o robô recua e vira.
 *
 * CARACTERÍSTICAS:
 * - Comportamento Braitenberg base
 * - 3 sensores de chão (left, center, right)
 * - Detecção de cliff com threshold
 * - Reação: recuar + virar
 * - LEDs indicam status
 *
 * @return 0 quando o pattern é finalizado
 */
int pattern3(void);

/**
 * @brief Pattern 4 - Demonstração multi-sensorial
 *
 * Demonstra uso integrado de múltiplos sensores (câmera, acelerômetro,
 * distância, luz) e atuadores (LEDs, motores). Robô se move em círculos
 * alternando direção periodicamente.
 *
 * CARACTERÍSTICAS:
 * - Câmera: soma de intensidade de pixels
 * - Acelerômetro: controla LEDs proporcionalmente
 * - Distância: soma de todos os sensores
 * - Luz: soma de todos os sensores
 * - Movimento circular com inversão periódica
 *
 * @return 0 quando o pattern é finalizado
 */
int pattern4(void);

#endif // PATTERNS_H
