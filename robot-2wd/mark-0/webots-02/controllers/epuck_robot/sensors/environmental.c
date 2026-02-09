#include "environmental.h"
#include "simulation.h"

#include <webots/device.h>
#include <webots/distance_sensor.h>
#include <webots/robot.h>

// ============== Camera Implementation ==============
void camera_init(void)
{
    // Initialize camera
}

void camera_capture(void)
{
    // Capture image from camera
}

void camera_process(void)
{
    // Process camera image
}

void camera_update(void)
{
    // Update camera
}

void camera_cleanup(void)
{
    // Cleanup camera
}

// ============== Ground Sensors ==============
#define GROUND_SENSORS_NUMBER 3
static WbDeviceTag ground_sensors[GROUND_SENSORS_NUMBER];
static double ground_sensors_values[GROUND_SENSORS_NUMBER] = {0.0, 0.0, 0.0};
static const char *ground_sensors_names[GROUND_SENSORS_NUMBER] = {"gs0", "gs1", "gs2"};

// ============== Light Sensor Implementation ==============
void light_init(void)
{
    // Initialize light sensor
}

void light_read(double *light_value)
{
    // Read light sensor
}

void light_update(void)
{
    // Update light sensor
}

void light_cleanup(void)
{
    // Cleanup light sensor
}

// ============== Ground Sensor Implementation ==============
void ground_init(void)
{
    int i;
    for (i = 0; i < GROUND_SENSORS_NUMBER; i++)
    {
        ground_sensors[i] = wb_robot_get_device(ground_sensors_names[i]);
        if (ground_sensors[i])
        {
            wb_distance_sensor_enable(ground_sensors[i], simulation_get_time_step());
        }
        ground_sensors_values[i] = 0.0;
    }
}

void ground_read(double *ground_value)
{
    if (ground_value)
    {
        int i;
        for (i = 0; i < GROUND_SENSORS_NUMBER; i++)
        {
            ground_value[i] = ground_sensors_values[i];
        }
    }
}

void ground_update(void)
{
    int i;
    for (i = 0; i < GROUND_SENSORS_NUMBER; i++)
    {
        if (ground_sensors[i])
        {
            ground_sensors_values[i] = wb_distance_sensor_get_value(ground_sensors[i]);
        }
    }
}

void ground_cleanup(void)
{
    int i;
    for (i = 0; i < GROUND_SENSORS_NUMBER; i++)
    {
        if (ground_sensors[i])
        {
            wb_distance_sensor_disable(ground_sensors[i]);
        }
    }
}
