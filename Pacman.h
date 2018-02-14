#pragma once


// If Windows and not in Debug, this will run without a console window
// You can use this to output information when debugging using cout or cerr
#ifdef WIN32 
	#ifndef _DEBUG
		#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
	#endif
#endif


#include "S2D/S2D.h"
//Here I am defining how much of something that I want
#define MUNCHIECOUNT 30
#define GHOSTCOUNT 10
#define CHERRY 5

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;

//Here I have created a structure and inside the stucture and inside the structure includes everything that belongs to that specific structure
struct Player
{
	Vector2* position;
	Rect* sourceRect;
	Texture2D* texture;
	boolean dead;
	int direction;
	int frame;
	int currentFrameTime;
	float speedMultiplier;
	int score;
};

struct Enemy
{
	// Data to represent Munchie
	int _frameCount;
	Rect* sourceRect;
	Texture2D* texture;
	Vector2* position;
	int frame;
	int currentFrameTime;
	
};

struct MovingEnemy
{
	Rect* position;
	Rect* sourceRect;
	Texture2D* texture;
	int direction;
	float speed;
};

struct PickUp
{
	int _frameCount;
	Rect* sourceRect;
	Texture2D* texture;
	Vector2* position;
	int frame;
	int currentFrameTime;
	
};



// Declares the Pacman class which inherits from the Game class.
// This allows us to overload the Game class methods to help us
// load content, draw and update our game.
class Pacman : public Game
{
private:
	//Here I am creating a pointer to the structure
	Player* _pacman;
	Enemy* _munchies[MUNCHIECOUNT];
	MovingEnemy* _ghosts[GHOSTCOUNT];
	PickUp* _cherry[CHERRY];
	//Here I am creating a pointer to all of the sounds that I Will be using
	SoundEffect* _pop;
	SoundEffect* _dead;
	SoundEffect* _winner;
	SoundEffect* _terrible;
	SoundEffect* _flawless;
	SoundEffect* _closeToEnd;
	SoundEffect* _halfWay;
	SoundEffect* _lastPlace;
	SoundEffect* _powerUp;

	//Here I am setting pre defined ints for my game
	int munchieOffSet = 50;
	int cherryOffSet = 50;
	int munchieEaten = 0;
	int pacmanDeath = 0;
	int speedPowerTimer;

	//Input definition
	void Input(int elapsedTime, Input::KeyboardState* state);

	//Check Methods
	void CheckPaused(Input::KeyboardState* state, Input::Keys pauseKey);
	void CheckViewPortCollision(int elapsedTime, Input::KeyboardState* state);
	boolean CollisionCheck(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2);
	void CheckMunchieCollision();
	void CheckGhostCollision();
	void CheckCherryCollision();

	//Update Methods
	void UpdatePacman(int elapsedTime);
	void UpdateMunchie(int elapsedTime);
	void UpdateGhosts(int elapsedTime);
	void UpdateCherry(int elapsedTime);
	
	
	// Data for Menu
	Texture2D* _menuBackground;
	Rect* _menuRectangle;
	Vector2* _menuStringPosition;
	Vector2* _scoreScreen;
	
	//Here I am declaring the pause and pKeyDown
	bool _paused;
	bool _pKeyDown;
	
	//Constant data for Game Variables 
	const float _cPacmanSpeed;
	const float _cPacmanFrame;
	const int _cMunchieFrameTime;
	const int _cCherryFrameTime;
	

	// Position for String
	Vector2* _stringPosition;

	//Menu Screen
	bool _startMenu;

public:
	/// <summary> Constructs the Pacman class. </summary>
	Pacman(int argc, char* argv[]);

	/// <summary> Destroys any data associated with Pacman class. </summary>
	virtual ~Pacman();

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent();

	/// <summary> Called every frame - update game logic here. </summary>
	void virtual Update(int elapsedTime);

	/// <summary> Called every frame - draw game here. </summary>
	void virtual Draw(int elapsedTime);
};