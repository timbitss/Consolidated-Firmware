#include "states/App_DriveState.h"
#include "states/App_ChargeState.h"
#include "states/App_FaultState.h"
#include "states/App_AllStates.h"

#include "App_BmsWorld.h"
#include "App_TractiveSystem.h"

#define PRECHARGE_THRESHOLD (0.90f) // EV.6.6.1: at least 90%
#define PRECHARGE_TIME_LOWER_BOUND_PERCENTAGE (0.10f)
#define PRECHARGE_TIME_UPPER_BOUND_PERCENTAGE (1.50f)
#define PRECHARGE_RESISTANCE_OHMS (1000U)

// COEFFICIENT_TO_THRESHOLD is the result of solving the exponential (for time
// to threshold) divided by RC (i.e. 4 would correspond to 98%)
#define COEFFICIENT_TO_THRESHOLD (2.30f)

static void PreChargeStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct BmsWorld *         world            = App_SharedStateMachine_GetWorld(state_machine);
    struct BmsCanTxInterface *can_tx_interface = App_BmsWorld_GetCanTx(world);
    struct Clock *            clock            = App_BmsWorld_GetClock(world);
    struct PrechargeRelay *   precharge_relay  = App_BmsWorld_GetPrechargeRelay(world);
    App_CanTx_SetPeriodicSignal_STATE(can_tx_interface, CANMSGS_BMS_STATE_MACHINE_STATE_PRE_CHARGE_CHOICE);

    // Enable precharge relay
    App_PrechargeRelay_Close(precharge_relay);

    // set the start time for precharge
    App_SharedClock_SetPreviousTimeInMilliseconds(clock, App_SharedClock_GetCurrentTimeInMilliseconds(clock));
}

static void PreChargeStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick1Hz(state_machine);
}

static void PreChargeStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick100Hz(state_machine);

    struct BmsWorld *      world           = App_SharedStateMachine_GetWorld(state_machine);
    struct Clock *         clock           = App_BmsWorld_GetClock(world);
    struct Airs *          airs            = App_BmsWorld_GetAirs(world);
    struct Accumulator *   accumulator     = App_BmsWorld_GetAccumulator(world);
    struct TractiveSystem *tractive_system = App_BmsWorld_GetTractiveSystem(world);
    struct Charger *       charger         = App_BmsWorld_GetCharger(world);

    // lower and upper bounds on expected charge time. Used to recognize faults
    const uint32_t lower_precharge_time = (uint32_t)(
        COEFFICIENT_TO_THRESHOLD * PRECHARGE_RESISTANCE_OHMS * App_TractiveSystem_GetSystemCapacitance() *
        PRECHARGE_TIME_LOWER_BOUND_PERCENTAGE);
    const uint32_t upper_precharge_time = (uint32_t)(
        COEFFICIENT_TO_THRESHOLD * PRECHARGE_RESISTANCE_OHMS * App_TractiveSystem_GetSystemCapacitance() *
        PRECHARGE_TIME_UPPER_BOUND_PERCENTAGE);

    // Voltage information
    float ts_voltage        = App_TractiveSystem_GetSystemVoltage(tractive_system);
    float threshold_voltage = App_Accumulator_GetPackVoltage(accumulator) * PRECHARGE_THRESHOLD;

    // Time since precharge started
    uint32_t elapsed_time =
        App_SharedClock_GetCurrentTimeInMilliseconds(clock) - App_SharedClock_GetPreviousTimeInMilliseconds(clock);

    // CHARGE MODE
    if (App_Charger_IsConnected(charger))
    {
        // if the charger is connected, we want to ignore the case where
        // pre-charge happens too quickly we expect an OC in this case
        if ((ts_voltage < threshold_voltage && elapsed_time >= upper_precharge_time))
        {
            // TODO: send non-crititcal fault message
            App_SharedStateMachine_SetNextState(state_machine, App_GetFaultState());
        }
        else if (ts_voltage >= threshold_voltage)
        {
            // if precharge happens within expected range enter
            // charge state
            App_Airs_CloseAirPositive(airs);
            App_SharedStateMachine_SetNextState(state_machine, App_GetChargeState());
        }
    }

    // DRIVE MODE
    else
    {
        // if the charger is disconnected, we want to watch for both cases of
        // pre-charging too quickly or too slowly

        if ((ts_voltage < threshold_voltage && elapsed_time >= upper_precharge_time) ||
            (ts_voltage > threshold_voltage && elapsed_time <= lower_precharge_time))
        {
            App_SharedStateMachine_SetNextState(state_machine, App_GetFaultState());
            // TODO: Send non-critical fault message
        }
        // if precharge happens within expected range, close air+ and enter
        // drive state
        else if (ts_voltage >= threshold_voltage)
        {
            App_Airs_CloseAirPositive(airs);
            App_SharedStateMachine_SetNextState(state_machine, App_GetDriveState());
        }
    }

    // if the AIR- opens again, go to fault state
    if (!App_SharedBinaryStatus_IsActive(App_Airs_GetAirNegative(airs)))
    {
        // TODO: send non-critical fault msg
        App_SharedStateMachine_SetNextState(state_machine, App_GetFaultState());
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
