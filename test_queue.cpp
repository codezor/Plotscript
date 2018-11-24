#include "catch.hpp"

#include "message_queue.hpp"

#include <thread>
// Write your tests first
void worker(message_queue<int> &que)
{

	for(int i = 0; i < 100; i++)
	{
		que.push(i);
	}
};

TEST_CASE("Test example", "[ThreadSafeQueue]")
{
	
	message_queue<int> &que = message_queue<int>::get_instance();
	//std::string test_program = "(+ 1 2)";
  std::thread th1(worker, std::ref(que));

  std::thread th2(worker, std::ref(que));

  while (true) {

    if (que.size() == 200) {
      break;
    }

    int result;
   
    while (que.try_pop( result)) {
      //std::cout << result << " ";
    }
    //std::cout << "\n";
  }

  th1.join();
  th2.join();
}
// TEST_CASE( "", "[ThreadSafeVector]" ) {

//}
