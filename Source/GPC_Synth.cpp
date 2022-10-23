/*
  ==============================================================================

    GPC_Synth.cpp
    Created: 22 Oct 2022 10:14:02am
    Author:  Ryan Devens

  ==============================================================================
*/

#include "GPC_Synth.h"
#include "GPC_Voice.h"


static constexpr size_t maxNumVoices = 5;

//======================
GPC_Synth::GPC_Synth()
{
	_initVoices(maxNumVoices);
}

//======================
GPC_Synth::~GPC_Synth()
{
	
}

//======================
void GPC_Synth::prepare(juce::dsp::ProcessSpec& spec)
{
	this->setCurrentPlaybackSampleRate(spec.sampleRate);
	
	for(auto* v : voices)
	{
		auto voice = dynamic_cast<GPC_Voice*>(v);
		voice->prepare(spec);
	}
}


//======================
void GPC_Synth::_initVoices(int numVoices)
{
	for(size_t i = 0; i <= maxNumVoices; ++i)
	{
		this->addVoice(new GPC_Voice());
	}
	
	setVoiceStealingEnabled(true);

}
