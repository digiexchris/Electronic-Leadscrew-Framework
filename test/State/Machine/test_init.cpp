#include <gtest/gtest.h>
#include "State/Machine/Machine.hpp"
#include "State/Machine/Running.hpp"
#include "State/Machine/Idle.hpp"
#include "State/Machine/EStop.hpp"
#include "State/Machine/Messages.hpp"
#include <stdint.h>
#include <etl/vector.h>
#include <etl/utility.h>
#include <etl/iterator.h>
#include <etl/memory.h> // Include for etl::shared_ptr
#include <etl/message_packet.h>
#include "State/Machine/transition.hpp"

class TransitionFromInitTest : public testing::Test {
protected:
    // void SetUp() override {
    //     //transitions.clear();
    //     //TODO I wonder if it's trying to access uninitialized transitions 
    //     transitions= etl::vector<TransitionCase,6>( {
    //         TransitionCase( MachineStateId::IDLE, new StartMessage(), MachineStateId::RUNNING ),
    //         TransitionCase( MachineStateId::IDLE, new StartAtMessage(0.0), MachineStateId::IDLE ),
    //         TransitionCase( MachineStateId::IDLE, new StopMessage(), MachineStateId::IDLE ),
    //         TransitionCase( MachineStateId::IDLE, new StopAtMessage(0.0), MachineStateId::IDLE ),
    //         TransitionCase( MachineStateId::IDLE, new EStopMessage(), MachineStateId::ESTOP ),
    //         TransitionCase( MachineStateId::IDLE, new ResetMessage(), MachineStateId::IDLE )

    //     });
    // };
};

TEST_F(TransitionFromInitTest, one_transition_from_reset) {

    const etl::pair<MachineMessageInterface&, MachineStateId> transitions[] = 
    {
        {StartMessage(), MachineStateId::RUNNING},
        {StartAtMessage(100), MachineStateId::IDLE},
        {StopMessage(), MachineStateId::IDLE},
        {StopAtMessage(200), MachineStateId::IDLE},
        {EStopMessage(), MachineStateId::ESTOP},
        {ResetMessage(), MachineStateId::IDLE}
    };

    Machine fsm;
    RunningState runningState;
    IdleState idleState;
    EStopState eStopState;

    // The list of states.
    etl::ifsm_state* stateList[] = { &idleState, &runningState, &eStopState };

    // Set up the FSM
    fsm.set_states(stateList, 3);
    fsm.start();

    etl::fsm_state_id_t currentState = fsm.get_state_id();
    EXPECT_EQ(currentState, static_cast<int>(MachineStateId::IDLE)) << "State is not IDLE at start";

    for (int i = 0; i != sizeof(transitions); i++) {
        fsm.reset();
        fsm.start();
        fsm.process_queue();

        currentState = fsm.get_state_id();
        EXPECT_EQ(currentState, static_cast<int>(MachineStateId::IDLE)) << "State is not IDLE after reset for case " << i;

        etl::send_message(fsm, transitions[i].first);

        fsm.process_queue();

        currentState = fsm.get_state_id();
        EXPECT_EQ(currentState, static_cast<int>(transitions[i].second)) << "Case " << i << " failed";
    }

    // No need for explicit cleanup; etl::shared_ptrs in the vector will automatically clean up
}
