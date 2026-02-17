/// Demonstração de sensores e atuadores
// Bibliotecas padrão C
#define _USE_MATH_DEFINES // Define constantes matemáticas como M_PI
#include <math.h>
#include <stdio.h>
// Bibliotecas do projeto
#include "environmental.h" // módulo de sensores de ambiente
#include "features.h"      // módulo de recursos extras
#include "locomotion.h"    // módulo de locomoção
#include "motion.h"        // módulo de sensores de movimento
#include "simulation.h"    // módulo de simulação

#define ROTATION_SPEED 1.88 // Velocidade de rotação do robô

int pattern4(void)
{
  printf("Pattern 4: Sensor demonstration started...\n");

  // Variáveis de controle
  short direction = 1;

  double position_offset = 0.0;
  double left_position = 0.0;
  double right_position = 0.0;
  double accel_x = 0.0, accel_y = 0.0, accel_z = 0.0;

  // Inicializa módulos
  locomotion_init();
  distance_init();
  light_init();
  accelerometer_init();
  position_init();
  camera_init();
  leds_init();

  // Loop principal
  while (wb_robot_step(simulation_get_time_step()) != -1)
  {
    // Atualiza todos os sensores
    distance_update();
    light_update();
    accelerometer_update();
    position_update();

    // Lê sensores
    double distance_sum = distance_get_sum();
    double light_sum = light_get_sum();
    long int camera_gray_sum = camera_get_gray_sum();

    accelerometer_read(&accel_x, &accel_y, &accel_z);
    position_read(&left_position, &right_position, NULL);

    // Limpa LEDs
    leds_clear_all();

    // LED 0: simulação
    if (wb_robot_get_mode() == 1)
      leds_set(0, 1);

    // LEDs 1,3,5,7: acelerômetro
    leds_set_by_accelerometer(accel_x, accel_y);

    // LED 8: distância alta
    if (distance_sum > 7000)
      leds_set(8, 1);

    // LED 9: luminosidade baixa
    if (camera_gray_sum < 100000)
      leds_set(9, 1);

    // Atualiza LEDs
    leds_update();

    // Inverte direção a cada 2π
    if ((left_position - position_offset) > (2 * M_PI) ||
        (left_position - position_offset) < -(2 * M_PI))
    {
      direction = (direction == 1) ? -1 : 1;
      position_offset = left_position;

      printf("Direction changed\n");
    }

    // Movimento em círculo
    locomotion_set_velocity(ROTATION_SPEED * direction, -ROTATION_SPEED * direction);
    locomotion_update();
  }

  return 0;
}
