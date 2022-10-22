/*
  ==============================================================================

    GPC_Osc.cpp
    Created: 22 Oct 2022 10:15:41am
    Author:  Ryan Devens

  ==============================================================================
*/

#include "GPC_Osc.h"
#include "WaveTable.h"

//===============================
GPC_Osc::GPC_Osc()
{
	waveTable = std::make_unique<WaveTable>();
}

//===============================
GPC_Osc::~GPC_Osc()
{
	waveTable.release();
}

//===============================
void GPC_Osc::prepareToPlay(double sampleRate)
{
	mSampleRate = sampleRate;
	jassert(mSampleRate > 0);
}

//==============================
void GPC_Osc::setFrequency(float freq)
{
	currentFreq = freq;
	currentPhaseIncrement = _calculatePhaseIncrement(currentFreq);
}

//==============================
float GPC_Osc::_calculatePhaseIncrement(float freq)
{
	int tableSize = waveTable->getTableSize();
	float tableSizeOverSampleRate =  tableSize / mSampleRate;
	float phaseIncrement = freq * tableSizeOverSampleRate;
	return phaseIncrement;
}

//==============================
float GPC_Osc::getNextSample()
{
	float index = _getNextSampleIndex();
	float sample = waveTable->getSampleAtIndex(index);
	return sample;
}

//==============================
float GPC_Osc::_getNextSampleIndex()
{
	float nextSampleIndex = currentIndex;
	
	currentIndex += currentPhaseIncrement;
	
	// wrap around table size
	if(currentIndex >= waveTable->getTableSize())
		currentIndex -= (float)waveTable->getLastIndexInWaveTable();
	
	return nextSampleIndex;

}
