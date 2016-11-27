/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "ModulationConnector.h"
#include "AmpComponent.h"
#include "FilterComponent.h"
#include "OscillatorComponent.h"
#include "DelayComponent.h"
#include "ReverbComponent.h"

class ParameterSlider;

//==============================================================================
/**
*/
class NoisemakerAudioProcessorEditor  : public AudioProcessorEditor, public DragAndDropContainer, private Timer
{
public:
    NoisemakerAudioProcessorEditor (NoisemakerAudioProcessor&);
    ~NoisemakerAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
	void timerCallback() override;

	virtual void dragOperationStarted();
	virtual void dragOperationEnded();

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.

    NoisemakerAudioProcessor& processor;
	MidiKeyboardComponent keyboardComponent;

	Label timecodeDisplayLabel, gainLabel, delayLabel, filterLabel, envAttackLabel, envDecayLabel;
	ScopedPointer<ParameterSlider> gainSlider, delaySlider, filterSlider, envAttackSlider, envDecaySlider;
	ScopedPointer<ResizableCornerComponent> resizer;
	ScopedPointer<ComboBox> waveformBox;
	ScopedPointer<ModulationConnector> envelopeConnector;
    ScopedPointer<AmpComponent> ampComponent;
    ScopedPointer<FilterComponent> filterComponent;
    ScopedPointer<OscillatorComponent> oscillatorComponent;
    ScopedPointer<DelayComponent> delayComponent;
    ScopedPointer<ReverbComponent> reverbComponent;
	ComponentBoundsConstrainer resizeLimits;

	AudioPlayHead::CurrentPositionInfo lastDisplayedPosition;

	//==============================================================================
	NoisemakerAudioProcessor& getProcessor() const
	{
		return static_cast<NoisemakerAudioProcessor&> (processor);
	}

	void updateTimecodeDisplay(AudioPlayHead::CurrentPositionInfo);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NoisemakerAudioProcessorEditor)
};


#endif  // PLUGINEDITOR_H_INCLUDED
