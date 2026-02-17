#ifndef MOTION_H
#define MOTION_H

// Motion and Proximity sensors module header
// Consolidates accelerometer, position, distance and infrared

// ============== Odometry Configuration ==============
#define WHEEL_RADIUS 0.02
#define AXLE_LENGTH 0.052
#define RANGE (1024 / 2)

// ============== Odometry ==============
void compute_odometry(double left_position, double right_position);

// ============== Distance Sensor (Proximity) ==============
void distance_init(void);
void distance_read(double *distance_value);
void distance_update(void);
void distance_cleanup(void);
double distance_get_sum(void);

// ============== Infrared Sensor ==============
void infra_red_init(void);
void infra_red_read(double *ir_value);
void infra_red_update(void);
void infra_red_cleanup(void);

// ============== Accelerometer ==============
void accelerometer_init(void);
void accelerometer_read(double *x, double *y, double *z);
void accelerometer_update(void);
void accelerometer_cleanup(void);

// ============== Position Sensor ==============
void position_init(void);
void position_read(double *x, double *y, double *z);
void position_update(void);
void position_cleanup(void);

#endif // MOTION_H
