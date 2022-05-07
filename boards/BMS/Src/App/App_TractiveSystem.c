#include <stdlib.h>
#include <assert.h>

#define NUM_INVERTERS (2U)
#define INVERTER_CAPACITANCE_F (0.280e-3f)
#define SYSTEM_CAPACITANCE (NUM_INVERTERS * INVERTER_CAPACITANCE_F)

struct TractiveSystem
{
    float (*get_raw_ts_voltage)(void);
    float (*get_ts_voltage)(float);
};

struct TractiveSystem *App_TractiveSystem_Create(float (*get_ts_raw_voltage)(void), float (*get_ts_voltage)(float))
{
    struct TractiveSystem *ts = malloc(sizeof(struct TractiveSystem));
    assert(ts != NULL);

    ts->get_raw_ts_voltage = get_ts_raw_voltage;
    ts->get_ts_voltage     = get_ts_voltage;

    return ts;
}

void App_TractiveSystem_Destroy(struct TractiveSystem *ts)
{
    free(ts);
}

float App_TractiveSystem_GetSystemVoltage(const struct TractiveSystem *ts)
{
    return ts->get_ts_voltage(ts->get_raw_ts_voltage());
}

float App_TractiveSystem_GetSystemCapacitance(void)
{
    return SYSTEM_CAPACITANCE;
}
