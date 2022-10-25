/*
  ==============================================================================

    GPC_Synth.h
    Created: 22 Oct 2022 10:14:02am
    Author:  Ryan Devens

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class GPC_Synth : public juce::MPESynthesiser
{
public:
	GPC_Synth();
	~GPC_Synth();
	
	void prepare(juce::dsp::ProcessSpec& spec);
	void updateAPVTS(juce::AudioProcessorValueTreeState& apvts);

private:
	void _initVoices(int numVoices);

};
