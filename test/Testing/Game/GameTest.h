#include <gtest/gtest.h>

#include "Testing/Extensions/ThrowingAssertions.h"

#include "GameWrapper.h"

class GameTest : public testing::Test {
 public:
    static void Init(GameWrapper *withWrapper);

    virtual void SetUp() override;
    virtual void TearDown() override;

 protected:
    GameWrapper *const game = nullptr; // To be used in tests.
};

#ifndef TEST_GROUP
#   error "Please define TEST_GROUP before including this header."
#endif

#define GAME_TEST(TestBase, TestName) \
    GTEST_TEST_(TEST_GROUP, TestBase##_##TestName, GameTest, testing::internal::GetTypeId<GameTest>())
