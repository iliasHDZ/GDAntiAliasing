#ifndef __PLAYLAYER_H__
#define __PLAYLAYER_H__

#include <gd.h>

namespace gd {

	class GJBaseGameLayer;
	class GJGameLevel;

	class PlayLayer : public GJBaseGameLayer {
		public:
			static PlayLayer* create(GJGameLevel* level) {
				return reinterpret_cast<PlayLayer*(__fastcall*)(
					GJGameLevel*
				)>(
					base + 0x1fb6d0
				)(level);
			}
	};
}

#endif