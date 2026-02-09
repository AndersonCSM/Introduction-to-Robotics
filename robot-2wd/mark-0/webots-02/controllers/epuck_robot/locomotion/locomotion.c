/**
 * Locomotion Module Implementation
 *
 * Este modulo centraliza a logica de locomocao e controle de movimento
 * do robo, desacoplado dos sensores.
 *
 * A logica de comportamento esta integrada com o modulo de controle (control.c)
 */

#include "locomotion.h"

#include <webots/motor.h>
#include <webots/robot.h>

#define LEFT 0
#define RIGHT 1

static WbDeviceTag left_motor;
static WbDeviceTag right_motor;
static double motor_speeds[2] = {0.0, 0.0};

void locomotion_init(void)
{
    left_motor = wb_robot_get_device("left wheel motor");
    right_motor = wb_robot_get_device("right wheel motor");
    motor_speeds[LEFT] = 0.0;
    motor_speeds[RIGHT] = 0.0;
}

void locomotion_set_velocity(double left_speed, double right_speed)
{
    motor_speeds[LEFT] = left_speed;
    motor_speeds[RIGHT] = right_speed;
}

void locomotion_update(void)
{
    wb_motor_set_velocity(left_motor, motor_speeds[LEFT]);
    wb_motor_set_velocity(right_motor, motor_speeds[RIGHT]);
}

void locomotion_cleanup(void)
{
    // Cleanup locomotion
// --- Braitenberg e controle de movimento ---
#define MAX_SPEED 6.28
    static double speeds[2] = {0.0, 0.0};
    static double weights[8][2] = {
        {-1.3, -1.0}, {-1.3, -1.0}, {-0.5, 0.5}, {0.0, 0.0}, {0.0, 0.0}, {0.05, -0.5}, {-0.75, 0}, {-0.75, 0}};
    static double offsets[2] = {0.5 * MAX_SPEED, 0.5 * MAX_SPEED};

    void run_braitenberg(double *distance_values)
    {
        for (int i = 0; i < 2; i++)
        {
            speeds[i] = 0.0;
            for (int j = 0; j < 8; j++)
                speeds[i] += distance_values[j] * weights[j][i];

            speeds[i] = offsets[i] + speeds[i] * MAX_SPEED;
            if (speeds[i] > MAX_SPEED)
                speeds[i] = MAX_SPEED;
            else if (speeds[i] < -MAX_SPEED)
                speeds[i] = -MAX_SPEED;
        }
    }

    void go_backwards(void)
    {
        locomotion_set_velocity(-MAX_SPEED, -MAX_SPEED);
        locomotion_update();
        passive_wait(0.2);
    }

    void turn_left(void)
    {
        locomotion_set_velocity(-MAX_SPEED, MAX_SPEED);
        locomotion_update();
        passive_wait(0.2);
    }

    void set_actuators(void)
    {
        locomotion_set_velocity(speeds[LEFT], speeds[RIGHT]);
        locomotion_update();
    }
}
