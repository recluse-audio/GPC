/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "GPC_Synth.h"

//==============================================================================
GPCAudioProcessor::GPCAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, nullptr, "Parameters", _createParameters())
#endif
{
	synth = std::make_unique<GPC_Synth>();
	apvts.state.addListener(this);
}

GPCAudioProcessor::~GPCAudioProcessor()
{
	apvts.state.removeListener(this);
}

//==============================================================================
const juce::String GPCAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool GPCAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool GPCAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool GPCAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double GPCAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int GPCAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int GPCAudioProcessor::getCurrentProgram()
{
    return 0;
}

void GPCAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String GPCAudioProcessor::getProgramName (int index)
{
    return {};
}

void GPCAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void GPCAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	juce::dsp::ProcessSpec spec{ sampleRate, (juce::uint32)samplesPerBlock, 2 };
	synth->prepare(spec);
	updateAPVTS();
}

void GPCAudioProcessor::releaseResources()
{

}

#ifndef JucePlugin_PreferredChannelConfigurations
bool GPCAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else

    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

//==================================
void GPCAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

	if(mustUpdateAPVTS.get())
		updateAPVTS();
	
	synth->renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

}

//==============================================================================
bool GPCAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* GPCAudioProcessor::createEditor()
{
    return new GPCAudioProcessorEditor (*this);
}

//==============================================================================
void GPCAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void GPCAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GPCAudioProcessor();
}

//=====================================================
void GPCAudioProcessor::updateAPVTS()
{
	synth->updateAPVTS(apvts);
	mustUpdateAPVTS = false;
}

//=====================================================
juce::AudioProcessorValueTreeState& GPCAudioProcessor::getValueTree()
{
	return apvts;
}


//=====================================================
juce::AudioProcessorValueTreeState::ParameterLayout GPCAudioProcessor::_createParameters()
{
	std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
	
	// ADSR Params for envelope1.  ADR in seconds, Sustain in gain 0.f-1.
	params.push_back(std::make_unique<juce::AudioParameterFloat>("ATTACK 1", "Attack 1", juce::NormalisableRange<float>(0.0f, 3.0f, 0.01f, 0.5f), 2.5f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>("DECAY 1", "Decay 1", juce::NormalisableRange<float>(0.0f, 1.0f, 0.01, 0.5f), 0.2f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>("SUSTAIN 1", "Sustain 1", juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.8f));
	params.push_back(std::make_unique<juce::AudioParameterFloat>("RELEASE 1", "Release 1", juce::NormalisableRange<float>(0.0f, 3.0f, 0.01f, 0.5f), 2.5f));
	
	return { params.begin(), params.end() };
}


//=====================================
void GPCAudioProcessor::valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property)
{
	mustUpdateAPVTS = true;
}
