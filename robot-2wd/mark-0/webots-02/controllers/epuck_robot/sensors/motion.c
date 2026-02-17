/// Sensores de movimento: distância, posição, acelerômetro
#include <stdio.h>
#include <stdlib.h>
// Bibliotecas Webots
#include <webots/accelerometer.h>
#include <webots/device.h>
#include <webots/distance_sensor.h>
#include <webots/position_sensor.h>
#include <webots/robot.h>
// Bibliotecas do projeto
#include "motion.h"
#include "simulation.h"

// Sensores de distância: 8 infravermelhos normalizados 0.0-1.0
static WbDeviceTag accelerometer;
static WbDeviceTag distance_sensors[8];
static WbDeviceTag left_position_sensor;
static WbDeviceTag right_position_sensor;

static double distance_sensors_values[8];
static double position_values[2];
static double accelerometer_values[3];

static const char *distance_sensors_names[8] = {"ps0", "ps1", "ps2", "ps3", "ps4", "ps5", "ps6", "ps7"};

// ============== Distance Sensor (Proximity) ==============
// Distance sensors: init/update/read/cleanup + auxiliares
/// Inicializa 8 sensores de distância infravermelha
void distance_init(void)
{
    for (int i = 0; i < 8; i++)
    {
        distance_sensors[i] = wb_robot_get_device(distance_sensors_names[i]);
        wb_distance_sensor_enable(distance_sensors[i], simulation_get_time_step());
        distance_sensors_values[i] = 0.0;
    }
}

/// Copia valores de distância para o array fornecido
void distance_read(double *distance_value)
{
    if (distance_value)
    {
        for (int i = 0; i < 8; i++)
        {
            distance_value[i] = distance_sensors_values[i];
        }
    }
}

/// Lê valores brutos do Webots e normaliza para 0.0-1.0
void distance_update(void)
{
    for (int i = 0; i < 8; i++)
    {
        distance_sensors_values[i] = wb_distance_sensor_get_value(distance_sensors[i]);
        distance_sensors_values[i] /= 4096.0; // Scale to 0.0-1.0
    }
}

/// Desabilita sensores
void distance_cleanup(void)
{
    for (int i = 0; i < 8; i++)
    {
        wb_distance_sensor_disable(distance_sensors[i]);
    }
}

/// Retorna soma bruta dos 8 sensores (0-32768)
double distance_get_sum(void)
{
    double sum = 0.0;
    for (int i = 0; i < 8; i++)
    {
        sum += wb_distance_sensor_get_value(distance_sensors[i]);
    }
    return sum;
}

// Infrared: delegado para distance_* (mesmo hardware)

// ============== Accelerometer Implementation ==============
/// Inicializa acelerômetro de 3 eixos
void accelerometer_init(void)
{
    accelerometer = wb_robot_get_device("accelerometer");
    wb_accelerometer_enable(accelerometer, simulation_get_time_step());

    accelerometer_values[0] = 0.0;
    accelerometer_values[1] = 0.0;
    accelerometer_values[2] = 0.0;
}

/// Copia valores de aceleração (X, Y, Z em m/s²) para ponteiros
void accelerometer_read(double *x, double *y, double *z)
{
    if (x)
        *x = accelerometer_values[0];
    if (y)
        *y = accelerometer_values[1];
    if (z)
        *z = accelerometer_values[2];
}

/// Lê aceleração bruta do Webots (m/s²)
void accelerometer_update(void)
{
    const double *values = wb_accelerometer_get_values(accelerometer);
    accelerometer_values[0] = values[0];
    accelerometer_values[1] = values[1];
    accelerometer_values[2] = values[2];
}

/// Desabilita acelerômetro
void accelerometer_cleanup(void)
{
    wb_accelerometer_disable(accelerometer);
}

// ============== Position Sensor Implementation ==============
/// Inicializa sensores de posição (encoders das rodas)
void position_init(void)
{
    left_position_sensor = wb_robot_get_device("left wheel sensor");
    right_position_sensor = wb_robot_get_device("right wheel sensor");

    wb_position_sensor_enable(left_position_sensor, simulation_get_time_step());
    wb_position_sensor_enable(right_position_sensor, simulation_get_time_step());

    position_values[0] = 0.0;
    position_values[1] = 0.0;
}

/// Copia posição de encoders (esquerda/direita em radianos) para ponteiros
void position_read(double *x, double *y, double *z)
{
    if (x)
        *x = position_values[0];
    if (y)
        *y = position_values[1];
    if (z)
        *z = 0.0;
}

/// Lê posição bruta dos encoders do Webots (radianos)
void position_update(void)
{
    position_values[0] = wb_position_sensor_get_value(left_position_sensor);
    position_values[1] = wb_position_sensor_get_value(right_position_sensor);
}

/// Desabilita encoders
void position_cleanup(void)
{
    wb_position_sensor_disable(left_position_sensor);
    wb_position_sensor_disable(right_position_sensor);
}

// ============== Accessors ==============
/// Retorna buffer com valores dos 8 sensores de distância (0.0-1.0)
double *distance_get_values(void)
{
    return distance_sensors_values;
}

// ============== Odometry Calculation ==============
/// Calcula odometria: distância percorrida e mudança de orientação
void compute_odometry(double left_position, double right_position)
{
    double dl = left_position * WHEEL_RADIUS;  // distance covered by left wheel in meter
    double dr = right_position * WHEEL_RADIUS; // distance covered by right wheel in meter
    double da = (dr - dl) / AXLE_LENGTH;       // delta orientation

    printf("estimated distance covered by left wheel: %g m.\n", dl);
    printf("estimated distance covered by right wheel: %g m.\n", dr);
    printf("estimated change of orientation: %g rad.\n", da);
}