#include "gtest/gtest.h"
#include "agent.h"

using namespace::testing;

class AgentTest: public Test { };

TEST_F(AgentTest, testRandomVacuumAgent) {
     auto agent = RandomVacuumAgent();
     auto env = std::make_unique<TrivialVacuumEnvironment>();
     env->add_object(std::move(agent));
     env->run();

    ASSERT_STREQ((format(env->get_status())).c_str(), "{(0, 0): 'Clean', (1, 0): 'Clean'}");
}
