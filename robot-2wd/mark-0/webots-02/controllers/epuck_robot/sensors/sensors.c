/// Hub centralizado de gerenciamento de sensores com ativação seletiva por bitmask
#include <stdint.h>  // Bibliotecas padrão C
#include "sensors.h" // Bibliotecas do projeto

/// Máscara de sensores ativos (bitmask)
static uint8_t sensor_mask = 0x00;

/// Inicializa os sensores selecionados pela máscara de bits
void sensors_init(uint8_t sensors_mask)
{
    sensor_mask = sensors_mask; // armazena a máscara globalmente para uso nas demais funções

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

/// Atualiza o estado dos sensores ativados
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

/// Lê valores dos sensores ativados para buffers
void sensors_read(void)
{
    if (sensor_mask & SENSOR_DISTANCE)
    {
        double *dist_values = distance_get_values();
        if (dist_values)
            distance_read(dist_values);
    }
    if (sensor_mask & SENSOR_INFRA_RED)
    {
        double *ir_values = distance_get_values();
        if (ir_values)
            infra_red_read(ir_values);
    }
    if (sensor_mask & SENSOR_ACCELEROMETER)
    {
        double accel[3] = {0.0, 0.0, 0.0};
        accelerometer_read(accel, accel + 1, accel + 2);
    }
    if (sensor_mask & SENSOR_POSITION)
    {
        double pos[3] = {0.0, 0.0, 0.0};
        position_read(pos, pos + 1, pos + 2);
    }
    if (sensor_mask & SENSOR_GROUND)
    {
        double *ground_vals = ground_get_values();
        if (ground_vals)
            ground_read(ground_vals);
    }
    if (sensor_mask & SENSOR_LIGHT)
    {
        double light[8] = {0.0};
        light_read(light);
    }
    if (sensor_mask & SENSOR_CAMERA)
    {
        camera_capture();
        camera_process();
    }
    if (sensor_mask & SENSOR_CUSTOM)
        custom_sensor_read();
}

/// Libera recursos dos sensores ativados
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

/// Detecta precipício/linha usando sensores de chão (threshold < 500)
int cliff_detected(void)
{
    double *ground_vals = ground_get_values();
    if (!ground_vals)
        return 0; // se ground_values não está alocado, não há cliff

    for (int i = 0; i < 3; i++)
    {
        if (ground_vals[i] < 500.0)
            return 1; // true
    }
    return 0; // false
}

/// Inicializa sensor infrared (infravermelhos)
/*void infra_red_init(void)
{
    // Infra-red é essencialmente o mesmo que o sensor de distância
    // Inicializado junto com distance_init()
}


/// Cópia de valores de proximidade infravermelha (mesmo que distance_read)
void infra_red_read(double *ir_value)
{
    if (!ir_value)
        return;

    double *dist = distance_get_values();
    if (dist)
    {
        for (int i = 0; i < 8; i++)
            ir_value[i] = dist[i];
    }
    else
    {
        for (int i = 0; i < 8; i++)
            ir_value[i] = 0.0;
    }
}

/// Atualiza sensor infrared
void infra_red_update(void)
{
    distance_update(); // Usa a mesma atualização de distância
}

/// Limpa sensor infrared (limpeza em distance_cleanup)
void infra_red_cleanup(void)
{
    // Limpeza feita em distance_cleanup()
}
*/

/// Inicializa sensor de posição (odometria)
void position_init(void)
{
    // Inicialização integrada com motion.c
}

/// Lê valores de posição dos encoders (X:esq, Y:dir em radianos)
void position_read(double *x, double *y, double *z)
{
    if (x)
        *x = 0.0;
    if (y)
        *y = 0.0;
    if (z)
        *z = 0.0;
    // Valores reais viriam de motion.c
}

/// Atualiza sensores de posição
void position_update(void)
{
    // Atualização feita em motion.c
}

/// Limpa sensores de posição
void position_cleanup(void)
{
    // Limpeza feita em motion.c
}

/// Inicializa 8 sensores de luz
void light_init(void)
{
    // Inicialização dele já feita em environmental.c
}

/// Cópia de valores dos 8 sensores de luz
void light_read(double *light_value)
{
    if (light_value)
    {
        // Valores reais viriam de environmental.c
        for (int i = 0; i < 8; i++)
            light_value[i] = 0.0;
    }
}

/// Atualiza sensores de luz
void light_update(void)
{
    // Atualização feita em environmental.c
}

/// Limpa sensores de luz
void light_cleanup(void)
{
    // Limpeza feita em environmental.c
}

/// Inicializa 3 sensores de chão
void ground_init(void)
{
    // Inicialização feita em environmental.c
}

/// Cópia de valores dos 3 sensores de chão
void ground_read(double *ground_value)
{
    if (!ground_value)
        return;

    double *g = ground_get_values();
    if (g)
    {
        for (int i = 0; i < 3; i++)
            ground_value[i] = g[i];
    }
    else
    {
        for (int i = 0; i < 3; i++)
            ground_value[i] = 0.0;
    }
}

/// Atualiza sensores de chão
void ground_update(void)
{
    // Atualização feita em environmental.c
}

/// Limpa sensores de chão
void ground_cleanup(void)
{
    // Limpeza feita em environmental.c
}

/// Inicializa acelerômetro triaxial
void accelerometer_init(void)
{
    // Inicialização feita em motion.c e environmental.c
}

/// Lê valores do acelerômetro (X, Y, Z em m/s²)
void accelerometer_read(double *x, double *y, double *z)
{
    if (x)
        *x = 0.0;
    if (y)
        *y = 0.0;
    if (z)
        *z = 0.0;
    // Valores reais viriam de motion.c ou environmental.c
}

/// Atualiza acelerômetro
void accelerometer_update(void)
{
    // Atualização feita em motion.c e environmental.c
}

/// Limpa acelerômetro
void accelerometer_cleanup(void)
{
    // Limpeza feita em motion.c e environmental.c
}

// ============== Custom Sensor ==============
/*
/// Inicializa sensor customizado (framework para sensores futuros)
void custom_sensor_init(void)
{
    // Placeholder para sensores customizados futuros
}

/// Lê dados do sensor customizado (stub)
void custom_sensor_read(void)
{
    // Placeholder para sensores customizados futuros
}

/// Atualiza sensor customizado (stub)
void custom_sensor_update(void)
{
    // Placeholder para sensores customizados futuros
}

/// Limpa sensor customizado (stub)
void custom_sensor_cleanup(void)
{
    // Placeholder para sensores customizados futuros
}
*/