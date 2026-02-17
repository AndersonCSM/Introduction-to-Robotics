#include <stdio.h> // Bibliotecas padrão C
// Bibliotecas do projeto
#include "environmental.h" // módulo de sensores de ambiente
#include "locomotion.h"    // módulo de locomoção
#include "motion.h"        // módulo de sensores de movimento
#include "sensors.h"       // módulo de sensores
#include "simulation.h"    // módulo de simulação

int pattern1(void)
{
    double speed[2];         // velocidades [esquerdo, direito]
    double sensors_value[8]; // valores dos 8 sensores de distância
    // Coeficientes Braitenberg: [sensor][motor] - positivo=atração, negativo=repulsão
    double braitenberg_coefficients[8][2] = {{0.942, -0.22},
                                             {0.63, -0.1},
                                             {0.5, -0.06},
                                             {-0.06, -0.06},
                                             {-0.06, -0.06},
                                             {-0.06, 0.5},
                                             {-0.19, 0.63},
                                             {-0.13, 0.942}};
    int i, j;

    // Inicializa locomoção e sensores
    locomotion_init();
    sensors_init(0b000011101); // distância, acelerômetro, posição, câmera

    // Loop principal
    while (wb_robot_step(simulation_get_time_step()) != -1)
    {
        // Variáveis de sensores
        double accel_x = 0.0, accel_y = 0.0, accel_z = 0.0;
        double left_position = 0.0, right_position = 0.0;

        // Atualiza e lê sensores
        distance_update();
        distance_read(sensors_value);

        accelerometer_update();
        accelerometer_read(&accel_x, &accel_y, &accel_z);
        printf("accelerometer values = %0.2f %0.2f %0.2f\n", accel_x, accel_y, accel_z);

        position_update();
        position_read(&left_position, &right_position, NULL);

        compute_odometry(left_position, right_position);

        // Braitenberg: speed[motor] = sum(coef[sensor][motor] * (1.0 - sensor_normalizado))
        for (i = 0; i < 2; i++)
        {
            speed[i] = 0.0;
            for (j = 0; j < 8; j++)
                speed[i] += braitenberg_coefficients[j][i] * (1.0 - (sensors_value[j] / RANGE));
        }

        // Define velocidades dos motores
        locomotion_set_velocity(speed[0], speed[1]);
        locomotion_update();
    }

    return 0;
}