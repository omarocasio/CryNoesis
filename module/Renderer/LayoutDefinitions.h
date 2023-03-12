////////////
//
// This has re-factored by Bismarck, original code was written by Sunnlok
// 
////////////
#pragma once

#include <CryRenderer/Pipeline/IPipeline.h>
#include <CryRenderer/Pipeline/RenderPass.h>

#include <variant>
#include <array>
#include <any>

namespace LayoutDefinitions
{
	using namespace Cry::Renderer::Pipeline;
	using namespace Cry::Renderer::Pipeline::Pass;
	using namespace Cry::Renderer::Shader;

	enum class In : uint64
	{
		Pos = 0, // linear Position(xy)
		Color, // nointerpolation sRBG Color (rgba)
		Tex0, // linear TexCoord0 (uv)
		Tex1, // linear TexCoord1 (uv)
		Coverage, // linear Coverage(alpha)
		Rect, // nointerpolation Rect(x0, y0, x1, y1)
		Tile, // nointerpolation Rect(x, y , width , height )
		ImagePos, // linear Position (xy) - Scale(zw)

		Count
	};

	constexpr SInputElementDescription gElementDescriptions[] =
	{
		{ "POSITION", 0, EInputElementFormat::FORMAT_R32G32_FLOAT, 0, APPEND_ALIGNED_ELEMENT, EInputSlotClassification::PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, EInputElementFormat::R8G8B8A8_UNORM, 0, APPEND_ALIGNED_ELEMENT, EInputSlotClassification::PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, EInputElementFormat::FORMAT_R32G32_FLOAT, 0, APPEND_ALIGNED_ELEMENT, EInputSlotClassification::PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 1, EInputElementFormat::FORMAT_R32G32_FLOAT, 0, APPEND_ALIGNED_ELEMENT, EInputSlotClassification::PER_VERTEX_DATA, 0 },
		{ "COVERAGE", 0, EInputElementFormat::R16G16B16A16_UNORM, 0, APPEND_ALIGNED_ELEMENT, EInputSlotClassification::PER_VERTEX_DATA, 0 },
		{ "RECT", 0, EInputElementFormat::FORMAT_R32G32B32A32_FLOAT, 0, APPEND_ALIGNED_ELEMENT, EInputSlotClassification::PER_VERTEX_DATA, 0 },
		{ "TILE", 0, EInputElementFormat::FORMAT_R32G32B32A32_FLOAT, 0, APPEND_ALIGNED_ELEMENT, EInputSlotClassification::PER_VERTEX_DATA, 0 },
		{ "IMAGE_POSITION", 0, EInputElementFormat::FORMAT_R32G32_FLOAT, 0, APPEND_ALIGNED_ELEMENT, EInputSlotClassification::PER_VERTEX_DATA, 0 }
	};

	template<size_t size>
	using TDescrArray = std::array<SInputElementDescription, size>;

	using TDescVar = std::variant<TDescrArray<1>, TDescrArray<2>, TDescrArray<3>, TDescrArray<4>>;

	template<size_t size>
	struct TDesc
	{
		TDescrArray<size> descArray;

		template<typename ...Args>
		constexpr TDesc(Args... elements)
		{
			size_t i = 0;
			(void(descArray[i++] = gElementDescriptions[(uint64)elements]), ...);
		}
	};

	template<In ...elements>
	constexpr auto GetElementDescription()
	{
		constexpr TDesc< sizeof...(elements)> desc(elements...);
		return desc.descArray;
	}

	enum class Extra : uint64
	{
		ST1 = (uint64)In::Count,
		Count
	};

	enum class Effect : uint64
	{
		Rgba = (uint64)Extra::Count,
		Mask,
		Clear,
		Path,
		Path_AA,
		SDF,
		SDF_LCD,
		Opacity,
		Upsample,
		Downsameple,
		Shadow,
		Blur,

		Count
	};

