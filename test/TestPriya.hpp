#include <cxxtest/TestSuite.h>
#include <cmath>
#include "AbstractCellBasedTestSuite.hpp"
#include <iostream>

using namespace std;
		
class TestPriya: public AbstractCellBasedTestSuite

{
public:
    void Test1()
    {      
	int main();
	{
            int i, n = 1000;
            double sum = 0;
            for (i = 1; i <= n; ++i) 
            {
                sum += 1.0 / (1.0 * i * i);
                cout << sum << endl;
            }
            TS_ASSERT_DELTA (sum, (M_PI * M_PI)/6, 1e-1)
        }
    }
};
