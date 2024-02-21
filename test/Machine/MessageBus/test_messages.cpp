#include "Logging/Logger.hpp"
#include "Mocks/Logging/MockLogBackend.hpp"
#include "Machine/MessageBus/Messages.hpp"

#include "TestHelpers/DefaultUnitTest.hpp"
#include "TestHelpers/Logging/LoggingUnitTest.hpp"
#include "Machine/MessageBus/Subscription.hpp"
#include "Machine/MessageBus/Broker.hpp"
#include "Machine/FSM/Machine.hpp"
#include "Machine/MessageBus/MachineRouter.hpp"
#include "Mocks/Machine/FSM/MockMachine.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace Machine::MessageBus;

class MessagesTest : public DefaultUnitTest {
    
};

TEST_F(MessagesTest, messages_with_params_store_corectly) {
    StartAtMessage startAtMessage(100);
    StopAtMessage stopAtMessage(200);

    ASSERT_EQ(startAtMessage.position, 100);
    ASSERT_EQ(stopAtMessage.position, 200);
}