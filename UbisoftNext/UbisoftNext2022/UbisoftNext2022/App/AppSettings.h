#ifndef _APP_SETTINGS_H
#define _APP_SETTINGS_H

// We can use two coordinate systems. NATIVE and VIRTUAL.
// VIRTUAL has coords from 0.0f to APP_VIRTUAL_[WIDTH/HEIGHT].
// NATIVE has coords from -1.0f to 1.0f with 0.0f,0.0f as the center of the window.
#define APP_USE_VIRTUAL_RES		true					// Set true to use virtual coords.					
#define APP_VIRTUAL_WIDTH		(1024)					// This will be the effective x resolution regardless of actual screen/window res.
#define APP_VIRTUAL_HEIGHT		(768)					// This will be the effective y resolution regardless of actual screen/window res.

#define APP_MAX_FRAME_RATE		(60.0f)					// Maximum update rate.
#define APP_INIT_WINDOW_WIDTH	(APP_VIRTUAL_WIDTH)		// Initial window width.
#define APP_INIT_WINDOW_HEIGHT	(APP_VIRTUAL_HEIGHT)	// Initial window height.
#define APP_WINDOW_TITLE		("Ubisoft NEXT 2022")

#define APP_ENABLE_DEBUG_INFO_BUTTON		(XINPUT_GAMEPAD_DPAD_UP)
#define APP_QUIT_KEY						(VK_ESCAPE)

// Pad emulation. (Uses keyboard if no pad is present) Maps the following keys to the pad inputs.
// Note analog inputs are either 0 or 1 when emulated. 
#define APP_PAD_EMUL_LEFT_THUMB_LEFT	('A')
#define APP_PAD_EMUL_LEFT_THUMB_RIGHT	('D')
#define APP_PAD_EMUL_LEFT_THUMB_UP		('W')
#define APP_PAD_EMUL_LEFT_THUMB_DOWN	('S')
#define APP_PAD_EMUL_BUTTON_ALT_A		(VK_NEXT)
#define APP_PAD_EMUL_START				(VK_PRIOR)

#define APP_PAD_EMUL_RIGHT_THUMB_LEFT	(VK_NUMPAD4)
#define APP_PAD_EMUL_RIGHT_THUMB_RIGHT	(VK_NUMPAD6)
#define APP_PAD_EMUL_RIGHT_THUMB_UP		(VK_NUMPAD8)
#define APP_PAD_EMUL_RIGHT_THUMB_DOWN	(VK_NUMPAD2)

#define APP_PAD_EMUL_DPAD_UP			(VK_UP)
#define APP_PAD_EMUL_DPAD_DOWN			(VK_DOWN)
#define APP_PAD_EMUL_DPAD_LEFT			(VK_LEFT)
#define APP_PAD_EMUL_DPAD_RIGHT			(VK_RIGHT)
	
#define APP_PAD_EMUL_BUTTON_BACK		(VK_BACK)
#define APP_PAD_EMUL_BUTTON_A			(VK_DELETE)
#define APP_PAD_EMUL_BUTTON_B			(VK_END)
#define APP_PAD_EMUL_BUTTON_X			(VK_INSERT)
#define APP_PAD_EMUL_BUTTON_Y			(VK_HOME)

#define APP_PAD_EMUL_LEFT_TRIGGER		(VK_RETURN)
#define APP_PAD_EMUL_RIGHT_TRIGGER		(VK_SPACE)

#define APP_PAD_EMUL_BUTTON_LEFT_THUMB		('1')
#define APP_PAD_EMUL_BUTTON_RIGHT_THUMB		('2')
#define APP_PAD_EMUL_BUTTON_LEFT_SHOULDER	('3')
#define APP_PAD_EMUL_BUTTON_RIGHT_SHOULDER	('4')

#ifdef _DEBUG
#define APP_RENDER_UPDATE_TIMES				true
#else
#define APP_RENDER_UPDATE_TIMES				false
#endif

#define FRAND	(static_cast <float> (rand()) / static_cast <float> (RAND_MAX))
#define FRAND_RANGE(_MIN_, _MAX_ ) (FRAND * ((_MAX_)-(_MIN_)) + (_MIN_))
#define PI		(3.14159265359f)

#define MOVEMENT_THRESHOLD	 (0.0001f)
#define TRIGGER_THRESHOLD	 (0.01f)

// Sound effect paths
#define ENGINE_SOUND (".\\Assets\\Sounds\\Engine.wav")
#define LASER_SOUND (".\\Assets\\Sounds\\Laser.wav")
#define DEATH_SOUND (".\\Assets\\Sounds\\Death.wav")
#define PICKUP_SOUND (".\\Assets\\Sounds\\Coin.wav")

// Sprite paths
#define SHIP_SPRITES (".\\Assets\\Sprites\\player.bmp")
#define PLAYER_BULLET_SPRITE (".\\Assets\\Sprites\\playerBullet.bmp")
#define FUEL_SPRITE (".\\Assets\\Sprites\\fuel.bmp")
#define BUNKER_SPRITE (".\\Assets\\Sprites\\bunker.bmp")

#endif
