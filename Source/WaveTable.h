/*
  ==============================================================================

    WaveTable.h
    Created: 22 Oct 2022 10:15:07am
    Author:  Ryan Devens

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

/**
	Class representing a single waveform
	Waveform is stored in a juce::AudioBuffer of floats
	Waveforms (buffers) will always be set to a size that is a power of two
	This class returns samples at a given index
	Handles float interpolation between integer indices
*/
class WaveTable
{
public:
	WaveTable();
	~WaveTable();
	
	
	void prepareToPlay(double sampleRate);
	
	float getSampleAtIndex(float index);
	
	int getTableSize();
	
	// Use THIS function to get tableSize - 1
	int getLastIndexInWaveTable();
	
private:
	juce::AudioBuffer<float> waveBuffer;
	
	void _generateWave();

};
