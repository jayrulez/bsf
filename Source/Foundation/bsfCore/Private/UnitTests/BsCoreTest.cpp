//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsApplication.h"
#include "Scene/BsSceneObject.h"
#include "Components/BsCCamera.h"
#include "RenderAPI/BsGpuProgram.h"

using namespace bs;

int main()
{
	Application::startUp(VideoMode(1280, 720), "My app", false);

	String source = R"(
		struct PSInput
	{
		float4 Pos : SV_POSITION;
		float3 Color : COLOR;
	};
	void main(in  uint    VertId : SV_VertexID,
		out PSInput PSIn)
	{
		float4 Pos[3];
		Pos[0] = float4(-0.5, -0.5, 0.0, 1.0);
		Pos[1] = float4(0.0, +0.5, 0.0, 1.0);
		Pos[2] = float4(+0.5, -0.5, 0.0, 1.0);
		float3 Col[3];
		Col[0] = float3(1.0, 0.0, 0.0); // red
		Col[1] = float3(0.0, 1.0, 0.0); // green
		Col[2] = float3(0.0, 0.0, 1.0); // blue
		PSIn.Pos = Pos[VertId];
		PSIn.Color = Col[VertId];
	}
	)";

	GPU_PROGRAM_DESC desc;
	desc.type = GPT_VERTEX_PROGRAM;
	desc.source = source;
	desc.entryPoint = "main";
	desc.language = "hlsl";

	SPtr<GpuProgram> myProgram = GpuProgram::create(desc);
	myProgram->blockUntilCoreInitialized();

	return 0;
}