#pragma once
#include <gtest/gtest.h>
#include "Logging/Logger.hpp"
#include "TestHelpers/Logging/TestLogBackend.hpp"

class DefaultUnitTest : public ::testing::Test {
protected:
    void SetUp() override {
        if(LogSingleton::is_valid())
        {
            LogSingleton::destroy();
        }

        ELSF_LOG_INIT(new TestLogBackend<ELSF_LOG_MAX_MESSAGE_LENGTH, 8>());
    }
};