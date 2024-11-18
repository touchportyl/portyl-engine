#pragma once


namespace ChronoShift
{
	enum PayloadTags
	{
		UNKNOWN,
		IMAGE,
		PREFAB,
		ENTITY,
		SHADER,
		SCENE,

		PAYLOADTAGS_LAST
	};

	inline const char* GetPayloadTagString(PayloadTags tag)
	{
		switch (tag)
		{
		case ChronoShift::IMAGE: return "IMAGE";
		case ChronoShift::PREFAB: return "PREFAB";
		case ChronoShift::ENTITY: return "ENTITY";
		case ChronoShift::SHADER: return "SHADER";
		case ChronoShift::SCENE: return "SCENE";
		default: return "UNKNOWN";
		}
	}

}