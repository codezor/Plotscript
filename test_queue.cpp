#include "catch.hpp"
#include "interpreter.hpp"
#include "message_queue.hpp"

#include <condition_variable>
#include <mutex>
#include <queue>
// Write your tests first

TEST_CASE("Test meassage", "[message_queue]")
{	
	message_queue<int> &que = message_queue<int>::get_instance();
	
	int i = 0;
  while (true) {
	  que.push(i);
    if (que.size() == 3) {
		
      break;  
	}    
  }

  while(!que.empty())
  {
	  que.try_pop(i);
  }

}

