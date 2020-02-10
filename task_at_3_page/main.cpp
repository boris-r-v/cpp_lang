#include <string>
#include <map>
#include <memory>

int values[3];

namespace FF
{
    #define LEGACY_SIZE 3
    extern int *values;
    class MyBlah
    {
	public:
	    MyBlah( int iu ) {}
    };

    class Adapter
    {
	public:
	    Adapter()
	    {
		for (int i = 0; i < LEGACY_SIZE; ++i)
		    map_.emplace( std::make_pair( values[i], std::make_shared<MyBlah>(values[i]) ) );
	    }
	private	:
	    std::map<int, std::shared_ptr<MyBlah>> map_;
    };
}

int main()
{
    return 0;
}