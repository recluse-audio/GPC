/*
  ==============================================================================

    GPC_Voice.cpp
    Created: 22 Oct 2022 10:14:18am
    Author:  Ryan Devens

  ==============================================================================
*/

#include "GPC_Voice.h"
#include "GPC_Osc.h"

//======================
GPC_Voice::GPC_Voice()
{
	oscillator = std::make_unique<GPC_Osc>();
	env = std::make_unique<juce::ADSR>();
	
}

//======================
GPC_Voice::~GPC_Voice()
{
	
}

//======================
void GPC_Voice::prepare(const juce::dsp::ProcessSpec& spec)
{
	oscillator->prepareToPlay(spec.sampleRate);
	env->setSampleRate(spec.sampleRate);
	//oscillator->setFrequency(600.f);
}

//======================
void GPC_Voice::noteStarted()
{
	auto velocity = getCurrentlyPlayingNote().noteOnVelocity.asUnsignedFloat();
	auto freqHz = (float)getCurrentlyPlayingNote().getFrequencyInHertz();
	
	oscillator->setFrequency(freqHz);
	
	env->noteOn();
}

//======================
void GPC_Voice::notePitchbendChanged()
{
	
}

//======================
void GPC_Voice::noteStopped(bool allowTailOff)
{
	env->noteOff();
}

//======================
void GPC_Voice::notePressureChanged()
{
	
}

//======================
void GPC_Voice::noteTimbreChanged()
{
	
}

//======================
void GPC_Voice::noteKeyStateChanged()
{
	
}

//======================
void GPC_Voice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
	if( ! env->isActive() )
		return;
	
	auto buffWrite =  outputBuffer.getArrayOfWritePointers();
	int endSample = startSample + numSamples;
	
	for(int sampleIndex = startSample; sampleIndex < endSample; ++sampleIndex)
	{
		float oscSample = oscillator->getNextSample();
		oscSample *= 0.5f;
		oscSample *= env->getNextSample();
		
		for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
		{
			buffWrite[channel][sampleIndex] += oscSample;
		}
	}
}

//=========================
void GPC_Voice::updateAPVTS(juce::AudioProcessorValueTreeState &apvts)
{
	// Assign Env params (in voice class)
	auto atk1 = apvts.getRawParameterValue("ATTACK 1")->load();
	auto dec1 = apvts.getRawParameterValue("DECAY 1")->load();
	auto sus1 = apvts.getRawParameterValue("SUSTAIN 1")->load();
	auto rel1 = apvts.getRawParameterValue("RELEASE 1")->load();
	
	juce::ADSR::Parameters newParameters { atk1, dec1, sus1, rel1 };
	
	env->setParameters(newParameters);
}


