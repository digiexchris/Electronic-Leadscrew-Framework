#include "Logging/Logger.hpp"
#include <gtest/gtest.h>

#include <gmock/gmock.h> // Include the necessary header file
#include "Mocks/MockLogBackend.hpp"

class LoggerTest : public testing::Test {
protected:
};

TEST_F(LoggerTest, use_without_create_throws)
{

  ASSERT_THROW({
    // Log<256>::Init(nullptr);
    LogSingleton::instance().Info("Foo");
  },etl::singleton_not_created);
}

TEST_F(LoggerTest, init_with_nullptr_backend_throws)
{
  ASSERT_THROW({
    LogFactory<256>::Create(nullptr);
  }, LoggerInitException);
}

TEST_F(LoggerTest, init_with_valid_backend_does_not_throw_and_can_log)
{
  MockLogBackend<ELSF_LOG_MAX_MESSAGE_LENGTH> mockLogBackend;
  // ON_CALL(mockLogBackend, Info(testing::_, testing::_)).WillByDefault([](){
  //   ASSERT_HRESULT_FAILED("Info called with unexpected arguments");
  // });
  LogFactory<256>::Create(&mockLogBackend);
  LogSingleton::instance().Info("Foo");
  // EXPECT_CALL(mockLogBackend, Info("Foo", testing::_)).Times(1);
  // EXPECT_CALL(mockLogBackend, Info(testing::_, testing::_)).Times(0);
}

#include <random>
TEST_F(LoggerTest, init_while_already_initialized_resets)
{
  uint32_t uniqueId1 = 0;
  uint32_t uniqueId2 = 0;


  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<uint32_t> dis(1, UINT32_MAX); // Adjust the range as needed
  uniqueId1 = dis(gen);
  uniqueId2 = dis(gen);
  int tries = 10;
  while(uniqueId1 == uniqueId2) { //an attempt to not make this test flakey on the off chance that the random number generator generates the same number twice
    ASSERT_GT(tries, 0); //an attempt to not let this loop run forever
    uniqueId2 = dis(gen);
    tries--;
  }

  MockLogBackend<256ULL>* mockLogBackend1 = new MockLogBackend<256ULL>(uniqueId1);
  MockLogBackend<256ULL> mockLogBackend2(uniqueId2);

  LogFactory<256>::Create(mockLogBackend1);
  LogFactory<256>::Create(&mockLogBackend2);
  auto backend = LogSingleton::instance().GetBackend();
  EXPECT_EQ(static_cast<MockLogBackend<256>*>(backend)->uniqueId, uniqueId2);
}

TEST_F(LoggerTest, macro_use_without_init_throws)
{

  ASSERT_THROW({
    // Log<256>::Init(nullptr);
    ELSF_LOG_INFO("Foo");

    // rework this logger so that Log::Info() calls LogSingleton::instance().Info() if LogSingleton::has_instance() otherwise throw an
  },
               LoggerInitException);
}