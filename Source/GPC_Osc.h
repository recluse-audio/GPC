/*
  ==============================================================================

    GPC_Osc.h
    Created: 22 Oct 2022 10:15:41am
    Author:  Ryan Devens

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class WaveTable;

class GPC_Osc
{
public:

	GPC_Osc();
	~GPC_Osc();
	
	void prepareToPlay(double sampleRate);
	
	// modulation should have been applied by the time you are calling this function
	void setFrequency(float freq);
	float getNextSample();
	
private:
	std::unique_ptr<WaveTable> waveTable;
	double mSampleRate = -1;
	float currentIndex = 0.f;
	float currentPhaseIncrement = 0.f;
	float currentFreq = 0.f;
	
	//
	float _calculatePhaseIncrement(float freq);
	
	// Calculate the next waveform index based on frequency passed in
	float _getNextSampleIndex();
};
