void run_braitenberg(double *distance_values);
void go_backwards(void);
void turn_left(void);
void set_actuators(void);
#ifndef LOCOMOTION_H
#define LOCOMOTION_H

/**
 * Locomotion Module
 * Responsavel pelo controle de movimentacao do robo
 */

void locomotion_init(void);
void locomotion_set_velocity(double left_speed, double right_speed);
void locomotion_update(void);
void locomotion_cleanup(void);

#endif // LOCOMOTION_H
