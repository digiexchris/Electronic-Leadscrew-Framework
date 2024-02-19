#pragma once

#include "State/MessageBus/Messages.hpp"
#include "State/Machine/Machine.hpp"
#include "State/MessageBus/Router.hpp"

#include <etl/hfsm.h>

namespace State
{
namespace MessageBus
{


//NOTE TO CHRIS. THIS IS THE ROUTER THAT THE BROKER CALLS, STOP TRYING TO MAKE IT THE BROKER
class MachineRouter 
    : public State::MessageBus::QueuedRouter<
    MachineRouter, 
    10, //Queue Depth
    StartMessage, 
    StartAtMessage, 
    StopMessage, 
    StopAtMessage, 
    EStopMessage, 
    ResetMessage>
{
public:
    explicit MachineRouter(State::Machine::Machine& fsm);

    virtual void on_receive(const etl::imessage& msg);
    virtual void on_receive_unknown(const etl::imessage& msg);

private:
    etl::hfsm& myFsm;
};

} // namespace Machine
} // namespace State