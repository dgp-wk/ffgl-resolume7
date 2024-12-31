#pragma once
#include <string>
#include <FFGLSDK.h>


class Effect : public ffglqs::Effect
{
public:
	Effect();
	~Effect();

	FFResult SetFloatParameter( unsigned int dwIndex, float value ) override;
};

