#include "FCMPSOcounter.h"
#include "FCMcounter.h"
#include "PSOcounter.h"

FCMPSOcounter::FCMPSOcounter(const Dataset * pdata)
{
	data = pdata;
}

FCMPSOcounter::~FCMPSOcounter()
{
}

void FCMPSOcounter::count(const Dataset * pdata)
{
	if (pdata) {
		data = pdata;
	}
	if (data) {
		PSOcounter pso;
		pso.count(data);
		const double** mu = pso.getBestX();
		FCMcounter fcm;
		//fcm.count(data, mu);
	}
}
