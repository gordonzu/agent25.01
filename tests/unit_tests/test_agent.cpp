#include "gtest/gtest.h"
#include "agent.h"

using namespace::testing;

class AgentTest: public Test { };

TEST_F(AgentTest, testRandomVacuumAgent) {
    auto agent = RandomVacuumAgent();
    auto env = std::make_unique<TrivialVacuumEnvironment>();
    env->add_object(std::move(agent));
    env->run();

    std::string map_value = format(env->get_status());
    ASSERT_STREQ(map_value.c_str(), "{(1, 0): 'Clean', (0, 0): 'Clean'}");
}
