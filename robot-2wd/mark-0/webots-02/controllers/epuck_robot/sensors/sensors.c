#include "sensors.h"
#include <stdint.h>

// --- Variáveis globais para sensores ---
double distance_values[8] = {0.0};
double ground_values[3] = {0.0};

/**
 * Inicializa os sensores selecionados pela máscara.
 * @param sensors_mask Máscara de bits indicando quais sensores ativar.
 */
void sensors_init(uint8_t sensors_mask)
{
    sensor_mask = sensors_mask;

    if (sensor_mask & SENSOR_DISTANCE)
        distance_init();
    if (sensor_mask & SENSOR_INFRA_RED)
        infra_red_init();
    if (sensor_mask & SENSOR_ACCELEROMETER)
        accelerometer_init();
    if (sensor_mask & SENSOR_POSITION)
        position_init();
    if (sensor_mask & SENSOR_CAMERA)
        camera_init();
    if (sensor_mask & SENSOR_LIGHT)
        light_init();
    if (sensor_mask & SENSOR_GROUND)
        ground_init();
    if (sensor_mask & SENSOR_CUSTOM)
        custom_sensor_init();
}

/**
 * Atualiza o estado dos sensores ativados.
 * Não requer máscara, usa a configuração definida em sensors_init.
 */
void sensors_update(void)
{
    if (sensor_mask & SENSOR_DISTANCE)
        distance_update();
    if (sensor_mask & SENSOR_INFRA_RED)
        infra_red_update();
    if (sensor_mask & SENSOR_ACCELEROMETER)
        accelerometer_update();
    if (sensor_mask & SENSOR_POSITION)
        position_update();
    if (sensor_mask & SENSOR_CAMERA)
        camera_update();
    if (sensor_mask & SENSOR_LIGHT)
        light_update();
    if (sensor_mask & SENSOR_GROUND)
        ground_update();
    if (sensor_mask & SENSOR_CUSTOM)
        custom_sensor_update();
}

/**
 * Lê os valores dos sensores ativados.
 * Não requer máscara, usa a configuração definida em sensors_init.
 */
void sensors_read(void)
{
    if (sensor_mask & SENSOR_DISTANCE)
        distance_read(distance_values);
    if (sensor_mask & SENSOR_INFRA_RED)
        infra_red_read();
    if (sensor_mask & SENSOR_ACCELEROMETER)
        accelerometer_read();
    if (sensor_mask & SENSOR_POSITION)
        position_read();
    if (sensor_mask & SENSOR_GROUND)
        ground_read(ground_values);
    if (sensor_mask & SENSOR_LIGHT)
        light_read();
    if (sensor_mask & SENSOR_CAMERA)
    {
        camera_capture();
        camera_process();
    }
    if (sensor_mask & SENSOR_CUSTOM)
        custom_sensor_read();
}

/**
 * Libera recursos dos sensores ativados.
 * Não requer máscara, usa a configuração definida em sensors_init.
 */
void sensors_cleanup(void)
{
    if (sensor_mask & SENSOR_DISTANCE)
        distance_cleanup();
    if (sensor_mask & SENSOR_INFRA_RED)
        infra_red_cleanup();
    if (sensor_mask & SENSOR_ACCELEROMETER)
        accelerometer_cleanup();
    if (sensor_mask & SENSOR_POSITION)
        position_cleanup();
    if (sensor_mask & SENSOR_CAMERA)
        camera_cleanup();
    if (sensor_mask & SENSOR_LIGHT)
        light_cleanup();
    if (sensor_mask & SENSOR_GROUND)
        ground_cleanup();
    if (sensor_mask & SENSOR_CUSTOM)
        custom_sensor_cleanup();
}

// --- Funções auxiliares ---
void get_sensor_input(void)
{
    distance_read(distance_values);
    ground_read(ground_values);
}

int cliff_detected(void)
{
    for (int i = 0; i < 3; i++)
    {
        if (ground_values[i] < 500.0)
            return 1; // true
    }
    return 0; // false
}
