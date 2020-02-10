#ifndef CODE_GEN_H
#define CODE_GEN_H

#include "singleton.h"
#include <string>

class GeneratorIface
{
    public:
	virtual std::string someCodeRelatedThing() = 0;
	virtual	std::string generateCode() = 0;

};

class BadLanguage: public GeneratorIface, public SingletonBase <BadLanguage> 
{
    public:
	std::string someCodeRelatedThing();
	std::string generateCode();
};

class JavaLanguage: public GeneratorIface, public SingletonBase <JavaLanguage> 
{
    public:
	std::string someCodeRelatedThing();
	std::string generateCode();
};

class CppLanguage: public GeneratorIface, public SingletonBase <CppLanguage> 
{
    public:
	std::string someCodeRelatedThing();
	std::string generateCode();
};

class PhpLanguage: public GeneratorIface, public SingletonBase <PhpLanguage> 
{
    public:
	std::string someCodeRelatedThing();
	std::string generateCode();
};

#endif // CODE_GEN_H