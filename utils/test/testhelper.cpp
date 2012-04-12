#include <boost/test/unit_test.hpp>

#include <vector>

BOOST_AUTO_TEST_CASE(test_helper)
{
	std::vector<bool> arr(16);

	arr[4] = true;
}
