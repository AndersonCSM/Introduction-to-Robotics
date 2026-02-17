#include <stdlib.h>
// Bibliotecas Webots
#include <webots/accelerometer.h>
#include <webots/camera.h>
#include <webots/device.h>
#include <webots/distance_sensor.h>
#include <webots/light_sensor.h>
#include <webots/robot.h>
// Bibliotecas do projeto
#include "environmental.h"
#include "simulation.h"

#define LIGHT_SENSORS_NUMBER 8
#define GROUND_SENSORS_NUMBER 3

static WbDeviceTag light_sensors[8];
static WbDeviceTag ground_sensors[3];
static WbDeviceTag camera;        // Câmera: dispositivo de imagem
static WbDeviceTag accelerometer; // Acelerômetro triaxial

static double light_sensors_values[8];
static double ground_sensors_values[3];

static const char *light_sensors_names[LIGHT_SENSORS_NUMBER] = {"ls0", "ls1", "ls2", "ls3", "ls4", "ls5", "ls6", "ls7"};
static const char *ground_sensors_names[GROUND_SENSORS_NUMBER] = {"gs0", "gs1", "gs2"};

/// Inicializa a câmera do e-puck (640x480 ou 64x64 conforme modelo)
void camera_init(void)
{
    // Initialize camera with appropriate time step for robot model
    camera = wb_robot_get_device("camera");
    if (camera)
    {
        wb_camera_enable(camera, simulation_get_camera_time_step());
    }
}

/// Captura imagem do Webots (deve ser chamado antes de process/get_gray_sum)
void camera_capture(void)
{
    // Capture image from camera
}

/// Processa imagem capturada (deve ser chamado após capture)
void camera_process(void)
{
    // Process camera image
}

/// Atualiza estado da câmera a cada passo da simulação
void camera_update(void)
{
    // Update camera
}

/// Desabilita câmera e libera recursos
void camera_cleanup(void)
{
    // Cleanup camera
    if (camera)
    {
        wb_camera_disable(camera);
    }
}

/// Calcula soma de intensidade de cinza de toda a imagem
long int camera_get_gray_sum(void)
{
    if (!camera)
        return 0;

    long int sum = 0;
    const unsigned char *im = wb_camera_get_image(camera);
    int width = wb_camera_get_width(camera);
    int height = wb_camera_get_height(camera);

    for (int m = 0; m < width; m++)
    {
        for (int n = 0; n < height; n++)
            sum += wb_camera_image_get_gray(im, width, m, n);
    }

    return sum;
}

/// Retorna largura da imagem em pixels (640 ou 64 conforme modelo)
int camera_get_width(void)
{
    if (camera)
        return wb_camera_get_width(camera);
    else
        return 0;
}

/// Retorna altura da imagem em pixels (480 ou 64 conforme modelo)
int camera_get_height(void)
{
    if (camera)
        return wb_camera_get_height(camera);
    else
        return 0;
}

// ============== Accelerometer Implementation ==============
/// Inicializa acelerômetro triaxial (3 eixos: X, Y, Z)
void accelerometer_init(void)
{
    // Initialize accelerometer
    accelerometer = wb_robot_get_device("accelerometer");
    if (accelerometer)
    {
        wb_accelerometer_enable(accelerometer, simulation_get_time_step());
    }
}

/// Lê valores do acelerômetro para 3 ponteiros (X, Y, Z)
void accelerometer_read(double *accel_x, double *accel_y, double *accel_z)
{
    if (accelerometer && accel_x && accel_y && accel_z)
    {
        const double *values = wb_accelerometer_get_values(accelerometer);
        *accel_x = values[0];
        *accel_y = values[1];
        *accel_z = values[2];
    }
}

/// Atualiza acelerômetro
void accelerometer_update(void)
{
    // Update accelerometer
}

/// Desabilita acelerômetro
void accelerometer_cleanup(void)
{
    // Cleanup accelerometer
    if (accelerometer)
    {
        wb_accelerometer_disable(accelerometer);
    }
}

/// Inicializa 8 sensores de luz
void light_init(void)
{
    for (int i = 0; i < LIGHT_SENSORS_NUMBER; i++)
    {
        light_sensors[i] = wb_robot_get_device(light_sensors_names[i]);
        if (light_sensors[i])
        {
            wb_light_sensor_enable(light_sensors[i], 2 * simulation_get_time_step());
        }
        light_sensors_values[i] = 0.0;
    }
}

/// Copia valores dos 8 sensores de luz para array fornecido
void light_read(double *light_value)
{
    if (light_value)
    {
        for (int i = 0; i < LIGHT_SENSORS_NUMBER; i++)
        {
            light_value[i] = light_sensors_values[i];
        }
    }
}

/// Lê valores dos sensores de luz do Webots (deve ser chamado antes de light_read)
void light_update(void)
{
    for (int i = 0; i < LIGHT_SENSORS_NUMBER; i++)
    {
        if (light_sensors[i])
        {
            light_sensors_values[i] = wb_light_sensor_get_value(light_sensors[i]);
        }
    }
}

/// Desabilita sensores de luz
void light_cleanup(void)
{
    for (int i = 0; i < LIGHT_SENSORS_NUMBER; i++)
    {
        if (light_sensors[i])
        {
            wb_light_sensor_disable(light_sensors[i]);
        }
    }
}

/// Retorna soma dos 8 sensores de luz
double light_get_sum(void)
{
    double sum = 0.0;
    for (int i = 0; i < LIGHT_SENSORS_NUMBER; i++)
    {
        sum += light_sensors_values[i];
    }
    return sum;
}

// ============== Ground Sensor Implementation ==============
/// Inicializa 3 sensores de chão
void ground_init(void)
{
    for (int i = 0; i < GROUND_SENSORS_NUMBER; i++)
    {
        ground_sensors[i] = wb_robot_get_device(ground_sensors_names[i]);
        if (ground_sensors[i])
        {
            wb_distance_sensor_enable(ground_sensors[i], simulation_get_time_step());
        }
        ground_sensors_values[i] = 0.0;
    }
}

/// Copia valores dos 3 sensores de chão para array fornecido (0-1000+)
void ground_read(double *ground_value)
{
    if (ground_value)
    {
        for (int i = 0; i < GROUND_SENSORS_NUMBER; i++)
        {
            ground_value[i] = ground_sensors_values[i];
        }
    }
}

/// Lê valores dos sensores de chão do Webots (deve ser chamado antes de ground_read)
void ground_update(void)
{
    for (int i = 0; i < GROUND_SENSORS_NUMBER; i++)
    {
        if (ground_sensors[i])
        {
            ground_sensors_values[i] = wb_distance_sensor_get_value(ground_sensors[i]);
        }
    }
}

/// Desabilita sensores de chão
void ground_cleanup(void)
{
    for (int i = 0; i < GROUND_SENSORS_NUMBER; i++)
    {
        if (ground_sensors[i])
        {
            wb_distance_sensor_disable(ground_sensors[i]);
        }
    }
}

// ============== Accessors ==============
/// Retorna buffer com valores dos 3 sensores de chão (0-1000+)
double *ground_get_values(void)
{
    return ground_sensors_values;
}