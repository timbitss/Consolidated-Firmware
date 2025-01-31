#include "states/App_DriveState.h"
#include "states/App_ChargeState.h"
#include "states/App_FaultState.h"
#include "states/App_AllStates.h"
#include "App_SharedMacros.h"

// clang-format off
#define NUM_OF_INVERTERS        (2U)
#define PRECHARGE_RESISTOR_OHMS (500U)
#define INV_CAPACITANCE_F       (0.280e-3f)
#define PRECHARGE_CAPACITANCE_F (INV_CAPACITANCE_F * NUM_OF_INVERTERS)
#define PRECHARGE_RC_MS         (S_TO_MS(PRECHARGE_RESISTOR_OHMS * PRECHARGE_CAPACITANCE_F))

// When TS reaches this threshold of ACC, AIR+ engages, and precharge is disabled, the voltage must be at least 90% EV.6.6.1
#define PRECHARGE_ACC_V_THRESHOLD (0.90f)

// 2.7RC corresponds to time to reach ~93% charged
#define PRECHARGE_COMPLETION_MS          ((float)PRECHARGE_RC_MS * 2.7f)
#define PRECHARGE_COMPLETION_LOWER_BOUND ((uint32_t)(PRECHARGE_COMPLETION_MS * 0.5f))
#define PRECHARGE_COMPLETION_UPPER_BOUND ((uint32_t)(PRECHARGE_COMPLETION_MS * 2.0f))

// clang-format on

static void PreChargeStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct BmsWorld *         world            = App_SharedStateMachine_GetWorld(state_machine);
    struct BmsCanTxInterface *can_tx_interface = App_BmsWorld_GetCanTx(world);
    struct Clock *            clock            = App_BmsWorld_GetClock(world);
    struct PrechargeRelay *   precharge_relay  = App_BmsWorld_GetPrechargeRelay(world);

    App_CanTx_SetPeriodicSignal_STATE(can_tx_interface, CANMSGS_BMS_STATE_MACHINE_STATE_PRE_CHARGE_CHOICE);

    App_SharedClock_SetPreviousTimeInMilliseconds(clock, App_SharedClock_GetCurrentTimeInMilliseconds(clock));
    App_PrechargeRelay_Close(precharge_relay);
}

static void PreChargeStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick1Hz(state_machine);
}

static void PreChargeStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    if (App_AllStatesRunOnTick100Hz(state_machine))
    {
        struct BmsWorld *      world       = App_SharedStateMachine_GetWorld(state_machine);
        struct Clock *         clock       = App_BmsWorld_GetClock(world);
        struct Airs *          airs        = App_BmsWorld_GetAirs(world);
        struct Accumulator *   accumulator = App_BmsWorld_GetAccumulator(world);
        struct Charger *       charger     = App_BmsWorld_GetCharger(world);
        struct TractiveSystem *ts          = App_BmsWorld_GetTractiveSystem(world);

        float ts_voltage        = App_TractiveSystem_GetVoltage(ts);
        float threshold_voltage = App_Accumulator_GetPackVoltage(accumulator) * PRECHARGE_ACC_V_THRESHOLD;

        uint32_t elapsed_time =
            App_SharedClock_GetCurrentTimeInMilliseconds(clock) - App_SharedClock_GetPreviousTimeInMilliseconds(clock);

        const bool is_ts_rising_slowly =
            (ts_voltage < threshold_voltage) && (elapsed_time >= PRECHARGE_COMPLETION_UPPER_BOUND);
        const bool is_ts_rising_quickly =
            (ts_voltage > threshold_voltage) && (elapsed_time <= PRECHARGE_COMPLETION_LOWER_BOUND);
        const bool is_charger_connected = App_Charger_IsConnected(charger);
        const bool has_precharge_fault =
            (is_charger_connected) ? is_ts_rising_slowly : (is_ts_rising_slowly | is_ts_rising_quickly);

        if (has_precharge_fault)
        {
            App_SharedStateMachine_SetNextState(state_machine, App_GetFaultState());
        }
        else if (ts_voltage >= threshold_voltage)
        {
            const struct State *next_state = (is_charger_connected) ? App_GetChargeState() : App_GetDriveState();
            App_Airs_CloseAirPositive(airs);
            App_SharedStateMachine_SetNextState(state_machine, next_state);
        }
    }
}

static void PreChargeStateRunOnExit(struct StateMachine *const state_machine)
{
    struct BmsWorld *      world           = App_SharedStateMachine_GetWorld(state_machine);
    struct PrechargeRelay *precharge_relay = App_BmsWorld_GetPrechargeRelay(world);

    App_PrechargeRelay_Open(precharge_relay);
}

const struct State *App_GetPreChargeState(void)
{
    static struct State pre_charge_state = {
        .name              = "PRE_CHARGE",
        .run_on_entry      = PreChargeStateRunOnEntry,
        .run_on_tick_1Hz   = PreChargeStateRunOnTick1Hz,
        .run_on_tick_100Hz = PreChargeStateRunOnTick100Hz,
        .run_on_exit       = PreChargeStateRunOnExit,
    };

    return &pre_charge_state;
}
