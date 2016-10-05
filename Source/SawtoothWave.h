/*
  ==============================================================================

    SawtoothWave.h
    Created: 13 May 2016 9:41:51pm
    Author:  Alex

  ==============================================================================
*/

#ifndef SAWTOOTHWAVE_H_INCLUDED
#define SAWTOOTHWAVE_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "SawtoothWavetable.h"

class SawtoothWaveSound : public SynthesiserSound
{
public:
	SawtoothWaveSound() {}

	bool appliesToNote(int /*midiNoteNumber*/) override { return true; }
	bool appliesToChannel(int /*midiChannel*/) override { return true; }
};

//==============================================================================
/** A simple demo synth voice that just plays a sine wave.. */
class SawtoothWaveVoice : public SynthesiserVoice
{
public:
	SawtoothWaveVoice()
		: angleDelta(0.0),
		tailOff(0.0),
        wavetable(SawtoothWavetable(40.0, 2048, getSampleRate()))
	{
	}

	bool canPlaySound(SynthesiserSound* sound) override
	{
		return dynamic_cast<SawtoothWaveSound*> (sound) != nullptr;
	}

	void startNote(int midiNoteNumber, float velocity,
		SynthesiserSound* /*sound*/,
		int /*currentPitchWheelPosition*/) override
	{
		currentAngle = 0.0;
		level = velocity * 0.15;
		tailOff = 0.0;

		frequency = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
		double frqRad = (2.0 * double_Pi) / getSampleRate();
		period = 1 / frequency;

		angleDelta = frqRad * frequency;
	}

	void stopNote(float /*velocity*/, bool allowTailOff) override
	{
		if (allowTailOff)
		{
			// start a tail-off by setting this flag. The render callback will pick up on
			// this and do a fade out, calling clearCurrentNote() when it's finished.

			if (tailOff == 0.0) // we only need to begin a tail-off if it's not already doing so - the
								// stopNote method could be called more than once.
				tailOff = 1.0;
		}
		else
		{
			// we're being told to stop playing immediately, so reset everything..

			clearCurrentNote();
			angleDelta = 0.0;
		}
	}

	void pitchWheelMoved(int /*newValue*/) override
	{
		// can't be bothered implementing this for the demo!
	}

	void controllerMoved(int /*controllerNumber*/, int /*newValue*/) override
	{
		// not interested in controllers in this case.
	}

	void renderNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override
	{
		processBlock(outputBuffer, startSample, numSamples);
	}

	void renderNextBlock(AudioBuffer<double>& outputBuffer, int startSample, int numSamples) override
	{
		processBlock(outputBuffer, startSample, numSamples);
	}

private:

	template <typename FloatType>
	void processBlock(AudioBuffer<FloatType>& outputBuffer, int startSample, int numSamples)
	{
		if (angleDelta != 0.0)
		{
            float* subtable = wavetable.getSubtableForFrequency(frequency);
            int tableSize = wavetable.getTableSize();
            double twoPi = 2.0 * double_Pi;
			if (tailOff > 0)
			{
				while (--numSamples >= 0)
				{
                    int index = (int)((currentAngle / twoPi) * tableSize);
                    FloatType currentSample = level * subtable[index];
                    //DBG(subtable[index]);

					for (int i = outputBuffer.getNumChannels(); --i >= 0;)
						outputBuffer.addSample(i, startSample, currentSample);

					currentAngle += angleDelta;
					if (currentAngle >= twoPi)
					{
						currentAngle -= twoPi;
					}
					++startSample;

					tailOff *= 0.99;

					if (tailOff <= 0.005)
					{
						clearCurrentNote();

						angleDelta = 0.0;
						break;
					}
				}
			}
			else
			{
				while (--numSamples >= 0)
				{
                    int index = (int)((currentAngle / twoPi) * tableSize);
					FloatType currentSample = level * subtable[index];
                    //DBG(subtable[index]);

					for (int i = outputBuffer.getNumChannels(); --i >= 0;)
						outputBuffer.addSample(i, startSample, currentSample);

					currentAngle += angleDelta;
					if (currentAngle >= twoPi)
					{
						currentAngle -= twoPi;
					}
					++startSample;
				}
			}
		}
	}

	double currentAngle, angleDelta, level, tailOff, frequency, period;
    SawtoothWavetable wavetable;
};



#endif  // SAWTOOTHWAVE_H_INCLUDED
