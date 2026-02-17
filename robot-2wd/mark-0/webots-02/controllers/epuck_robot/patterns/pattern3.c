/// Braitenberg com detecção de precipícios
// Bibliotecas padrão C
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
// Bibliotecas do projeto
#include "environmental.h" // módulo de sensores de ambiente
#include "features.h"      // módulo de recursos extras
#include "locomotion.h"    // módulo de locomoção
#include "motion.h"        // módulo de sensores de movimento
#include "sensors.h"       // módulo de sensores
#include "simulation.h"    // módulo de simulação

// Configurações do Braitenberg
#define MAX_SPEED 6.28

static double weights[8][2] = {{-1.3, -1.0},
                               {-1.3, -1.0},
                               {-0.5, 0.5},
                               {0.0, 0.0},
                               {0.0, 0.0},
                               {0.05, -0.5},
                               {-0.75, 0},
                               {-0.75, 0}};
static double offsets[2] = {0.5 * MAX_SPEED, 0.5 * MAX_SPEED};

/// Move para trás por 0.2s
static void go_backwards(void)
{
  locomotion_set_velocity(-MAX_SPEED, -MAX_SPEED);
  locomotion_update();
  passive_wait(0.2);
}

/// Vira à esquerda por 0.2s
static void turn_left(void)
{
  locomotion_set_velocity(-MAX_SPEED, MAX_SPEED);
  locomotion_update();
  passive_wait(0.2);
}

/// Executa Braitenberg com pesos para controle reativo
static void run_braitenberg(double *distance_values)
{
  int i, j;
  double speeds[2];

  // Calcula velocidades com pesos de Braitenberg
  for (i = 0; i < 2; i++)
  {
    speeds[i] = 0.0;
    for (j = 0; j < 8; j++)
      speeds[i] += distance_values[j] * weights[j][i];

    speeds[i] = offsets[i] + speeds[i] * MAX_SPEED;

    // Satura velocidade
    if (speeds[i] > MAX_SPEED)
      speeds[i] = MAX_SPEED;
    else if (speeds[i] < -MAX_SPEED)
      speeds[i] = -MAX_SPEED;
  }

  // Seta velocidades dos motores
  locomotion_set_velocity(speeds[0], speeds[1]);
  locomotion_update();
}

int pattern3(void)
{
  printf("Pattern 3: Braitenberg controller with cliff detection started...\n");

  double distance_values[8];
  double ground_values[3];

  // Inicializa módulos
  locomotion_init();
  distance_init();
  ground_init();
  leds_init();

  // Loop principal
  while (wb_robot_step(simulation_get_time_step()) != -1)
  {
    // Reseta atuadores
    reset_actuator_values();

    // Atualiza sensores
    distance_update();
    ground_update();
    distance_read(distance_values);
    ground_read(ground_values);

    // Pisca LEDs
    blink_leds();

    // Verifica precipício
    if (cliff_detected())
    {
      // Detectou → recua e vira
      go_backwards();
      turn_left();
    }
    else
    {
      // Segue Braitenberg
      run_braitenberg(distance_values);
    }

    // Atualiza LEDs
    leds_update();
  }

  return EXIT_SUCCESS;
}