	enum class Paint : uint64
	{
		Solid = (uint64)Effect::Count,
		Linear,
		Radial,
		Pattern,
		Pattern_Clamp,
		Pattern_Repeat,
		Pattern_MirrorU,
		Pattern_MirrorV,
		Pattern_Mirror,

		Count
	};

	const char* gMaskNames[] = {
		"Pos",
		"Color",
		"Tex0",
		"Tex1",
		"Coverage",
		"ST1",
		"Rgba",
		"Mask",
		"Path",
		"Path_AA",
		"SDF",
		"SDF_LCD",
		"Opacity",
		"Upsample",
		"Downsample",
		"Shadow",
		"Blur"
	};

	template<class TBit>
	constexpr uint64 ToMask(TBit bit)
	{
		return 1ull << static_cast<uint64>(bit);
	}

	constexpr std::array<uint64, Noesis::Shader::Count> gShaderMasks = {
		// Path
		ToMask(Effect::Rgba),ToMask(Effect::Mask),
		ToMask(Effect::Path) | ToMask(Paint::Solid),
		ToMask(Effect::Path) | ToMask(Paint::Linear),
		ToMask(Effect::Path) | ToMask(Paint::Radial),
		ToMask(Effect::Path) | ToMask(Paint::Pattern),
		ToMask(Effect::Path) | ToMask(Paint::Pattern_Clamp),
		ToMask(Effect::Path) | ToMask(Paint::Pattern_Repeat),
		ToMask(Effect::Path) | ToMask(Paint::Pattern_Mirror),
		ToMask(Effect::Path) | ToMask(Paint::Pattern_MirrorU),
		ToMask(Effect::Path) | ToMask(Paint::Pattern_MirrorV),
		//PathAA
		ToMask(Effect::Path_AA) | ToMask(Paint::Solid),
		ToMask(Effect::Path_AA) | ToMask(Paint::Linear),
		ToMask(Effect::Path_AA) | ToMask(Paint::Radial),
		ToMask(Effect::Path_AA) | ToMask(Paint::Pattern),
		ToMask(Effect::Path_AA) | ToMask(Paint::Pattern_Clamp),
		ToMask(Effect::Path_AA) | ToMask(Paint::Pattern_Repeat),
		ToMask(Effect::Path_AA) | ToMask(Paint::Pattern_Mirror),
		ToMask(Effect::Path_AA) | ToMask(Paint::Pattern_MirrorU),
		ToMask(Effect::Path_AA) | ToMask(Paint::Pattern_MirrorV),

		//SDF
		ToMask(Effect::SDF) | ToMask(Paint::Solid) | ToMask(Extra::ST1),
		ToMask(Effect::SDF) | ToMask(Paint::Linear) | ToMask(Extra::ST1),
		ToMask(Effect::SDF) | ToMask(Paint::Radial) | ToMask(Extra::ST1),
		ToMask(Effect::SDF) | ToMask(Paint::Pattern) | ToMask(Extra::ST1),
		ToMask(Effect::SDF) | ToMask(Paint::Pattern_Clamp) | ToMask(Extra::ST1),
		ToMask(Effect::SDF) | ToMask(Paint::Pattern_Repeat) | ToMask(Extra::ST1),
		ToMask(Effect::SDF) | ToMask(Paint::Pattern_Mirror) | ToMask(Extra::ST1),
		ToMask(Effect::SDF) | ToMask(Paint::Pattern_MirrorU) | ToMask(Extra::ST1),
		ToMask(Effect::SDF) | ToMask(Paint::Pattern_MirrorV) | ToMask(Extra::ST1),

		//SDF_LCD
		ToMask(Effect::SDF_LCD) | ToMask(Paint::Solid) | ToMask(Extra::ST1),
		ToMask(Effect::SDF_LCD) | ToMask(Paint::Linear) | ToMask(Extra::ST1),
		ToMask(Effect::SDF_LCD) | ToMask(Paint::Radial) | ToMask(Extra::ST1),
		ToMask(Effect::SDF_LCD) | ToMask(Paint::Pattern) | ToMask(Extra::ST1),
		ToMask(Effect::SDF_LCD) | ToMask(Paint::Pattern_Clamp) | ToMask(Extra::ST1),
		ToMask(Effect::SDF_LCD) | ToMask(Paint::Pattern_Repeat) | ToMask(Extra::ST1),
		ToMask(Effect::SDF_LCD) | ToMask(Paint::Pattern_Mirror) | ToMask(Extra::ST1),
		ToMask(Effect::SDF_LCD) | ToMask(Paint::Pattern_MirrorU) | ToMask(Extra::ST1),
		ToMask(Effect::SDF_LCD) | ToMask(Paint::Pattern_MirrorV) | ToMask(Extra::ST1),

		//Opacity
		ToMask(Effect::Opacity) | ToMask(Paint::Solid),
		ToMask(Effect::Opacity) | ToMask(Paint::Linear),
		ToMask(Effect::Opacity) | ToMask(Paint::Radial),
		ToMask(Effect::Opacity) | ToMask(Paint::Pattern),
		ToMask(Effect::Opacity) | ToMask(Paint::Pattern_Clamp),
		ToMask(Effect::Opacity) | ToMask(Paint::Pattern_Repeat),
		ToMask(Effect::Opacity) | ToMask(Paint::Pattern_Mirror),
		ToMask(Effect::Opacity) | ToMask(Paint::Pattern_MirrorU),
		ToMask(Effect::Opacity) | ToMask(Paint::Pattern_MirrorV),

		ToMask(Effect::Upsample) | ToMask(Paint::Solid),
		ToMask(Effect::Downsameple) | ToMask(Paint::Solid),

		ToMask(Effect::Shadow) | ToMask(Paint::Solid),
		ToMask(Effect::Blur) | ToMask(Paint::Solid)
	};

