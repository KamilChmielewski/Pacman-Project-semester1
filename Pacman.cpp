//Here I am making sure that this class has access to other classes and functions
#include "Pacman.h"
#include <iostream>
#include <sstream>
#include <time.h>

#define SPEED_POWER_TIME 3000

Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv), _cPacmanSpeed(0.1f), _cPacmanFrame(250), _cMunchieFrameTime(500), _cCherryFrameTime(500)
{
	//This is a seed to generate random position for the the munchies
	 srand(time(NULL));
	//here I am creating a new player and setting the players information like frame and speed
	 _pacman = new Player();
	_pacman->direction = 0;
	_paused = false;
	_pacman-> currentFrameTime = 0;
	_pacman-> frame = 0;
	_pacman->speedMultiplier = 1.0f;
	_pacman->dead = false;
	_pacman->score = 0;

	//Here I am loading all the sound effects that I will be using
	_pop = new SoundEffect();
	_dead = new SoundEffect(); 
	_winner = new SoundEffect();
	_terrible = new SoundEffect();
	_flawless = new SoundEffect();
	_closeToEnd = new SoundEffect();
	_halfWay = new SoundEffect();
	_lastPlace = new SoundEffect();
	_powerUp = new SoundEffect();
	
	//Creating each instance of the ghost by using a for loop
	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		_ghosts[i] = new MovingEnemy();
		_ghosts[i]->direction = 0;
		_ghosts[i]->speed = 0.2f;
	}
	

	//Creating each instance of the munchie by using a for loop
	
	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchies[i] = new Enemy();
		_munchies[i]->currentFrameTime;
		_munchies[i]->frame = 0;
		_munchies[i]->currentFrameTime = 0;
	}

	//Creating each instance of the cherry by using a for loop
	for (int i = 0; i < CHERRY; i++)
	{
		_cherry[i] = new PickUp();
		_cherry[i]->currentFrameTime;
		_cherry[i]->frame;
		_cherry[i]->currentFrameTime = 0;
	}
	
	//Here I am setting the pKeyDown to false so that you are not automatically put into the pause screen, but I am also defining it here.
	_pKeyDown = false;
	_startMenu = false;

	

	//Initialise important Game aspects
	Audio::Initialise();
	Graphics::Initialise(argc, argv, this, 1024, 768, false, 25, 25, "Pacman", 60);
	Input::Initialise();

	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();

}
//This is the destructor for the game.
Pacman::~Pacman()
{
	delete _pacman->texture;
	delete _pacman->sourceRect;
	delete _pacman->position;
	delete _ghosts[GHOSTCOUNT];
	delete[] _munchies;
	delete _cherry[CHERRY];

}

//Input definition
void Pacman::Input(int elapsedTime, Input::KeyboardState* state)
{

	// Checks if D key is pressed
	if (state->IsKeyDown(Input::Keys::D))
	{
		_pacman->position->X += _cPacmanSpeed * elapsedTime *_pacman->speedMultiplier; //Moves Pacman across X axis
		_pacman->direction = 0;
	}

	else if (state->IsKeyDown(Input::Keys::S))
	{
		_pacman->position->Y += _cPacmanSpeed * elapsedTime *_pacman->speedMultiplier; //Moves Pacman across y axis
		_pacman->direction = 1;
	}

	else if (state->IsKeyDown(Input::Keys::A))
	{
		_pacman->position->X -= _cPacmanSpeed * elapsedTime *_pacman->speedMultiplier; //Moves Pacman across y axisdsa
		_pacman->direction = 2;
	}

	else if (state->IsKeyDown(Input::Keys::W))
	{
		_pacman->position->Y -= _cPacmanSpeed * elapsedTime *_pacman->speedMultiplier; //Moves Pacman across y axis
		_pacman->direction = 3;// it will switch between the sprite sheet giving the animation effect.

	}
}

//Check Methods

