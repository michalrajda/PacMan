#pragma once
struct Flags
{
	uint16_t _isInitSDL : 1,
		_gameState : 1,
		_isRender : 1,
		_isWindow : 1,
		_isImgInit : 1,
		_isLives : 1,
		_reset : 1,
		_showPoints : 1,
		_saveName : 1,
		_showFPS : 1,
		_getname : 1,
		_nextLevel : 1,
		_actLevel : 1;
};

