#include <stdint.h>

#ifndef SENSORS_H
#define SENSORS_H

// Máscara de bits para os sensores
#define SENSOR_DISTANCE (1 << 0)      // 0000 0001
#define SENSOR_INFRA_RED (1 << 1)     // 0000 0010
#define SENSOR_ACCELEROMETER (1 << 2) // 0000 0100
#define SENSOR_POSITION (1 << 3)      // 0000 1000
#define SENSOR_CAMERA (1 << 4)        // 0001 0000
#define SENSOR_LIGHT (1 << 5)         // 0010 0000
#define SENSOR_GROUND (1 << 6)        // 0100 0000
#define SENSOR_CUSTOM (1 << 7)        // 1000 0000
// Combinações prontas
#define SENSORS_MOTION (SENSOR_DISTANCE | SENSOR_INFRA_RED | SENSOR_ACCELEROMETER | SENSOR_POSITION)
#define SENSORS_ENV (SENSOR_CAMERA | SENSOR_LIGHT | SENSOR_GROUND)
#define SENSORS_ALL (0xFF)

// Hub centralizado para todos os sensores do robô
// Distance Sensor (Proximity)
void distance_init(void);
void distance_read(double *distance_value);
void distance_update(void);
void distance_cleanup(void);

// Infrared Sensor
void infra_red_init(void);
void infra_red_read(double *ir_value);
void infra_red_update(void);
void infra_red_cleanup(void);

// Accelerometer
void accelerometer_init(void);
void accelerometer_read(double *x, double *y, double *z);
void accelerometer_update(void);
void accelerometer_cleanup(void);

// Position Sensor
void position_init(void);
void position_read(double *x, double *y, double *z);
void position_update(void);
void position_cleanup(void);

// Camera
void camera_init(void);
void camera_capture(void);
void camera_process(void);
void camera_update(void);
void camera_cleanup(void);

// Light Sensor
void light_init(void);
void light_read(double *light_value);
void light_update(void);
void light_cleanup(void);

// Ground Sensor
void ground_init(void);
void ground_read(double *ground_value);
void ground_update(void);
void ground_cleanup(void);

// --- Sensores utilitários ---
int cliff_detected(void);

// ============== Encapsulated Data Accessors ==============
/**
 * @brief Retorna ponteiro ao buffer encapsulado de valores de distância
 *
 * Fornece acesso seguro ao buffer interno de sensores de distância.
 * O buffer é mantido em motion.c e só existe se SENSOR_DISTANCE foi
 * inicializado via sensors_init().
 *
 * @return Ponteiro para array[8] com valores dos 8 sensores (0.0-1.0)
 *
 * @see distance_get_values() implementado em motion.c
 */
double *distance_get_values(void);

/**
 * @brief Retorna ponteiro ao buffer encapsulado de valores de chão
 *
 * Fornece acesso seguro ao buffer interno de sensores de chão.
 * O buffer é mantido em environmental.c e só existe se SENSOR_GROUND
 * foi inicializado via sensors_init().
 *
 * @return Ponteiro para array[3] com valores dos 3 sensores (0-1000+)
 *
 * @see ground_get_values() implementado em environmental.c
 */
double *ground_get_values(void);

// ============== Sensors Control Hub ==============
/**
 * @brief Inicializa sensores com base em uma máscara de bits.
 *
 * Esta função analisa cada bit da máscara fornecida e inicializa apenas
 * os sensores correspondentes aos bits ativos (1).
 *
 * Exemplo de uso:
 *
 * // Inicializa apenas o sensor de distância
 * sensors_init(SENSOR_DISTANCE);
 *
 * // Inicializa distância e infravermelho
 * sensors_init(SENSOR_DISTANCE | SENSOR_INFRA_RED);
 *
 * // Inicializa todos os sensores (notação hexadecimal)
 * sensors_init(0xFF);
 *
 * // Inicializa distância e infravermelho (notação binária)
 * sensors_init(0b00000011);
 *
 *
 * @param sensors_mask Máscara de bits que define quais sensores serão inicializados.
 */
void sensors_init(uint8_t sensors_mask);

/**
 * Atualiza o estado dos sensores ativados.
 * Não requer máscara, usa a configuração definida em sensors_init.
 */
void sensors_update(void);

/**
 * Lê os valores dos sensores ativados.
 * Não requer máscara, usa a configuração definida em sensors_init.
 */
void sensors_read(void);

/**
 * Libera recursos dos sensores ativados.
 * Não requer máscara, usa a configuração definida em sensors_init.
 */
void sensors_cleanup(void);

#endif // SENSORS_H
