////////////
//
// This has re-factored by Bismarck, original code was written by Sunnlok
// 
////////////
#pragma once

#include <CryRenderer/Pipeline/IPipeline.h>

struct ViewRenderData
{
	Cry::Renderer::Pipeline::TStageBasePtr	stage;

	uint32					viewPass;

	_smart_ptr<ITexture>	viewColorTarget;
	_smart_ptr<ITexture>	viewDepthTarget;

	std::atomic<uint64>		m_newRes;

	uintptr_t activeIndexBuffer = Cry::Renderer::Buffers::CINVALID_BUFFER;
	uintptr_t activeVertexBuffer = Cry::Renderer::Buffers::CINVALID_BUFFER;

	uint32_t vertexCBHash;
	uint32_t pixelCBHash;
	uint32_t effectCBHash;
	uint32_t texDimensionsCBHash;

	uint32 viewWidth;
	uint32 viewHeight;

	bool bDrawToScreen;

	Noesis::IRenderer* pNsRenderer;
};