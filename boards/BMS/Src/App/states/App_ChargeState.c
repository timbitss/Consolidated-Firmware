#include "states/App_AllStates.h"
#include "states/App_ChargeState.h"
#include "states/App_FaultState.h"

#include "App_SharedMacros.h"

#define TOTAL_CHARGE_TIME_S (3600U) // 1 hr

static void ChargeStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct BmsWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct BmsCanTxInterface *can_tx_interface = App_BmsWorld_GetCanTx(world);
    struct Charger *          charger          = App_BmsWorld_GetCharger(world);
    struct Clock *            clock            = App_BmsWorld_GetClock(world);

    App_Charger_Enable(charger); // Start charging process

    App_SharedClock_SetPreviousTimeInMilliseconds(
        clock, App_SharedClock_GetCurrentTimeInMilliseconds(clock));

    App_CanTx_SetPeriodicSignal_STATE(
        can_tx_interface, CANMSGS_BMS_STATE_MACHINE_STATE_CHARGE_CHOICE);
}

static void ChargeStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick1Hz(state_machine);
}

static void ChargeStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick100Hz(state_machine);

    struct BmsWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct BmsCanTxInterface *can_tx  = App_BmsWorld_GetCanTx(world);
    struct Charger *          charger = App_BmsWorld_GetCharger(world);
    struct Clock *            clock   = App_BmsWorld_GetClock(world);

    if (!App_Charger_IsConnected(charger))
    {
        App_CanTx_SetPeriodicSignal_CHARGER_DISCONNECTED_IN_CHARGE_STATE(
            can_tx, true);
        App_SharedStateMachine_SetNextState(state_machine, App_GetFaultState());
    }

    if (App_Charger_HasFaulted(charger))
    {
        App_CanTx_SetPeriodicSignal_CHARGER_FAULT_DETECTED(can_tx, true);
        App_SharedStateMachine_SetNextState(state_machine, App_GetFaultState());
    }

    // Track the time since accumulator charging started
    const uint32_t elapsed_time_s =
        App_SharedClock_GetCurrentTimeInSeconds(clock) -
        App_SharedClock_GetPreviousTimeInSeconds(clock);

    // Go to the fault state when charging has completed to indicate that
    // the charger should be disconnected and the BMS-Core should be
    // power-cycled
    if (elapsed_time_s >= TOTAL_CHARGE_TIME_S)
    {
        App_CanTx_SetPeriodicSignal_CHARGING_DONE(can_tx, true);
        App_SharedStateMachine_SetNextState(state_machine, App_GetFaultState());
    }
}

static void ChargeStateRunOnExit(struct StateMachine *const state_machine)
{
    struct BmsWorld *world   = App_SharedStateMachine_GetWorld(state_machine);
    struct Charger * charger = App_BmsWorld_GetCharger(world);

    App_Charger_Disable(charger); // Stop charging process
}

const struct State *App_GetChargeState(void)
{
    static struct State charge_state = {
        .name              = "CHARGE",
        .run_on_entry      = ChargeStateRunOnEntry,
        .run_on_tick_1Hz   = ChargeStateRunOnTick1Hz,
        .run_on_tick_100Hz = ChargeStateRunOnTick100Hz,
        .run_on_exit       = ChargeStateRunOnExit,
    };

    return &charge_state;
}
