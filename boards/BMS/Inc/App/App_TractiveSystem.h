#pragma once

struct TractiveSystem;

/**
 * Allocate and initialize a tractive system
 * @param get_ts_raw_voltage A function that can be called to get the raw
 * voltage for the TS voltage
 * @param get_ts_voltage A function that can be called to get the tractive
 * system voltage
 */
struct TractiveSystem *App_TractiveSystem_Create(float (*get_ts_raw_voltage)(void), float (*get_ts_voltage)(float));

/**
 * Deallocate the memory used by the given OK status
 * @param ok_status The OK status to deallocate
 */
void App_TractiveSystem_Destroy(struct TractiveSystem *ts);

/**
 * Get the TS voltage
 * @param ts The given tractive system
 * @return The voltage in (V)
 */
float App_TractiveSystem_GetVoltage(struct TractiveSystem *ts);

/**
 * Get the voltage from the Tractive System
 * @param tractive_system The Tractive System to get the voltage from
 * @return The voltage of the given Tractive System
 */
float App_TractiveSystem_GetSystemVoltage(const struct TractiveSystem *ts);

/**
 * Get the capacitance on the Tractive System
 * @param tractive_system The Tractive System to get the voltage from
 * @return The capacitance of the given Tractive System
 */
float App_TractiveSystem_GetSystemCapacitance(void);
