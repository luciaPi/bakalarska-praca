#pragma once
#include "Dataset.h"

class FCMPSOcounter
{
private:
	const Dataset* data;

public:
	FCMPSOcounter() : data(nullptr) {}
	FCMPSOcounter(const Dataset* pdata);
	~FCMPSOcounter();

	void count(const Dataset* pdata = nullptr);
};

