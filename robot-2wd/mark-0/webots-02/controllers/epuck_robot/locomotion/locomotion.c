/// Módulo de locomoção e controle de motores
// Bibliotecas Webots
#include <webots/motor.h>
#include <webots/robot.h>
// Bibliotecas do projeto
#include "locomotion.h"
#include "simulation.h"

// Índices para arrays de motores
#define LEFT 0
#define RIGHT 1

// Dispositivos dos motores das rodas
static WbDeviceTag left_motor;
static WbDeviceTag right_motor;

// Buffer de velocidades armazenadas (rad/s)
static double motor_speeds[2] = {0.0, 0.0};

/// Inicializa os motores em modo de controle por velocidade
void locomotion_init(void)
{
    // Obtém referências aos dispositivos dos motores
    left_motor = wb_robot_get_device("left wheel motor");
    right_motor = wb_robot_get_device("right wheel motor");

    // Inicializa velocidades em zero
    motor_speeds[LEFT] = 0.0;
    motor_speeds[RIGHT] = 0.0;

    // Configura posição infinita (modo de controle por velocidade)
    wb_motor_set_position(left_motor, INFINITY);
    wb_motor_set_position(right_motor, INFINITY);

    // Define velocidades iniciais
    wb_motor_set_velocity(left_motor, 0.0);
    wb_motor_set_velocity(right_motor, 0.0);
}

/// Define velocidades desejadas para os motores (armazena no buffer)
void locomotion_set_velocity(double left_speed, double right_speed)
{
    motor_speeds[LEFT] = left_speed;
    motor_speeds[RIGHT] = right_speed;
}

/// Aplica as velocidades armazenadas nos motores
void locomotion_update(void)
{
    wb_motor_set_velocity(left_motor, motor_speeds[LEFT]);
    wb_motor_set_velocity(right_motor, motor_speeds[RIGHT]);
}

/// Para os motores (velocidade zero)
void locomotion_cleanup(void)
{
    wb_motor_set_velocity(left_motor, 0.0);
    wb_motor_set_velocity(right_motor, 0.0);
}