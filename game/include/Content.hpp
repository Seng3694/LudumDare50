#pragma once

enum class Content
{
	MapFrameTexture,
	MapTilesTexture,
	MowBotIconTexture,
    MowBotTexture,
	MownGrassTexture,
    ScrollingRectangleTexture,
	SFMLTexture,
	TransitionBannerLeftTexture,
	TransitionBannerRightTexture,
	VolumeBarTexture,
	VolumeBarPartTexture,
	MapTilesTileset,
	MowBotTileset,
	MownGrassTileset,
	HitSound,
	MowSound,
	SelectSound,
	WushSound,
	MenuSoundtrack,
	PlayStateSoundtrack,
	MonogramFont
};

#include <ContentManager.hpp>

typedef gjt::ContentManager<Content> GameContentManager;