	template<Noesis::Shader::Enum shaderID, In... elements>
	constexpr uint64_t CreateMask()
	{
		return (ToMask(elements) | ... | gShaderMasks[shaderID]);
	}

	constexpr unsigned char elementSizesBits[] =
	{
		32 + 32,
		8 + 8 + 8 + 8,
		32 + 32,
		32 + 32,
		16 + 16 + 16 + 16,
		32,
		0
	};

	constexpr unsigned char GetElementSize(In elementType)
	{
		return elementSizesBits[(uint64)elementType] / 8;
	}

	template<In ...elements>
	constexpr unsigned char GetStride()
	{
		return (GetElementSize(elements) + ...);
	}

	struct SShaderDescInfo
	{
		Noesis::Shader	shader;
		TDescVar		descriptions;
		uint64			mask;
		uint8			stride;
	};

	using TDescInfoList = std::array<SShaderDescInfo, Noesis::Shader::Count>;

	template<Noesis::Shader::Enum shaderID, In... elements>
	constexpr SShaderDescInfo MakeInfo()
	{
		SShaderDescInfo info{
			shaderID
			, GetElementDescription<elements...>()
			, CreateMask<shaderID,elements...>()
			, GetStride<elements...>()
		};

		return info;
	}

	using namespace Noesis;

