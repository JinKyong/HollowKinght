#pragma once

class Player;

/*************** KEY ***************/
#define KEY_RIGHT	VK_RIGHT
#define KEY_LEFT	VK_LEFT
#define KEY_UP		VK_UP
#define KEY_DOWN	VK_DOWN
#define KEY_JUMP	VK_SPACE
#define KEY_DASH	VK_LSHIFT
#define KEY_SLASH	'X'
#define KEY_FOCUS	'C'
#define KEY_FIRE	'Z'
#define KEY_INVEN	'V'

#define KEY_GOD		'B'
#define KEY_SOUL	'G'
#define KEY_HURT	'F'
#define KEY_WING	VK_F1
/***********************************/

enum PLAYERSTATE {
	IDLE,
	RUN,
	JUMP,
	DOUBLEJUMP,
	FALL,
	DASH,
	DASH2IDLE,
	SLASH,
	UPSLASH,
	DOWNSLASH,
	FIRE,
	FOCUS,
	FOCUS2IDLE,
	LOOKUP,
	LOOKUP2IDLE,
	LOOKDOWN,
	LOOKDOWN2IDLE,
	WALL,
	WALLJUMP,
	STUN,
	DEATH,
	INVENTORY,
	ENDSTATE
};

class State
{
private:

protected:
	static Player* _player;

	image* _img;
	float _count;
	int _direct;
	bool _reverse;

public:
	State() {};
	~State() {};

	virtual HRESULT init(Player* player, bool reverse = false);
	virtual void release() = 0;
	virtual void update() = 0;
	virtual void render(HDC hdc) = 0;
	virtual void updateDirect(int direct) = 0;
	virtual void updateRect() = 0;

	virtual int getStateNum() = 0;
};
