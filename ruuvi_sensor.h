/*
 * Common interface to all Ruuvi Sensors
 * Every sensor must implement these functions:
 * - init
 * - unint
 * - samplerate_set
 * - samplerate_get
 * - dsp_set
 * - dsp_get
 * - scale_set
 * - scale_set
 * - resolution_set
 * - resolution_get
 * - mode_set
 * - mode_get
 * - interrupt_set
 * - interrupt_get
 * - data_get
 *
 * If function does not make sense for the sensor, it will return error code.
 *
 * INIT, UNINT: Init will prepare sensor for use, reset, run self-test and place it in low-power mode
 *              Uninit will release any resources used by sensor
 *
 * Samplerate: Applicable on continous mode, how often sensor takes samples. Hz
 *
 * DSP: DSP function and parameter, i.e. "OVERSAMPLING, 16". Return error if device does not support it.
 * 
 * scale: Maximum scale in meaningful physical unit
 * 
 * resolution: Resolution in bits
 *
 * mode: Sleep, single_blocking, single_asynchronous, continous
 *
 * interrupt_set: interrupt number, upper bound, lowerbound, (ABOVE, BELOW, BETWEEN, OUTSIDE), DSP
 *
 * data get: return data from sensor
 */ 

#ifndef RUUVI_SENSOR_H
#define RUUVI_SENSOR_H
#include "ruuvi_error.h"



typedef enum {
  RUUVI_SENSOR_SAMPLERATE_STOP      = 0,   //Stop sampling
  RUUVI_SENSOR_SAMPLERATE_SINGLE    = 251, //Take a single sample
  RUUVI_SENSOR_SAMPLERATE_MIN       = 252, //Minimum
  RUUVI_SENSOR_SAMPLERATE_MAX       = 253, //Maximum
  RUUVI_SENSOR_SAMPLERATE_NO_CHANGE = 255
}ruuvi_sensor_samplerate_t;

/** 
 * Resolution. Other values are bits
 */
typedef enum {
  RUUVI_SENSOR_RESOLUTION_MIN        = 251,
  RUUVI_SENSOR_RESOLUTION_MAX        = 252,
  RUUVI_SENSOR_RESOLUTION_NO_CHANGE  = 255
}ruuvi_sensor_resolution_t;

/**
 * Scale. Other values are in physical unit, i.e. gravity or volt
 */
typedef enum {
  RUUVI_SENSOR_SCALE_MIN        = 251,
  RUUVI_SENSOR_SCALE_MAX        = 252,
  RUUVI_SENSOR_SCALE_NO_CHANGE  = 255
}ruuvi_sensor_scale_t;

/**
 *. DSP function
 */
typedef enum {
  RUUVI_SENSOR_DSP_LAST      = 0,
  RUUVI_SENSOR_DSP_MIN       = (1<<0),
  RUUVI_SENSOR_DSP_MAX       = (1<<1),
  RUUVI_SENSOR_DSP_AVERAGE   = (1<<2),
  RUUVI_SENSOR_DSP_STDEV     = (1<<3),
  RUUVI_SENSOR_DSP_IMPULSE   = (1<<4),
  RUUVI_SENSOR_DSP_LOW_PASS  = (1<<5),
  RUUVI_SENSOR_DSP_HIGH_PASS = (1<<6),
  RUUVI_SENSOR_DSP_IIR       = (1<<7),
  RUUVI_SENSOR_DSP_OS        = (1<<8)
}ruuvi_sensor_dsp_function_t;

typedef enum {
  RUUVI_SENSOR_MODE_SLEEP               = 0,
  RUUVI_SENSOR_MODE_SINGLE_BLOCKING     = 1,
  RUUVI_SENSOR_MODE_SINGLE_ASYNCHRONOUS = 2,
  RUUVI_SENSOR_MODE_CONTINOUS           = 3,
  RUUVI_SENSOR_MODE_INVALID             = 10
}ruuvi_sensor_mode_t;

/**
 * Interrupts can be latched or pulsed, depends on sensor
 */
typedef enum {
  RUUVI_SENSOR_ABOVE   = 0,
  RUUVI_SENSOR_BELOW   = 1,
  RUUVI_SENSOR_BETWEEN = 2,
  RUUVI_SENSOR_OUTSIDE = 3
}ruuvi_sensor_trigger_t;

// Declare function pointers common to all sensors
// Init and uninit, take no parameters
typedef ruuvi_status_t(*ruuvi_sensor_init_fp)(void);
// Setters, getters, sent to sensor on set, copied to pointer on get
typedef ruuvi_status_t(*ruuvi_sensor_samplerate_fp)(ruuvi_sensor_samplerate_t*);
typedef ruuvi_status_t(*ruuvi_sensor_resolution_fp)(ruuvi_sensor_resolution_t*);
typedef ruuvi_status_t(*ruuvi_sensor_scale_fp)(ruuvi_sensor_scale_t*);
typedef ruuvi_status_t(*ruuvi_sensor_dsp_fp)(ruuvi_sensor_dsp_function_t*, uint8_t*);
typedef ruuvi_status_t(*ruuvi_sensor_mode_fp)(ruuvi_sensor_mode_t*);
// Interrupt numbers, threshold, trigger, dsp before interrupt
typedef ruuvi_status_t(*ruuvi_sensor_interrupt_fp)(uint8_t, float*, ruuvi_sensor_trigger_t*, ruuvi_sensor_dsp_function_t*);

// Void pointer to sensor-specific struct which gets filled with data
typedef ruuvi_status_t(*ruuvi_sensor_data_fp)(void*);

typedef struct 
{
  ruuvi_sensor_init_fp init;
  ruuvi_sensor_init_fp uninit;
  ruuvi_sensor_samplerate_fp samplerate_set;
  ruuvi_sensor_samplerate_fp samplerate_get;
  ruuvi_sensor_resolution_fp resolution_set;
  ruuvi_sensor_resolution_fp resolution_get;
  ruuvi_sensor_scale_fp      scale_set;
  ruuvi_sensor_scale_fp      scale_get;
  ruuvi_sensor_dsp_fp        dsp_set;
  ruuvi_sensor_dsp_fp        dsp_get;
  ruuvi_sensor_mode_fp       mode_set;
  ruuvi_sensor_mode_fp       mode_get;
  ruuvi_sensor_interrupt_fp  interrupt_set;
  ruuvi_sensor_interrupt_fp  interrupt_get;
  ruuvi_sensor_data_fp       data_get;
}ruuvi_sensor_t;

#endif