//This method is the collision check method and it checks for a collision by taking two objects and comparing them to see if they are toching.
boolean Pacman::CollisionCheck(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2)
{
	 int left1 = x1;
	 int left2 = x2;
	 int right1 = x1 + width1;
	 int right2 = x2 + width2;
	 int top1 = y1;
	 int top2 = y2;
	 int bottom1 = y1 + height1;
	 int bottom2 = y2 + height2;

	 if (bottom1 < top2)
		 return false;
	 if (top1 > bottom2)
		 return false;
	 if (right1 < left2)
		 return false;
	 if (left1 > right2)
		 return false;

	 return true;

	 

}

//This method checks if the game if in a paused state.
void Pacman::CheckPaused(Input::KeyboardState* state, Input::Keys pauseKey)
{
	//if the P key is pressed then put the game into pause
	if (state->IsKeyDown(Input::Keys::P) && !_pKeyDown)
	{
		_pKeyDown = true;
		_paused = !_paused;

	}
	//if the P key is pressed again take the game out of pause
	if (state->IsKeyUp(Input::Keys::P))
	{
		_pKeyDown = false;
	}
}

//this method is a simple collision check against the wall by stating if pacman moves to a certain X or Y then dont let him go further.
void Pacman::CheckViewPortCollision(int elapsedTime, Input::KeyboardState* state)
{
	if (_pacman->position->X  > 761)
	{
		_pacman->position->X = 761;
	}
	if (_pacman->position->X  < 0)
	{
		_pacman->position->X = 0;
	}
	if (_pacman->position->Y > 600)
	{
		_pacman->position->Y = 600;
	}
	if (_pacman->position->Y  < 0)
	{
		_pacman->position->Y = 0;
	}
}

//Update Methods
void Pacman::UpdatePacman(int elapsedTime)
{

	_pacman->currentFrameTime += elapsedTime;
	//this if statement takes pacmans current frametime and compares it to pacmans frame.
	if (_pacman->currentFrameTime >_cPacmanFrame)
	{
		//This increments pacmans current frame time
		_pacman->frame++;
		//this sets pacman current frame time to 0 allowing it to go into the next if statement
		_pacman->currentFrameTime = 0;
		//this takes pacmans frame counts upto 2 and then resets the frame back to 0
		if (_pacman->frame >= 2)
		{
			_pacman->frame = 0;

		}
		//this gets pacmans x position so it knows where to draw it later
		_pacman->sourceRect->X = _pacman->sourceRect->Width* _pacman->frame;

	}
	//this determines the direction pacman is facing so that it knows which part of the spritesheet to draw
	_pacman->sourceRect->Y = _pacman->sourceRect->Height *_pacman->direction;
	
}
void Pacman::UpdateMunchie(int elapsedTime)
{
	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchies[i]->currentFrameTime += elapsedTime;
		if (_munchies[i]->currentFrameTime > _cMunchieFrameTime)
		{
			_munchies[i]->frame++;
			_munchies[i]->currentFrameTime = 0;
			if (_munchies[i]->frame >=2)
			{
				_munchies[i]->frame = 0;
			}
			_munchies[i]->sourceRect->X = _munchies[i]->sourceRect->Width* _munchies[i]->frame;
			_munchies[i]->sourceRect->Y = _munchies[i]->sourceRect->Height*_munchies[i]->frame;
		}
	}


	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		//Here I am checking if pacman has collided with a munchie
		if (CollisionCheck(_pacman->position->X, _pacman->position->Y, _pacman->sourceRect->Width, _pacman->sourceRect->Height, _munchies[i]->position->X, _munchies[i]->position->Y, _munchies[i]->sourceRect->Width, _munchies[i]->sourceRect->Height) == true)
		{ 
			//if a collision has occored then add 50 to munchieOffSet
			munchieOffSet += 50;
			//once a collision occurs we move the position of the munchie off screen and add munchieOffSet to it so that munchies don't collided with each other.
			_munchies[i]->position = new Vector2(-500, -500 + munchieOffSet);
			//Here I am incrementing for everytime pacman collideds with a munchie
			munchieEaten++;
			//Here I am incrementing the score
			_pacman->score++;
		
			//Once all the munchies have been eaten then we check the score and then decieded which audio file we will be playing
			if (munchieEaten == MUNCHIECOUNT)
			{
				if (_pacman->score == MUNCHIECOUNT)
				{
					Audio::Play(_flawless);
				}
				 if (_pacman->score <= 25 && _pacman->score > 15)
				{
					Audio::Play(_closeToEnd);
				}
				if (_pacman->score == 15)
				{
					Audio::Play(_halfWay);
				}
				 if (_pacman->score <= 14)
				{
					Audio::Play(_lastPlace);
				}

			}
			//this is the audio for when pacman and the munchie collideds
			Audio::Play(_pop);
		}
	}
	
		
}

