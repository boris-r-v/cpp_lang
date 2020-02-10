#include "code_gen.h"
#include <iostream>

class CodeGenerator
{
	GeneratorIface	*gen_;
    public:
	enum Lang {JAVA, C_PLUS_PLUS, PHP, C};
	CodeGenerator(Lang lang ):
		gen_ ( BadLanguage::instance() )
	{ 
	    switch( lang ) 
	    {
    		case JAVA:        	gen_ = JavaLanguage::instance(); break;
	        case C_PLUS_PLUS: 	gen_ = CppLanguage::instance();  break;
	        case PHP:         	gen_ = PhpLanguage::instance();  break;
    	    }
	}
	std::string generateCode()
	{
	    return gen_-> generateCode();
	}
	std::string someCodeRelatedThing()
	{
	    return gen_-> someCodeRelatedThing();
	}
};

int main()
{
    CodeGenerator g ( CodeGenerator::PHP );

    std::cout << g.someCodeRelatedThing() << std::endl;
    std::cout << g.generateCode() << std::endl;

    CodeGenerator g2 ( CodeGenerator::C );

    std::cout << g2.someCodeRelatedThing() << std::endl;
    std::cout << g2.generateCode() << std::endl;

    return 0;
    
}