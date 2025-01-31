#include "states/App_AllStates.h"
#include "states/App_DriveState.h"
#include "states/App_FaultState.h"
#include "App_SetPeriodicCanSignals.h"
#include "App_SharedMacros.h"

static void DriveStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct BmsWorld *         world            = App_SharedStateMachine_GetWorld(state_machine);
    struct BmsCanTxInterface *can_tx_interface = App_BmsWorld_GetCanTx(world);
    App_CanTx_SetPeriodicSignal_STATE(can_tx_interface, CANMSGS_BMS_STATE_MACHINE_STATE_DRIVE_CHOICE);
}

static void DriveStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick1Hz(state_machine);
}

static void DriveStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    if (App_AllStatesRunOnTick100Hz(state_machine))
    {
        struct BmsWorld *         world  = App_SharedStateMachine_GetWorld(state_machine);
        struct BmsCanTxInterface *can_tx = App_BmsWorld_GetCanTx(world);
        struct Imd *              imd    = App_BmsWorld_GetImd(world);
        struct Airs *             airs   = App_BmsWorld_GetAirs(world);

        App_SetPeriodicCanSignals_Imd(can_tx, imd);

        if (!App_Airs_IsAirNegativeClosed(airs))
        {
            // if AIR- opens, go back to fault state (AIR+ will be opened there)
            App_SharedStateMachine_SetNextState(state_machine, App_GetFaultState());
        }
    }
}

static void DriveStateRunOnExit(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}

const struct State *App_GetDriveState(void)
{
    static struct State drive_state = {
        .name              = "DRIVE",
        .run_on_entry      = DriveStateRunOnEntry,
        .run_on_tick_1Hz   = DriveStateRunOnTick1Hz,
        .run_on_tick_100Hz = DriveStateRunOnTick100Hz,
        .run_on_exit       = DriveStateRunOnExit,
    };

    return &drive_state;
}