void Pacman::UpdateCherry(int elapsedTime)
{
	for (int i = 0; i < CHERRY; i++)
	{
			_cherry[i]->currentFrameTime += elapsedTime;
		if (_cherry[i]->currentFrameTime > _cCherryFrameTime)
		{
			_cherry[i]->frame++;
			_cherry[i]->currentFrameTime = 0;
			if (_cherry[i]->frame >= 2)
			{
				_cherry[i]->frame = 0;
			}
			_cherry[i]->sourceRect->X = _cherry[i]->sourceRect->Width* _cherry[i]->frame;
			_cherry[i]->sourceRect->Y = _cherry[i]->sourceRect->Height*_cherry[i]->frame;
		}
	
		for (int i = 0; i < CHERRY; i++)
		{
			if (CollisionCheck(_pacman->position->X, _pacman->position->Y, _pacman->sourceRect->Width, _pacman->sourceRect->Height,
				_cherry[i]->position->X, _cherry[i]->position->Y, _cherry[i]->sourceRect->Width, _cherry[i]->sourceRect->Height) == true)
			{
				
				cherryOffSet += 50;
				//Here I am setting the power up spped to 0
				speedPowerTimer = 0;
				_cherry[i]->position = new Vector2(-500, -500 + cherryOffSet);
				Audio::Play(_powerUp);

			}
		}
	}

	//Here I am adding elapsedTime to the power up and if there is a collision then speed up pacman for 3 seconds because it is defined at the top to 3000 and this is calculating in miliseconds.
	speedPowerTimer += elapsedTime;
	if (speedPowerTimer < SPEED_POWER_TIME)
	{
		_pacman->speedMultiplier = 4.0f;
	}
	else
	{
		_pacman->speedMultiplier = 2.0f;

	}
}



void Pacman::UpdateGhosts(int elapsedTime)
{
	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		if (_ghosts[i]->direction == 0) //Moves Right
		{
			_ghosts[i]->position->X += _ghosts[0]->speed* elapsedTime;
		}
		else if (_ghosts[i]->direction == 1) //Moves Left
		{
			_ghosts[i]->position->X -= _ghosts[i]->speed* elapsedTime;
		}
		if (_ghosts[i]->position->X + _ghosts[i]->position->Width >=
			Graphics::GetViewportWidth()) //Hits Right edge
		{
			_ghosts[i]->direction = 1; //Change direction
		}
		else if (_ghosts[i]->position->X <= 100) //Hits left edge
		{
			_ghosts[i]->direction = 0; //Change direction									   
			//once a ghosts hits 100 on the x scale then give it a new random position
			_ghosts[i]->position = new Rect(rand()% 751 + 10, rand() % 600 + 10, 20, 20);


		}

		if (CollisionCheck(_pacman->position->X, _pacman->position->Y, _pacman->sourceRect->Width, _pacman->sourceRect->Height, _ghosts[i]->position->X,
			_ghosts[i]->position->Y, _ghosts[i]->sourceRect->Width, _ghosts[i]->sourceRect->Height) == true)
		{
			//Here I am counting how many times pacman has died so that  when it hits 5 it plays a sound
			 pacmanDeath++;
			//if pacman collideds with a ghost then decrement the score
			 _pacman->score--;
			 //set pacmans position
			_pacman->position = new Vector2(0, 350);
			Audio::Play(_dead);

			if (pacmanDeath == 5)
			{
				Audio::Play(_terrible);
				//resetting pacmanDeath to 0 so that it can be counted again
				pacmanDeath = 0;
			}
		}
	}

	
	
	
}

