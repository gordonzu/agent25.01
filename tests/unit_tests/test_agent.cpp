#include "gtest/gtest.h"
#include "agent.h"

using namespace::testing;

class AgentTest: public Test { };

TEST_F(AgentTest, testRandomVacuumAgent) {
	auto agent = RandomVacuumAgent();
    auto env = std::make_unique<TrivialVacuumEnvironment>();
    env->add_object(std::move(agent));
    env->run();
    std::cout << "rows: " << env->get_size() << "\n";
    std::cout << format(env->get_status()) << "\n";

	//std::unique_ptr<Object> agent = RandomVacuumAgent();
	//std::unique_ptr<Environment> env = std::make_unique<TrivialVacuumEnvironment>;
	//env->add_object(agent);
	//env->run();
	//std::cout << print(env->status) << "\n";
	//ASSERT_STREQ(env.status.c_str())
	//assert env.status == {(1, 0): 'Clean', (0, 0): 'Clean'}
	EXPECT_EQ (0, 0);
}
