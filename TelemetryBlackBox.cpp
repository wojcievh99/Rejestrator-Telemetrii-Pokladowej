#include "TelemetryBlackBox.h"
#include "DataFrame.h"

#include <iostream>

void TelemetryBlackBox::onRocketStateChange(RocketState state) {

	DataFrame::instance().setState(state);

}

void TelemetryBlackBox::onParachuteDeployed() {
	
	DataFrame::instance().onParachuteDeploy();

}

void TelemetryBlackBox::onBaroReading(bool error, uint16_t value) {
	
	if (error)
		DataFrame::instance().onBaroError();
	else
		DataFrame::instance().setAltitude(value);

}

void TelemetryBlackBox::recordCurrentState() {
	
	

}

uint32_t TelemetryBlackBox::getLastDataframe() {

}