//this method compares two munchies and if they are toching give the first munchie a new X, Y position
void Pacman::CheckMunchieCollision()
{
	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		for (int x = 0; x < MUNCHIECOUNT; x++)
		{
			//If i is not the same as X then check for a collison, because you dont want the munchie to be comparing against itself
			if (i != x)
			{
				if (CollisionCheck(_munchies[i]->position->X, _munchies[i]->position->Y, _munchies[i]->sourceRect->Width, _munchies[i]->sourceRect->Height,
					_munchies[x]->position->X, _munchies[x]->position->Y, _munchies[x]->sourceRect->Width, _munchies[x]->sourceRect->Height) == true)
				{
					_munchies[i]->position = new Vector2(rand() % 751, rand() % 600);
				}
			}
		}
		
	}
}

void Pacman::CheckGhostCollision()
{
	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		for (int x = 0; x < GHOSTCOUNT; x++)
		{
			if (i != x)
			{
				if (CollisionCheck(_ghosts[i]->position->X, _ghosts[i]->position->Y, _ghosts[i]->sourceRect->Width, _ghosts[i]->sourceRect->Height, 
					_ghosts[x]->position->X, _ghosts[x]->position->Y, _ghosts[x]->sourceRect->Width, _ghosts[x]->sourceRect->Height) == true)
				{
					_ghosts[i]->position = new Rect(rand() % 751 + 10, rand() % 600 + 10, 20, 20);
				}
			}
		}
		
	}
}

void Pacman::CheckCherryCollision()
{
	for (int i = 0; i < CHERRY; i++)
	{
		for (int x = 0; x < CHERRY; x++)
		{
			if (i != x)
			{
				if (CollisionCheck(_cherry[i]->position->X, _cherry[i]->position->Y, _cherry[i]->sourceRect->Width, _cherry[i]->sourceRect->Height,
					_cherry[x]->position->X, _cherry[x]->position->Y, _cherry[x]->sourceRect->Width, _cherry[x]->sourceRect->Height) == true)
				{
					_cherry[i]->position = new Vector2(rand() % 751, rand() % 600);
				}
			}
		}

	}
}





void Pacman::LoadContent()
{
	// Load Pacman
	_pacman->texture = new Texture2D();
	_pacman->texture->Load("Textures/Pacman.tga", false);
	_pacman->position = new Vector2(0.0f, 350.0f);
	_pacman->sourceRect = new Rect(0.0f, 0.0f, 32, 32);

	//loading sounds
	_pop->Load("Sounds/pop.wav");
	_dead->Load("Sounds/dead.wav");
	_winner->Load("Sounds/win.wav");
	_terrible->Load("Sounds/keepDying.wav");
	_flawless->Load("Sounds/Flawless-victory.wav");
	_closeToEnd->Load("Sounds/closeToEnd.wav");
	_halfWay->Load("Sounds/halfway.wav");
	_lastPlace->Load("Sounds/lastPlace.wav");
	_powerUp->Load("Sounds/pickup.wav");

	//load Ghosts
	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		_ghosts[i]->texture = new Texture2D();
		_ghosts[i]->texture->Load("Textures/GhostBlue.png", true);
		_ghosts[i]->position = new Rect(rand() % Graphics::GetViewportWidth(), rand() % Graphics::GetViewportHeight(), 20, 20);
		_ghosts[i]->sourceRect = new Rect(0.0f, 0.0f, 20, 20);
	}
	

	// Load Munchie
	Texture2D* munchieTex = new Texture2D();
	munchieTex->Load("Textures/Munchie.tga", true);
	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchies[i]->texture = munchieTex;
		_munchies[i]->sourceRect = new Rect(0.0f, 0.0f, 12, 12);	
		_munchies[i]->position = new Vector2(rand() % 751 + 10, rand() % 600 + 10);		
	}
	
	//Load Cherry
	
	for (int i = 0; i < CHERRY; i++)
	{
		_cherry[i]->texture = new Texture2D();
		_cherry[i]->texture->Load("Textures/Cherry.png", true);
		_cherry[i]->sourceRect = new Rect(0.0f, 0.0f, 32, 32);
		_cherry[i]->position = new Vector2(rand() % 751 + 10, rand() % 600 + 10);
	}
		
		

	
	// Set string position
	_stringPosition = new Vector2(10.0f, 25.0f);

	// Setting the score screen posistion
	_scoreScreen = new Vector2(0, 50);
	//Here I am setting what the value of speedPowerTime
	speedPowerTimer = 3000;
}

