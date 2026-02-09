// bibliotecas do c
#include <stdlib.h>

// bibliotecas do simulador
#include <webots/device.h>
#include <webots/distance_sensor.h>
#include <webots/led.h>
#include <webots/motor.h>
#include <webots/nodes.h>
#include <webots/robot.h>

int simulation_get_time_step()
{
    static int time_step = -1;
    if (time_step == -1)
        time_step = (int)wb_robot_get_basic_time_step();
    return time_step;
}

void step()
{
    if (wb_robot_step(simulation_get_time_step()) == -1)
    {
        wb_robot_cleanup();
        exit(EXIT_SUCCESS);
    }
}

void passive_wait(double sec)
{
    double start_time = wb_robot_get_time();
    do
    {
        step();
    } while (start_time + sec > wb_robot_get_time());
}

void simulation_init()
{
    wb_robot_init();
}

void simulation_update(void)
{
    // No-op for now (placeholder for future simulation logic)
}

void simulation_cleanup(void)
{
    wb_robot_cleanup();
}