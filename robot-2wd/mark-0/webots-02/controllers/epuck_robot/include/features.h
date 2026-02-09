#ifndef FEATURES_H
#define FEATURES_H

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

// Audio
void audio_init(void);
void audio_play(void);
void audio_stop(void);
void audio_cleanup(void);

// ============== Communication Features ==============
void bluetooth_init(void);
void bluetooth_send(const char *data);
void bluetooth_receive(void);
void bluetooth_cleanup(void);

// ============== Features Control Hub ==============
void features_init(void);
void features_update(void);
void features_cleanup(void);

#endif // FEATURES_H
