#pragma once

#include "pluginterfaces/base/funknown.h"
#include "pluginterfaces/vst/vsttypes.h"

namespace Steinberg {
namespace Vst {

//------------------------------------------------------------------------
enum ExamplePluginParams : ParamID
{
	kBypassId = 100,

	kParamVolId = 102,
	kParamOnId = 1000
};

} // namespace Vst

//------------------------------------------------------------------------
static const FUID kExamplePluginProcessorUID (0x32C50013, 0xFF5F5CB4, 0x871C312D, 0xB4F42368);
static const FUID kExamplePluginControllerUID (0xAE34DD83, 0x308259DF, 0xA0D88E2F, 0xB1C1CB8B);

#define ExamplePluginVST3Category "Fx"

//------------------------------------------------------------------------
} // namespace Steinberg 