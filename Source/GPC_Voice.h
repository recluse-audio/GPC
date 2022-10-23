/*
  ==============================================================================

    GPC_Voice.h
    Created: 22 Oct 2022 10:14:18am
    Author:  Ryan Devens

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class GPC_Osc;

class GPC_Voice : public juce::MPESynthesiserVoice
{
public:
	GPC_Voice();
	~GPC_Voice();
	
	void prepare(const juce::dsp::ProcessSpec& spec);

	//***********************
	// PURE VIRTUAL OVERRIDES
	void noteStarted() override;
	void notePitchbendChanged() override;
	void noteStopped(bool allowTailOff) override;
	void notePressureChanged() override ;
	void noteTimbreChanged() override ;
	void noteKeyStateChanged() override ;
	void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;

private:
	std::unique_ptr<GPC_Osc> oscillator;
	std::unique_ptr<juce::ADSR> env;
};