void Pacman::Update(int elapsedTime)
{

	//Checks if the game has started yet
	if (!_startMenu)
	{
		_startMenu = true;// Here should be an input to check when space is pressed
		 //This if statement looks if the game has started and if it has force it to the menu screen.
	}

	//Adding the mouse state
	Input::MouseState* mouseState = Input::Mouse::GetState();
	//Trying to make the munchie move to the x and y position of the mouse
	Input::ButtonState myButtonState = Input::ButtonState::PRESSED;


	// Gets the current state of the keyboard

	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();
	CheckPaused(keyboardState, Input::Keys::P);
	
	//if the game is not paused then call all the methods inside of here
	if (!_paused)
	{
		Input(elapsedTime, keyboardState);
		UpdatePacman(elapsedTime);
		UpdateMunchie(elapsedTime);
		UpdateGhosts(elapsedTime);
		UpdateCherry(elapsedTime);
		CheckGhostCollision();
		CheckMunchieCollision();
		CheckCherryCollision();
		CheckViewPortCollision(elapsedTime, keyboardState);

	}
}


void Pacman::Draw(int elapsedTime)
{
	// Allows us to easily create a string
	std::stringstream stream;
	//This tells me the X and Y position of pacman currently
	stream << "Pacman X: " << _pacman->position->X << " Y: " << _pacman->position->Y;

	SpriteBatch::BeginDraw(); // Starts Drawing

	
		//Here I am drawing the score so you can see your score in game
		std::stringstream menuStream;
		menuStream << "Score: " << _pacman->score;
		SpriteBatch::DrawString(menuStream.str().c_str(), _scoreScreen, Color::Yellow);
	


	//if pacman is not dead then draw everything inside of the if statement
	if (!_pacman->dead)
	{
		SpriteBatch::Draw(_pacman->texture, _pacman->position, _pacman->sourceRect); // Draws Pacman
		
		for (size_t i = 0; i < GHOSTCOUNT; i++)
		{
			SpriteBatch::Draw(_ghosts[i]->texture, _ghosts[i]->position, _ghosts[i]->sourceRect);
		}


		for (int i = 0; i < MUNCHIECOUNT; i++)
		{
			SpriteBatch::Draw(_munchies[i]->texture, _munchies[i]->position, _munchies[i]->sourceRect);
		}

		for (int i = 0; i < CHERRY; i++)
		{
			SpriteBatch::Draw(_cherry[i]->texture, _cherry[i]->position, _cherry[i]->sourceRect);
		}
	}





	// Draws String
	SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green);
	
	if (_paused)
	{
		std::stringstream menuStream;
		menuStream << "PAUSED!";
		SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menuStringPosition,
			Color::Red);
	}

	SpriteBatch::EndDraw(); // Ends Drawing
}
