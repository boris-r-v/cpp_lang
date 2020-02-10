#include "code_gen.h"
#include <stdexcept>

std::string BadLanguage::someCodeRelatedThing()
{
    throw std::logic_error ("Bad language");
    return "";
}
std::string BadLanguage::generateCode()
{
    throw std::logic_error ("Bad language");
    return "";
}

std::string JavaLanguage::someCodeRelatedThing()
{
    return "JavaLanguage::someCodeRelatedThing";
}

std::string JavaLanguage::generateCode()
{
    return "JavaLanguage::generateCode";
}

std::string CppLanguage::someCodeRelatedThing()
{
    return "CppLanguage::someCodeRelatedThing()";
}

std::string CppLanguage::generateCode()
{
    return "CppLanguage::generateCode";
}

std::string PhpLanguage::someCodeRelatedThing()
{
    return "PhpLanguage::someCodeRelatedThing()";
}

std::string PhpLanguage::generateCode()
{
    return "PhpLanguage::generateCode";
}




