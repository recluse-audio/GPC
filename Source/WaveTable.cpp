/*
  ==============================================================================

    WaveTable.cpp
    Created: 22 Oct 2022 10:15:07am
    Author:  Ryan Devens

  ==============================================================================
*/

#include "WaveTable.h"
#include "GPC_Constants.h"

//==============================
WaveTable::WaveTable()
{
	_generateWave();
}

//==============================
WaveTable::~WaveTable()
{
	
}

//==============================
void WaveTable::_generateWave()
{
	int tableSize = GPC_CONSTANTS::TABLE_SIZE;
	waveBuffer.setSize(1, tableSize);
	waveBuffer.clear();
	
	auto* buffWrite = waveBuffer.getWritePointer(0);
	
	double TWO_PI = juce::MathConstants<double>::twoPi;
	//double PI = juce::MathConstants<double>::pi;
	auto angleDelta = TWO_PI / (double)(tableSize - 1);
	double currentAngle = 0.0;
	
	for (unsigned int i = 0; i < tableSize; ++i)
	{
		float sample = 0.f;
		sample = std::sin(currentAngle);
		
		buffWrite[i] = sample;
		currentAngle += angleDelta;
		
	}
}


//==============================
float WaveTable::getSampleAtIndex(float index)
{
	// needs to handle wrapping before getting here
	jassert(index < GPC_CONSTANTS::TABLE_SIZE);
	
	int lowIndex  = (int)index;
	int highIndex = lowIndex + 1;
	
	if(highIndex >= GPC_CONSTANTS::TABLE_SIZE)
		highIndex = 0;
	
	float spillOver         = index - lowIndex; // how far into next sample this index is
	float inverseSpillOver  = 1.f   - spillOver;  //
	
	float sample1 = waveBuffer.getSample(0, lowIndex) * spillOver;
	float sample2 = waveBuffer.getSample(0, highIndex) * inverseSpillOver;
	
	float sample = (sample1 + sample2) / 2.f;
	
	return sample;
}


//=================================
int WaveTable::getTableSize()
{
	int tableSize = waveBuffer.getNumSamples();
	return tableSize;
}

//=================================
int WaveTable::getLastIndexInWaveTable()
{
	return waveBuffer.getNumSamples() - 1;
}
