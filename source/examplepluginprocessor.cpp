#include "examplepluginprocessor.h"
#include "exampleplugincids.h"

#include "base/source/fstreamer.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"

using namespace Steinberg;

namespace Steinberg {

//------------------------------------------------------------------------
// ExamplePluginProcessor
//------------------------------------------------------------------------
ExamplePluginProcessor::ExamplePluginProcessor ()
{
	//--- set the wanted controller for our processor
	setControllerClass (kExamplePluginControllerUID);
}

//------------------------------------------------------------------------
ExamplePluginProcessor::~ExamplePluginProcessor ()
{}

//------------------------------------------------------------------------
tresult PLUGIN_API ExamplePluginProcessor::initialize (FUnknown* context)
{
	// Here the Plug-in will be instantiated
	
	//---always initialize the parent-------
	tresult result = AudioEffect::initialize (context);
	// if everything Ok, continue
	if (result != kResultOk)
	{
		return result;
	}

	//--- create Audio IO ------
	addAudioInput (STR16 ("Stereo In"), Steinberg::Vst::SpeakerArr::kStereo);
	addAudioOutput (STR16 ("Stereo Out"), Steinberg::Vst::SpeakerArr::kStereo);

	/* If you don't need an event bus, you can remove the next line */
	addEventInput (STR16 ("Event In"), 1);

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API ExamplePluginProcessor::terminate ()
{
	// Here the Plug-in will be de-instantiated, last possibility to remove some memory!
	
	//---do not forget to call parent ------
	return AudioEffect::terminate ();
}

//------------------------------------------------------------------------
tresult PLUGIN_API ExamplePluginProcessor::setActive (TBool state)
{
	//--- called when the Plug-in is enable/disable (On/Off) -----
	return AudioEffect::setActive (state);
}

//------------------------------------------------------------------------
tresult PLUGIN_API ExamplePluginProcessor::process (Vst::ProcessData& data)
{
    //--- Read inputs parameter changes-----------
    if (data.inputParameterChanges)
    {
        int32 numParamsChanged = data.inputParameterChanges->getParameterCount ();
        for (int32 index = 0; index < numParamsChanged; index++)
        {
            Vst::IParamValueQueue* paramQueue =
                data.inputParameterChanges->getParameterData (index);
            if (paramQueue)
            {
                Vst::ParamValue value;
                int32 sampleOffset;
                int32 numPoints = paramQueue->getPointCount ();
                switch (paramQueue->getParameterId ())
                {
                    case Vst::ExamplePluginParams::kParamVolId:
                        if (paramQueue->getPoint (numPoints - 1, sampleOffset, value) ==
                            kResultTrue)
                            mParam1 = value;
                        break;
                    case Vst::ExamplePluginParams::kParamOnId:
                        if (paramQueue->getPoint (numPoints - 1, sampleOffset, value) ==
                            kResultTrue)
                            mParam2 = value > 0 ? 1 : 0;
                        break;
                    case Vst::ExamplePluginParams::kBypassId:
                        if (paramQueue->getPoint (numPoints - 1, sampleOffset, value) ==
                            kResultTrue)
                            mBypass = (value > 0.5f);
                        break;
                }
            }
        }
    }

    //--- Process Audio---------------------
    if (data.numInputs == 0 || data.numOutputs == 0)
    {
        // nothing to do
        return kResultOk;
    }

    if (data.numSamples > 0)
    {
        // Process Algorithm: Simple Volume Control
        for (int32 channel = 0; channel < data.inputs[0].numChannels; channel++)
        {
            float* in = data.inputs[0].channelBuffers32[channel];
            float* out = data.outputs[0].channelBuffers32[channel];
            for (int32 sample = 0; sample < data.numSamples; sample++)
            {
                // Apply volume control only if mParam2 (On/Off) is active
                if (mParam2 > 0)
                {
                    out[sample] = in[sample] * mParam1; // Apply volume control
                }
                else
                {
                    out[sample] = in[sample]; // Bypass processing
                }
            }
        }
    }
    return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API ExamplePluginProcessor::setupProcessing (Vst::ProcessSetup& newSetup)
{
	//--- called before any processing ----
	return AudioEffect::setupProcessing (newSetup);
}

//------------------------------------------------------------------------
tresult PLUGIN_API ExamplePluginProcessor::canProcessSampleSize (int32 symbolicSampleSize)
{
	// by default kSample32 is supported
	if (symbolicSampleSize == Vst::kSample32)
		return kResultTrue;

	// disable the following comment if your processing support kSample64
	/* if (symbolicSampleSize == Vst::kSample64)
		return kResultTrue; */

	return kResultFalse;
}

//------------------------------------------------------------------------
tresult PLUGIN_API ExamplePluginProcessor::setState (IBStream* state)
{
	if (!state)
		return kResultFalse;

	// called when we load a preset or project, the model has to be reloaded

	IBStreamer streamer (state, kLittleEndian);

	float savedParam1 = 0.f;
	if (streamer.readFloat (savedParam1) == false)
		return kResultFalse;

	int32 savedParam2 = 0;
	if (streamer.readInt32 (savedParam2) == false)
		return kResultFalse;

	int32 savedBypass = 0;
	if (streamer.readInt32 (savedBypass) == false)
		return kResultFalse;

	mParam1 = savedParam1;
	mParam2 = savedParam2 > 0 ? 1 : 0;
	mBypass = savedBypass > 0;

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API ExamplePluginProcessor::getState (IBStream* state)
{
	// here we need to save the model (preset or project)

	float toSaveParam1 = mParam1;
	int32 toSaveParam2 = mParam2;
	int32 toSaveBypass = mBypass ? 1 : 0;

	IBStreamer streamer (state, kLittleEndian);
	streamer.writeFloat (toSaveParam1);
	streamer.writeInt32 (toSaveParam2);
	streamer.writeInt32 (toSaveBypass);

	return kResultOk;
}

//------------------------------------------------------------------------
} // namespace Steinberg 