#include "Filter.h"

Filter::Filter() 
{
    name = "template";
}


Filter::Filter(std::string _name) 
{
    name = _name;
}


Filter::~Filter()
{

}


char* Filter::cstr_Name()
{
	return (char*)name.c_str();
}


std::string Filter::Name()
{
	return name;
}

