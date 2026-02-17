#ifndef FEATURES_H
#define FEATURES_H

#include <stdbool.h>

// Features module main header
// Consolidated control hub for all robot features

// ============== Output Features ==============
// Actuators
void actuators_init(void);
void actuators_update(void);
void actuators_cleanup(void);

// LEDs
void leds_init(void);
void leds_set(int led_id, int state);
void leds_update(void);
void leds_cleanup(void);
void reset_actuator_values(void);
void blink_leds(void);
void leds_set_by_accelerometer(double accel_x, double accel_y);
void leds_clear_all(void);

// Audio
void audio_init(void);
void audio_play(void);
void audio_stop(void);
void audio_cleanup(void);

// ============== Communication Features ==============
// Bluetooth
void bluetooth_init(void);
void bluetooth_send(const char *data);
void bluetooth_receive(void);
void bluetooth_cleanup(void);

// API (TCP/IP Socket)
#define API_DEFAULT_PORT 1000 // Porta padrão do e-puck2 para comunicação TCP/IP

int api_init(int port);
int api_accept_connection(void);
int api_process_command(int fd, unsigned char *command_buffer);
bool api_send_sensors(int fd, unsigned char *sensors_buffer);
bool api_send_image(int fd, unsigned char *image_buffer);
void api_cleanup(void);

// ============== Features Control Hub ==============
void features_init(void);
void features_update(void);
void features_cleanup(void);

#endif // FEATURES_H