	constexpr TDescInfoList g_layoutInfoList = {
		MakeInfo<Shader::RGBA, In::Pos>(),
		MakeInfo<Shader::Mask, In::Pos>(),
		MakeInfo<Shader::Clear, In::Pos>(),


		MakeInfo<Shader::Path_Solid, In::Pos, In::Color>(),
		MakeInfo<Shader::Path_Linear, In::Pos, In::Tex0>(),
		MakeInfo<Shader::Path_Radial, In::Pos, In::Tex0>(),
		MakeInfo<Shader::Path_Pattern, In::Pos, In::Tex0>(),
		MakeInfo<Shader::Path_Pattern_Clamp, In::Pos, In::Tex0>(),
		MakeInfo<Shader::Path_Pattern_Repeat, In::Pos, In::Tex0>(),
		MakeInfo<Shader::Path_Pattern_MirrorU, In::Pos, In::Tex0>(),
		MakeInfo<Shader::Path_Pattern_MirrorV, In::Pos, In::Tex0>(),

		MakeInfo<Shader::Path_AA_Solid, In::Pos, In::Color, In::Coverage>(),
		MakeInfo<Shader::Path_AA_Linear, In::Pos, In::Tex0, In::Coverage>(),
		MakeInfo<Shader::Path_AA_Radial, In::Pos, In::Tex0, In::Coverage>(),
		MakeInfo<Shader::Path_AA_Pattern, In::Pos, In::Tex0, In::Coverage>(),

		MakeInfo<Shader::SDF_Solid, In::Pos, In::Color, In::Tex1>(),
		MakeInfo<Shader::SDF_Linear, In::Pos, In::Tex0, In::Tex1>(),
		MakeInfo<Shader::SDF_Radial, In::Pos, In::Tex0, In::Tex1>(),
		MakeInfo<Shader::SDF_Pattern, In::Pos, In::Tex0, In::Tex1>(),

		MakeInfo<Shader::SDF_LCD_Solid, In::Pos, In::Color, In::Tex1>(),
		MakeInfo<Shader::SDF_LCD_Linear, In::Pos, In::Tex0, In::Tex1>(),
		MakeInfo<Shader::SDF_LCD_Radial, In::Pos, In::Tex0, In::Tex1>(),
		MakeInfo<Shader::SDF_LCD_Pattern, In::Pos, In::Tex0, In::Tex1>(),

		MakeInfo<Shader::Opacity_Solid, In::Pos, In::Color, In::Tex1>(),
		MakeInfo<Shader::Opacity_Linear, In::Pos, In::Tex0, In::Tex1>(),
		MakeInfo<Shader::Opacity_Radial, In::Pos, In::Tex0, In::Tex1>(),
		MakeInfo<Shader::Opacity_Pattern, In::Pos, In::Tex0, In::Tex1>(),

		MakeInfo<Shader::Upsample, In::Pos, In::Tex0>(),
		MakeInfo<Shader::Downsample, In::Pos, In::Tex0>(),

		MakeInfo<Shader::Shadow, In::Pos, In::Color, In::Tex1, In::Tex1>(),
		MakeInfo<Shader::Blur, In::Pos, In::Color, In::Tex1, In::Tex1>()
	};
}

/*
		MakeInfo<Shader::Path_Solid, In::Pos, In::Color>(),
		MakeInfo<Shader::Path_Linear, In::Pos, In::Tex0>(),
		MakeInfo<Shader::Path_Radial, In::Pos, In::Tex0>(),
		MakeInfo<Shader::Path_Pattern, In::Pos, In::Tex0>(),
		MakeInfo<Shader::Path_Pattern_Clamp, In::Pos, In::Tex0>(),
		MakeInfo<Shader::Path_Pattern_Repeat, In::Pos, In::Tex0>(),
		MakeInfo<Shader::Path_Pattern_MirrorU, In::Pos, In::Tex0>(),
		MakeInfo<Shader::Path_Pattern_MirrorV, In::Pos, In::Tex0>(),
		MakeInfo<Shader::Path_Pattern_Mirror, In::Pos, In::Tex0>(),

		MakeInfo<Shader::SDF_Solid, In::Pos, In::Color, In::Tex1>(),
		MakeInfo<Shader::SDF_Linear, In::Pos, In::Tex0, In::Tex1>(),
		MakeInfo<Shader::SDF_Radial, In::Pos, In::Tex0, In::Tex1>(),
		MakeInfo<Shader::SDF_Pattern, In::Pos, In::Tex0, In::Tex1>(),*/