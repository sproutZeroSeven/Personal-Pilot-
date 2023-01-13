#include "raylib.h"  
#include <cmath>
#include <time.h>  
#include <iostream>
#include "string"

//structs
struct Window 
{
	int width = 512;
	int height = 380;
	const char* title = "Personal Pilot!";
}window;

struct character 
{
	//load player sprite 
	
	
	//set dimensions
	float width = 60;//updated to match width of single sprite
	float height = 28;//manually determined the height
	int hp = 3;
	int totalHp = 3;

	float Yvelocity = 0;
	float Xvelocity = 0;
	float baseVelocity = 300;
	
	float posY = window.height - height;
	float posX = window.height / 2 - width / 2;
	
	//animation data
	
	int frame = 2;
	int startFrame = 2;
	int maxFrame = 5;
	float currentSprite = 162;
	float updateTime = 1.0 / 4.0;
	float runningTime = 0;
	int frameUpdater = 1;
	//is dead 
	bool isDead = false;
	//has gotten a point from the enemy
	bool isScored = false;
	bool isCompletedDeath = false;
}player, bug;

struct enemyShip 
{
	
	//set dimensions
	float width = 60;//updated to match width of single sprite
	float height = 28;//manually determined the height
	int hp = 3;

	float Yvelocity = 0;
	float Xvelocity = 0;
	float baseVelocity = 300;
	float posY = 0;
	float posX = 0;

	//animation data

	int frame = 2;
	int startFrame = 2;
	int maxFrame = 5;
	float currentSprite = 162;
	float updateTime = 1.0 / 4.0;
	float runningTime = 0;
	int frameUpdater = 1;
	//is dead 
	bool isDead = false;
	bool isCompletedDeath = false;

	//has gotten a point from the enemy
	bool isScored = false;
	bool homing = false;
	
	//shooting
	int shootFrame = 0;
	int frameCounter = 0;
}enemyShipDefault, enemyHomingShip;


struct bullets 
{
	//set dimensions
	//raveesh was here
	float width = 8;//updated to match width of single sprite
	float height = 6;//manually determined the height

	float Xvelocity = 0;
	float Yvelocity = 0;
	float baseVelocity = 350;

	float posY = 700;
	float posX = 700;
	float currentSprite = 0;
	int frame = 2;
	float updateTime = 1.0 / 4.0;
	float runningTime = 0;
	int frameUpdater = 1;

	float homingTheta = 0;
	
	//sent off screen
	bool inStorage = true;
	bool isHoming = false;
} playerBullet, enemyBullet, enemyHomingMissle;

int main()
{
	bool startGame = false, mainMenu = true;
	int numberOfActivePlayerBullets = 0, numberOfActiveEnemyBullets = 0, numberOfActiveEnemyHomingBullets = 0;
	float difference, BGposX = 0;
	//sets the window dimensions
	InitWindow(window.width, window.height, window.title);
	Texture2D sprite = LoadTexture("gameAssets/playerShips/rocketshipBlue(60x28).png");


	//enemy texture
	Texture2D defaultEnemySpritePink = LoadTexture("gameAssets/enemies/defaultEnemy/defaultEnemyPink(33x35).png");	
	Texture2D defaultEnemySpriteYellow = LoadTexture("gameAssets/enemies/defaultEnemy/defaultEnemyYellow(33x35).png");
	Texture2D defaultEnemySpriteBlue = LoadTexture("gameAssets/enemies/defaultEnemy/defaultEnemyBlue(33x35).png");
	Texture2D defaultEnemySpriteGreen = LoadTexture("gameAssets/enemies/defaultEnemy/defaultEnemyGreen(33x35).png");

	Texture2D enemyShipSprite = LoadTexture("gameAssets/enemies/enemySpaceShips/enemySpaceship(63x24).png");

	Texture2D enemyHomingShipSprite = LoadTexture("gameAssets/enemies/enemySpaceShips/EnemyHomingShip(60x33).png");
	//backround texture
	Texture2D bg = LoadTexture("gameAssets/spaceBG(1500x380).png");

	//player bullets texture
	Texture2D playerBulletTexture = LoadTexture("gameAssets/projectiles/playerPellets(8x6).png");
	Texture2D enemyBulletTexture = LoadTexture("gameAssets/projectiles/enemyPellet(12x12).png");
	Texture2D enemyHomingBulletTexture = LoadTexture("gameAssets/projectiles/homingMissle(25x11).png");


	//player hearts 
	Texture2D playerHeart = LoadTexture("gameAssets/wrench(21x21).png");

	Texture2D lossBg = LoadTexture("gameAssets/uLoseScreen.png");
	
	Texture2D sMenuBg = LoadTexture("gameAssets/menus/startMenu/logo(512x380).png");

	Texture2D sMenuOptions = LoadTexture("gameAssets/menus/startMenu/options(189x127).png");

	Texture2D dMenuOptions = LoadTexture("gameAssets/menus/deathScreen/options(189x127).png");

	float startMenuFrame = 0, sMenuOptionSelected = 0, startMenuRunningTime = 0, dMenuOptionSelected = 0;
	int startMenuFrameChange = window.width, sMenuOptionWidth = 189, dMenuOptionWidth = 189, score = 0;

	SetTargetFPS(60);
	while (!(WindowShouldClose())) 
	{//menu
		const float dT = GetFrameTime();
		
		if (mainMenu) {
			startMenuRunningTime += dT;

			if (startMenuRunningTime >= 0.1)
			{
				startMenuRunningTime = 0;
				//updates the sprite
				startMenuFrame += startMenuFrameChange;
				//reset frame and set sprite to middle animation to loop
				if (startMenuFrame >= window.width * 3)
				{
					startMenuFrameChange = -window.width;
				}
				else if (startMenuFrame <= 0) {
					startMenuFrameChange = window.width;
				}
			}

			if ((IsKeyPressed(KEY_DOWN)) or (IsKeyPressed(KEY_S)))
			{

				sMenuOptionSelected += sMenuOptionWidth;

			}
			if ((IsKeyPressed(KEY_UP)) or (IsKeyPressed(KEY_W)))
			{

				sMenuOptionSelected -= sMenuOptionWidth;

			}
			if ((IsKeyPressed(KEY_SPACE)) or (IsKeyPressed(KEY_ENTER)))
			{
				int optionSelected = (sMenuOptionSelected / 189) + 1;

				if (optionSelected == 1)
				{
					startGame = true;
					mainMenu = false;
				}
				else if (optionSelected == 2)
				{

				}
				else if (optionSelected == 3)
				{
					break;
				}


			}

			BeginDrawing();
			ClearBackground(WHITE);
			//DrawTextureEx(sMenuBg, {startMenuFrame,0 }, 0, 145/100, WHITE);
			//doesnt work as a bg so i drew it as a sprite
			DrawTextureRec(sMenuBg, (Rectangle{ startMenuFrame, 0, 676, 380 }), (Vector2{ 0, 0 }), WHITE);
			DrawTextureRec(sMenuOptions, (Rectangle{ sMenuOptionSelected, 0, 189, 127 }), (Vector2{ 290, 240 }), WHITE);
			DrawText("By Kylar McLean & Carter Newman", (5), (360), 17, SKYBLUE);


			EndDrawing();
		}
		else if (startGame)
		{	
			score = 0;
			player.hp = 3;
			const int numberOfBugs = 12;
			character bugs[numberOfBugs];
			srand(time(0));
			//updates bugs locations
			for (int i = 0; i < numberOfBugs; i++)
			{
				//set to template
				bugs[i].width = 33;
				bugs[i].height = 35;
				bugs[i].frame = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (4)));
				bugs[i].currentSprite = 0;
				bugs[i].updateTime = 1.0 / ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (5))) + 8);
				bugs[i].posY = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (window.height - bugs[1].width - 10)));
				bugs[i].Xvelocity = -(static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 50)) - 50;
				bugs[i].hp = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (4))) + 1;
				bugs[i].posX += 300 * i + window.width;
			}
			const int numberOFEnemyShips = 12;
			enemyShip enemyShips[numberOFEnemyShips];

			for (int i = 0; i < numberOFEnemyShips; i++)
			{
				//set to template
				enemyShips[i].width = 63;
				enemyShips[i].height = 24;

				enemyShips[i].frame = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (4)));
				enemyShips[i].currentSprite = 0;
				enemyShips[i].updateTime = 1.0 / ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (2))) + 5);
				enemyShips[i].posY = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (window.height - bugs[1].width - 10)));
				enemyShips[i].Xvelocity = -(static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 50)) - 25;
				enemyShips[i].posX = (300 * i) + window.width;
				enemyShips[i].shootFrame = ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (8))) + 12);
			}


			const int numberOfEnemyHomingShips = 12;
			enemyShip enemyHomingShips[numberOfEnemyHomingShips];
			for (int i = 0; i < numberOfEnemyHomingShips; i++)
			{
				//set to template
				enemyHomingShips[i].width = 60;
				enemyHomingShips[i].height = 29;

				enemyHomingShips[i].frame = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (4)));
				enemyHomingShips[i].currentSprite = 0;
				enemyHomingShips[i].updateTime = 1.0 / ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (2))) + 5);
				enemyHomingShips[i].posY = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (window.height - bugs[1].width - 10)));
				enemyHomingShips[i].Xvelocity = -(static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 50)) - 25;
				enemyHomingShips[i].posX = (300 * i) + window.width;
				enemyHomingShips[i].shootFrame = ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (16))) + 16);
			}

			const int numberOfPlayerBullets = 24;

			bullets playerBullets[numberOfPlayerBullets];
			for (int i = 0; i < numberOfPlayerBullets; i++)
			{
				playerBullets[i].currentSprite = i * 8;
			}

			const int numberOfEnemyBullets = 40;

			bullets enemyBullets[numberOfEnemyBullets];
			for (int i = 0; i < numberOfEnemyBullets; i++)
			{
				enemyBullets[i].height = 12;
				enemyBullets[i].width = 12;
				enemyBullets[i].baseVelocity = 200;
			}

			const int numberOfEnemyHomingBullets = 15;
			bullets enemyHomingBullets[numberOfEnemyHomingBullets];
			for (int i = 0; i < numberOfEnemyHomingBullets; i++)
			{
				enemyHomingBullets[i].height = 11;
				enemyHomingBullets[i].width = 25;
				enemyHomingBullets[i].baseVelocity = -100;
				enemyHomingBullets[i].isHoming = true;

				enemyHomingShips[i].updateTime = 1.0 / ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (6))) + 6);

				enemyHomingBullets[i].posX = window.width;
				enemyHomingBullets[i].posY = window.height / 2;
			}
			while (!(WindowShouldClose()))//game
			{
				//when dead
				
				if (!player.isDead or !player.hp <= 0) {
					const float dT = GetFrameTime();

					//begin drawin
					BeginDrawing();
					ClearBackground(WHITE);

					//catching bullet at end of window
					for (int i = 0; i < numberOfPlayerBullets; i++)
					{
						if (!(playerBullets[i].inStorage) and (playerBullets[i].posX >= (window.width + 30)))
						{
							playerBullets[i].Xvelocity = 0;
							playerBullets[i].posX = 700;
							playerBullets[i].posY = 700;
						}
					}

					//catches enemy bullets at the start of the window and sends them off screen with no velocity as dead
					for (int i = 0; i < numberOfEnemyBullets; i++)
					{
						if (!(enemyBullets[i].inStorage) and (enemyBullets[i].posX <= -40))
						{
							enemyBullets[i].Xvelocity = 0;
							enemyBullets[i].posX = 700;
							enemyBullets[i].posY = 700;
							enemyBullets[i].inStorage = true;
						}
					}
					//catches enemyships at the start of the window and sends them off screen with no velocity as dead
					for (int i = 0; i < numberOFEnemyShips; i++)
					{
						if ((enemyShips[i].isCompletedDeath) or (enemyShips[i].posX <= -140))
						{
							enemyShips[i].Xvelocity = -(static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 50)) - 50;
							enemyShips[i].posX = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX / ((1000 / (score + 1) + 1) * window.width))) + window.width;
							enemyShips[i].posY = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (window.height - enemyShips[1].width - 10)));
							enemyShips[i].isDead = false;
							enemyShips[i].shootFrame += ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (6))) + 4);
							enemyShips[i].isScored = false;

						}
					}
					//catches enemyHomingShips at the start of the window and sends them off screen with no velocity as dead
					for (int i = 0; i < numberOfEnemyHomingShips; i++)
					{
						if ((enemyHomingShips[i].isCompletedDeath) or (enemyHomingShips[i].posX <= -140))
						{
							enemyHomingShips[i].Xvelocity = -(static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 50)) - 50;
							enemyHomingShips[i].posX = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX / ((1000 / (score + 1) + 1) * window.width))) + window.width;
							enemyHomingShips[i].posY = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (window.height - enemyHomingShips[1].width - 10)));
							enemyHomingShips[i].isDead = false;
							enemyHomingShips[i].shootFrame += ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (6))) + 4);
							enemyHomingShips[i].isScored = false;

						}
					}
					//catches bugs at the start of the window and sends them off screen with no velocity as dead
					for (int i = 0; i < numberOfBugs; i++)
					{
						if ((bugs[i].isCompletedDeath) or (bugs[i].posX <= -40))
						{
							bugs[i].Xvelocity = -(static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 50)) - 50;
							bugs[i].posX = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX / ((1000 / (score + 1) + 1) * window.width))) + window.width;
							bugs[i].posY = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (window.height - bugs[1].width - 10)));
							bugs[i].isDead = false;
							bugs[i].isCompletedDeath = false;
							bugs[i].hp = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (4))) + 1;
							bugs[i].frame = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (5)));
							bugs[i].currentSprite = 0;
							bugs[i].isScored = false;

						}
					}

					//moves bugs
					for (int i = 0; i < numberOfBugs; i++)
					{
						if (!bugs[i].isDead)
						{
							bugs[i].posX += bugs[i].Xvelocity * dT;
						}
					}
					//moves enemy ships
					for (int i = 0; i < numberOFEnemyShips; i++)
					{
						enemyShips[i].posX += enemyShips[i].Xvelocity * dT;
					}

					//moves enemy Homing ships
					for (int i = 0; i < numberOfEnemyHomingShips; i++)
					{
						enemyHomingShips[i].posX += enemyHomingShips[i].Xvelocity * dT;
					}

					//moves bullets
					for (int i = 0; i < numberOfPlayerBullets; i++)
					{
						playerBullets[i].posX += playerBullets[i].Xvelocity * dT;
					}

					//moves enemy Bullets
					for (int i = 0; i < numberOfEnemyBullets; i++)
					{
						enemyBullets[i].posX -= enemyBullets[i].Xvelocity * dT;
					}

					//homing bullet tracking
					for (int i = 0; i < numberOfEnemyHomingBullets; i++)
					{
						if (enemyHomingBullets[i].isHoming)
						{
							if (!enemyHomingBullets[i].inStorage)
							{
								float baseVelocity = enemyHomingBullets[i].baseVelocity * dT;

								if (enemyHomingBullets[i].posX > player.posX)
								{
									enemyHomingBullets[i].homingTheta = atan((enemyHomingBullets[i].posY - player.posY) / (enemyHomingBullets[i].posX - player.posX));
								}
								else if (enemyHomingBullets[i].posX < player.posX)
								{
									enemyHomingBullets[i].homingTheta = atan(-1 * (player.posY - enemyHomingBullets[i].posY) / (player.posX - enemyHomingBullets[i].posX));
								}
								enemyHomingBullets[i].posX += baseVelocity * cos(enemyHomingBullets[i].homingTheta);
								enemyHomingBullets[i].posY += baseVelocity * sin(enemyHomingBullets[i].homingTheta);
							}
						}
					}

					if (player.posY >= window.height - player.height)
					{
						//rectangle is on the ground
						player.Yvelocity = 0;
						player.posY = window.height - player.height;
					}

					//grounded and pressed space
					if ((IsKeyDown(KEY_W) or IsKeyDown(KEY_UP)) && (player.posY >= 0))
					{
						player.Yvelocity = player.baseVelocity;
					}
					else
					{
						if (player.Yvelocity > 0)
						{
							player.Yvelocity = player.Yvelocity / 2;
						}
					}

					if ((IsKeyDown(KEY_S) or IsKeyDown(KEY_DOWN)) && player.posY <= window.height)
					{
						player.Yvelocity = -player.baseVelocity;
					}
					else
					{
						if (player.Yvelocity < 0)
						{
							player.Yvelocity = player.Yvelocity / 2;
						}
					}

					if ((IsKeyDown(KEY_A) or IsKeyDown(KEY_LEFT)) && player.posX >= 0)
					{
						player.Xvelocity = player.baseVelocity;
					}
					else
					{
						if (player.Xvelocity > 0)
						{
							player.Xvelocity = player.Xvelocity / 2;
						}
					}
					if ((IsKeyDown(KEY_D) or IsKeyDown(KEY_RIGHT)) && player.posX <= window.width - player.width)
					{
						player.Xvelocity = -player.baseVelocity;
					}
					else
					{
						if (player.Xvelocity < 0)
						{
							player.Xvelocity = player.Xvelocity / 2;
						}
					}

					if ((IsKeyPressed(KEY_SPACE)))
					{
						if (numberOfActivePlayerBullets == numberOfPlayerBullets)
						{
							numberOfActivePlayerBullets = 0;
						}
						else
						{
							playerBullets[numberOfActivePlayerBullets].posX = player.posX + player.width;
							playerBullets[numberOfActivePlayerBullets].posY = player.posY + (player.height / 2);
							playerBullets[numberOfActivePlayerBullets].Xvelocity = playerBullets[numberOfActivePlayerBullets].baseVelocity;
							playerBullets[numberOfActivePlayerBullets].inStorage = false;
							numberOfActivePlayerBullets++;
						}
					}

					if ((IsKeyPressed(KEY_ESCAPE)))
					{
						bool looping = true;
						while (looping)
						{
							//////looping menu
						}
					}


					//player box colliders
					Rectangle playerFrontWingRec
					{
						player.posX + 32,
						player.posY + 6,
						4,
						player.height - 6
					};
					Rectangle playerBackWingRec
					{
						player.posX + 14,
						player.posY + 1,
						9,
						18
					};
					Rectangle playerLengthRec
					{
						player.posX,
						player.posY + 9,
						player.width,
						10
					};


					for (int i = 0; i < numberOfBugs; i++)
					{


						Rectangle bugBodyrec
						{
							bugs[i].posX,
							bugs[i].posY + 10,
							bugs[i].width,
							bugs[i].height - 10
						};
						Rectangle bugAntennaRec
						{
							bugs[i].posX,
							bugs[i].posY,
							20,
							12
						};

						if (!bugs[i].isDead)
						{
							if ((CheckCollisionRecs(bugBodyrec, playerLengthRec) or CheckCollisionRecs(bugBodyrec, playerBackWingRec) or CheckCollisionRecs(bugBodyrec, playerFrontWingRec)) or (CheckCollisionRecs(bugAntennaRec, playerLengthRec) or CheckCollisionRecs(bugAntennaRec, playerBackWingRec) or CheckCollisionRecs(bugAntennaRec, playerFrontWingRec))) {
								player.hp -= 1;
								bugs[i].currentSprite = 6 * bugs[i].width;
								bugs[i].hp = 0;
								bugs[i].Xvelocity = 0;
								bugs[i].frame = 0;
								bugs[i].isDead = true;
							}
							else
							{
								for (int e = 0; e < numberOfPlayerBullets; e++)
								{
									Rectangle playerBulletRec
									{
										playerBullets[e].posX,
										playerBullets[e].posY,
										playerBullets[e].width,
										playerBullets[e].height
									};
									if ((CheckCollisionRecs(playerBulletRec, bugBodyrec) or (CheckCollisionRecs(playerBulletRec, bugAntennaRec))))
									{
										playerBullets[e].inStorage = true;
										playerBullets[e].posX = 700;
										playerBullets[e].posY = 700;
										playerBullets[e].Xvelocity = 0;
										if (!bugs[i].hp == 1) {
											bugs[i].hp -= 1;
										}

										if (bugs[i].hp == 0)
										{
											bugs[i].isDead = true;
											bugs[i].currentSprite = 6 * bugs[i].width;
											bugs[i].Xvelocity = 0;
											bugs[i].frame = 0;
										}
									}
								}
							}
						}
					}

					for (int i = 0; i < numberOfEnemyHomingBullets; i++)
					{
						if (!enemyHomingBullets[i].inStorage)
						{
							Rectangle homingBulletMainMass
							{
								enemyHomingBullets[i].posX + 11,
								enemyHomingBullets[i].posY + 1,
								10,
								enemyHomingBullets[i].height - 2
							};
							Rectangle homingBulletSecondaryMass
							{
								enemyHomingBullets[i].posX + 3,
								enemyHomingBullets[i].posY + 4,
								8,
								5
							};
							if ((CheckCollisionRecs(homingBulletMainMass, playerLengthRec) or CheckCollisionRecs(homingBulletMainMass, playerBackWingRec) or CheckCollisionRecs(homingBulletMainMass, playerFrontWingRec)) or (CheckCollisionRecs(homingBulletSecondaryMass, playerLengthRec) or CheckCollisionRecs(homingBulletSecondaryMass, playerBackWingRec) or CheckCollisionRecs(homingBulletSecondaryMass, playerFrontWingRec)))
							{
								player.hp -= 1;
								enemyHomingBullets[i].inStorage = true;
								enemyHomingBullets[i].isHoming = false;
								enemyHomingBullets[i].posX = 700;
								enemyHomingBullets[i].posY = 700;
							}
						}
					}

					for (int i = 0; i < numberOFEnemyShips; i++)
					{
						if (!enemyShips[i].isDead)
						{
							//enemyShip box colliders
							Rectangle enemyShipFrontWingRec{
								enemyShips[i].posX + 32,
								enemyShips[i].posY + 6,
								4,
								enemyShips[i].height - 6
							};									////values need to be changed
							Rectangle enemyShipBackWingRec{
								enemyShips[i].posX + 14,
								enemyShips[i].posY + 1,
								9,
								18
							};
							Rectangle enemyShipLengthRec{
								enemyShips[i].posX,
								enemyShips[i].posY + 9,
								enemyShips[i].width,
								10
							};



							if ((CheckCollisionRecs(enemyShipFrontWingRec, playerLengthRec) or CheckCollisionRecs(enemyShipFrontWingRec, playerBackWingRec) or CheckCollisionRecs(enemyShipFrontWingRec, playerFrontWingRec)) or (CheckCollisionRecs(enemyShipBackWingRec, playerLengthRec) or CheckCollisionRecs(enemyShipBackWingRec, playerBackWingRec) or CheckCollisionRecs(enemyShipBackWingRec, playerFrontWingRec)) or (CheckCollisionRecs(enemyShipLengthRec, playerLengthRec) or CheckCollisionRecs(enemyShipLengthRec, playerBackWingRec) or CheckCollisionRecs(enemyShipLengthRec, playerFrontWingRec)))
							{
								player.hp -= 1;
								//enemyShips[i].currentSprite = 6 * bugs[i].enemyShips;
								enemyShips[i].hp = 0;
								enemyShips[i].Xvelocity = 0;
								enemyShips[i].frame = 0;
								enemyShips[i].isDead = true;
							}
							else
							{
								for (int e = 0; e < numberOfPlayerBullets; e++)
								{
									Rectangle playerBulletRec
									{
										playerBullets[e].posX,
										playerBullets[e].posY,
										playerBullets[e].width,
										playerBullets[e].height
									};
									if ((CheckCollisionRecs(playerBulletRec, enemyShipLengthRec)) or (CheckCollisionRecs(playerBulletRec, enemyShipBackWingRec)) or (CheckCollisionRecs(playerBulletRec, enemyShipFrontWingRec)))
									{
										playerBullets[e].inStorage = true;
										playerBullets[e].posX = 700;
										playerBullets[e].posY = 700;
										playerBullets[e].Xvelocity = 0;
										enemyShips[i].isDead = true;

										enemyShips[i].currentSprite = 6 * bugs[i].width;       //update for death animation
										enemyShips[i].Xvelocity = 0;
										enemyShips[i].frame = 0;
									}

								}
							}
						}
					}

					for (int i = 0; i < numberOfEnemyHomingShips; i++)
					{

						if (!enemyHomingShips[i].isDead)
						{
							//enemyShip box colliders
							Rectangle enemyHomingShipWingRec
							{	///
								enemyHomingShips[i].posX + 36,
								enemyHomingShips[i].posY + 10,
								9,
								18
							};									////values need to be changed
							//Rectangle enemyHomingShipThingRec
							//(
							//	enemyHomingShips[i].posX + 32,
							//	enemyHomingShips[i].posY + 6,
							//	4,
							//	enemyHomingShips[i].height - 6,
							//	GREEN
							//);
							Rectangle homingShipMainHeightRec
							{			///
								enemyHomingShips[i].posX + 22,
								enemyHomingShips[i].posY,
								16,
								21
							};
							Rectangle HomingShipLengthRec
							{	///
								enemyHomingShips[i].posX + 1,
								enemyHomingShips[i].posY + 11,
								46,
								10
							};



							if ((CheckCollisionRecs(enemyHomingShipWingRec, playerLengthRec) or CheckCollisionRecs(enemyHomingShipWingRec, playerBackWingRec) or CheckCollisionRecs(enemyHomingShipWingRec, playerFrontWingRec)) or (CheckCollisionRecs(homingShipMainHeightRec, playerLengthRec) or CheckCollisionRecs(homingShipMainHeightRec, playerBackWingRec) or CheckCollisionRecs(homingShipMainHeightRec, playerFrontWingRec)) or (CheckCollisionRecs(HomingShipLengthRec, playerLengthRec) or CheckCollisionRecs(HomingShipLengthRec, playerBackWingRec) or CheckCollisionRecs(HomingShipLengthRec, playerFrontWingRec)))
							{
								player.hp -= 1;
								//enemyShips[i].currentSprite = 6 * bugs[i].enemyShips;
								enemyHomingShips[i].Xvelocity = 0;
								enemyHomingShips[i].frame = 0;
								enemyHomingShips[i].isDead = true;
							}
							else
							{
								for (int e = 0; e < numberOfPlayerBullets; e++)
								{
									Rectangle playerBulletRec
									{
										playerBullets[e].posX,
										playerBullets[e].posY,
										playerBullets[e].width,
										playerBullets[e].height
									};
									if ((CheckCollisionRecs(playerBulletRec, HomingShipLengthRec)) or (CheckCollisionRecs(playerBulletRec, homingShipMainHeightRec)) or (CheckCollisionRecs(playerBulletRec, enemyHomingShipWingRec)))
									{
										playerBullets[e].inStorage = true;
										playerBullets[e].posX = 700;
										playerBullets[e].posY = 700;
										playerBullets[e].Xvelocity = 0;
										enemyHomingShips[i].Xvelocity = 0;
										enemyHomingShips[i].frame = 0;
										enemyHomingShips[i].isDead = true;
									}

								}
							}
						}
					}
					for (int i = 0; i < numberOfEnemyBullets; i++)
					{
						if (!enemyBullets[i].inStorage)
						{
							Rectangle enemyBulletRec
							{
							   (enemyBullets[i].posX + 1),
							   (enemyBullets[i].posY + 1),
							   (enemyBullets[i].width - 2),
							   (enemyBullets[i].height - 2)
							};

							if ((CheckCollisionRecs(enemyBulletRec, playerFrontWingRec)) or (CheckCollisionRecs(enemyBulletRec, playerBackWingRec)) or (CheckCollisionRecs(enemyBulletRec, playerLengthRec)))
							{
								enemyBullets[i].inStorage = true;
								enemyBullets[i].posX = 700;
								enemyBullets[i].posY = 700;
								enemyBullets[i].Xvelocity = 0;
								player.hp -= 1;
							}
						}
					}




					player.posY -= player.Yvelocity * dT;
					player.posX -= player.Xvelocity * dT;


					player.runningTime += dT;
					for (int i = 0; i < numberOfBugs; i++)
					{
						bugs[i].runningTime += dT;
					}
					for (int i = 0; i < numberOFEnemyShips; i++)
					{
						enemyShips[i].runningTime += dT;
					}
					for (int i = 0; i < numberOfEnemyHomingShips; i++)
					{
						enemyHomingShips[i].runningTime += dT;
					}
					for (int i = 0; i < numberOfEnemyHomingBullets; i++)
					{
						enemyHomingBullets[i].runningTime += dT;
					}
					if (player.runningTime >= player.updateTime)
					{
						player.runningTime = 0;
						//updates the sprite
						player.currentSprite = player.frame * player.width;
						player.frame += player.frameUpdater;
						//reset frame and set sprite to middle animation to loop
						if (player.frame > 2 and (player.frameUpdater == 1))
						{
							player.currentSprite = 0 * player.width;
							player.frameUpdater = -1;
						}
						else if ((player.frame == 0) and (player.frameUpdater == -1)) {
							player.frameUpdater = 1;
							player.frame = 0;
						}
					}

					for (int i = 0; i < numberOfBugs; i++)
					{
						if (bugs[i].runningTime > bugs[i].updateTime)
						{
							bugs[i].runningTime = 0;
							if (bugs[i].isDead) {
								if (bugs[i].currentSprite >= 9 * (enemyShips[i].width)) {
									bugs[i].posX = 600;
									bugs[i].posX = 700;
									bugs[i].isCompletedDeath = true;
								}
								else if (bugs[i].currentSprite < 5 * (bugs[i].width)) {
									bugs[i].currentSprite = 5 * (bugs[i].width);

								}
								else {
									bugs[i].currentSprite += (bugs[i].width);
								}

							}
							else{
								//updates the sprite
								bugs[i].currentSprite = bugs[i].frame * bugs[i].width;
								bugs[i].frame += bugs[i].frameUpdater;
								if ((bugs[i].frame == 5) and (bugs[i].frameUpdater == 1))
								{
									bugs[i].frameUpdater = -1;
								}
								else if ((bugs[i].frame == 0) and (bugs[i].frameUpdater == -1))
								{
									bugs[i].frameUpdater = 1;
								}
							}
						}
					}

					//enemy ship animations + shooting
					for (int i = 0; i < numberOFEnemyShips; i++)
					{
						if (enemyShips[i].runningTime > enemyShips[i].updateTime)
						{
							enemyShips[i].runningTime = 0;							
							if (enemyShips[i].isDead) {
								if (enemyShips[i].currentSprite >=  21 * (enemyShips[i].width)) {
									enemyShips[i].posX = 600;
									enemyShips[i].posX = 700;
									enemyShips[i].isCompletedDeath = true;
								}
								else if (enemyShips[i].currentSprite < 5 * (enemyShips[i].width)) {
									enemyShips[i].currentSprite = 5 * (enemyShips[i].width);

								}
								else {
									enemyShips[i].currentSprite += (enemyShips[i].width);
								}

							}
							else {
								//updates the sprite
								enemyShips[i].currentSprite = enemyShips[i].frame * enemyShips[i].width;
								enemyShips[i].frame += enemyShips[i].frameUpdater;
								enemyShips[i].frameCounter++;
								if ((enemyShips[i].posX < (window.width + 10)) and (enemyShips[i].frameCounter % enemyShips[i].shootFrame) == 0)
								{
									if (numberOfActiveEnemyBullets == numberOfEnemyBullets)
									{
										numberOfActiveEnemyBullets = 0;
									}
									else
									{
										enemyBullets[numberOfActiveEnemyBullets].posX = enemyShips[i].posX;
										enemyBullets[numberOfActiveEnemyBullets].posY = enemyShips[i].posY + (enemyShips[i].height / 2);
										enemyBullets[numberOfActiveEnemyBullets].Xvelocity = enemyBullets[numberOfActiveEnemyBullets].baseVelocity;
										enemyBullets[numberOfActiveEnemyBullets].inStorage = false;
										enemyShips[i].frameCounter = 0;
										numberOfActiveEnemyBullets++;
									}
								}
								if ((enemyShips[i].frame == 4) and (enemyShips[i].frameUpdater == 1))
								{
									enemyShips[i].frameUpdater = -1;
								}
								else if ((enemyShips[i].frame == 0) and (enemyShips[i].frameUpdater == -1))
								{
									enemyShips[i].frameUpdater = 1;
								}
							}
						}
					}

					//enemy homing ship animations + shooting
					for (int i = 0; i < numberOfEnemyHomingShips; i++)
					{
						if (enemyHomingShips[i].runningTime > enemyHomingShips[i].updateTime)
						{
							enemyHomingShips[i].runningTime = 0;
							if (enemyHomingShips[i].isDead) {
								if (enemyHomingShips[i].currentSprite >= 18 * (enemyHomingShips[i].width)) {
									enemyHomingShips[i].posX = 600;
									enemyHomingShips[i].posX = 700;
									enemyHomingShips[i].isCompletedDeath = true;
								}
								else if (enemyHomingShips[i].currentSprite < 5 * (enemyHomingShips[i].width)) {
									enemyHomingShips[i].currentSprite = 5 * (enemyHomingShips[i].width);

								}
								else {
									enemyHomingShips[i].currentSprite += (enemyHomingShips[i].width);
								}

							}
							else {			
								////////////////////////
								//updates the sprite
								enemyHomingShips[i].currentSprite = enemyHomingShips[i].frame * enemyHomingShips[i].width;
								enemyHomingShips[i].frame += enemyHomingShips[i].frameUpdater;
								enemyHomingShips[i].frameCounter++;
								if ((enemyHomingShips[i].posX < (window.width + 10)) and (enemyHomingShips[i].frameCounter % enemyHomingShips[i].shootFrame) == 0)
								{
									if (numberOfActiveEnemyHomingBullets == numberOfEnemyHomingBullets)
									{
										numberOfActiveEnemyHomingBullets = 0;
									}
									
									else
									{
										enemyHomingBullets[numberOfActiveEnemyHomingBullets].posX = enemyHomingShips[i].posX;
										enemyHomingBullets[numberOfActiveEnemyHomingBullets].posY = enemyHomingShips[i].posY + (enemyHomingShips[i].height / 2);
										enemyHomingBullets[numberOfActiveEnemyHomingBullets].Xvelocity = enemyHomingBullets[numberOfActiveEnemyHomingBullets].baseVelocity;
										enemyHomingBullets[numberOfActiveEnemyHomingBullets].inStorage = false;
										enemyHomingShips[i].frameCounter = 0;
										numberOfActiveEnemyHomingBullets++;
									}
								}
								
								if ((enemyHomingShips[i].frame == 4) and (enemyHomingShips[i].frameUpdater == 1))
								{
									enemyHomingShips[i].frameUpdater = -1;
								}
								
								else if ((enemyHomingShips[i].frame == 0) and (enemyHomingShips[i].frameUpdater == -1))
								{
									enemyHomingShips[i].frameUpdater = 1;
								}
							}
						}
					}



					for (int i = 0; i < numberOfEnemyHomingBullets; i++)
					{
						if (enemyHomingBullets[i].runningTime > enemyHomingBullets[i].updateTime)
						{
							enemyHomingBullets[i].runningTime = 0;
							//if (enemyHomingBullets[i].currentSprite >= (6 * enemyHomingBullets[i].width) and enemyHomingBullets[i].frame > 0)
							//{

							//	enemyHomingBullets[i].currentSprite = (enemyHomingBullets[i].frame * enemyHomingBullets[i].width) + (6 * 33);
							//	if (bugs[i].frame > 3)
							//	{
							//		bugs[i].posX = 600;
							//		bugs[i].posY = 600;
							//		bugs[i].isCompletedDeath = true;
							//	}
							//	bugs[i].frame++;
							//}
							
							//else if (bugs[i].isDead)
							//{
							//	bugs[i].Xvelocity = 0;
							//	bugs[i].currentSprite = (bugs[i].frame * bugs[i].width) + (6 * 33);
							//	bugs[i].frame++;
							//}
							//above is for a death if i wanna add an exposion on impact i need to add else and change variables

							if (!enemyHomingBullets[i].inStorage)
							{
								//updates the sprite
								enemyHomingBullets[i].currentSprite = enemyHomingBullets[i].frame * enemyHomingBullets[i].width;
								enemyHomingBullets[i].frame += enemyHomingBullets[i].frameUpdater;
								if ((enemyHomingBullets[i].frame == 6) and (enemyHomingBullets[i].frameUpdater == 1))
								{
									enemyHomingBullets[i].frameUpdater = -1;
								}
								
								else if ((enemyHomingBullets[i].frame == 0) and (enemyHomingBullets[i].frameUpdater == -1))
								{
									enemyHomingBullets[i].frameUpdater = 1;
								}
							}
						}
					}

					BGposX -= 40 * dT;
					if (BGposX <= -(1500))
					{
						BGposX = 0;
					}

					//draw background first
					DrawTextureEx(bg, { BGposX,0 }, 0, 145 / 100, WHITE);
					DrawTextureEx(bg, { (BGposX + (bg.width * (145 / 100))),0 }, 0, (145 / 100), WHITE);

					for (int i = 0; i < numberOfBugs; i++)
					{
						if (bugs[i].hp == 4)
						{
							DrawTextureRec(defaultEnemySpritePink, (Rectangle{ bugs[i].currentSprite, 0, bugs[i].width, bugs[i].height }), (Vector2{ bugs[i].posX,bugs[i].posY }), WHITE);
						}
						
						else if (bugs[i].hp == 3)
						{
							DrawTextureRec(defaultEnemySpriteYellow, (Rectangle{ bugs[i].currentSprite, 0, bugs[i].width, bugs[i].height }), (Vector2{ bugs[i].posX,bugs[i].posY }), WHITE);
						}
						
						else if (bugs[i].hp == 2)
						{
							DrawTextureRec(defaultEnemySpriteBlue, (Rectangle{ bugs[i].currentSprite, 0, bugs[i].width, bugs[i].height }), (Vector2{ bugs[i].posX,bugs[i].posY }), WHITE);
						}
						
						else if (bugs[i].hp == 1)
						{
							DrawTextureRec(defaultEnemySpriteGreen, (Rectangle{ bugs[i].currentSprite, 0, bugs[i].width, bugs[i].height }), (Vector2{ bugs[i].posX,bugs[i].posY }), WHITE);
						}
					}
					
					for (int i = 0; i < numberOFEnemyShips; i++)
					{
						DrawTextureRec(enemyShipSprite, (Rectangle{ enemyShips[i].currentSprite, 0, enemyShips[i].width, enemyShips[i].height }), (Vector2{ enemyShips[i].posX,enemyShips[i].posY }), WHITE);
					}

					for (int i = 0; i < numberOfEnemyHomingBullets; i++)
					{
						DrawTextureRec(enemyHomingShipSprite, (Rectangle{ enemyHomingShips[i].currentSprite, 0, enemyHomingShips[i].width, enemyHomingShips[i].height }), (Vector2{ enemyHomingShips[i].posX,enemyHomingShips[i].posY }), WHITE);
					}

					for (int i = 0; i < numberOfPlayerBullets; i++)
					{
						DrawTextureRec(playerBulletTexture, (Rectangle{ playerBullets[i].currentSprite, 0, playerBullets[i].width, playerBullets[i].height }), (Vector2{ playerBullets[i].posX,playerBullets[i].posY }), WHITE);
					}
					
					for (int i = 0; i < numberOfEnemyBullets; i++)
					{
						DrawTextureRec(enemyBulletTexture, (Rectangle{ enemyBullets[i].currentSprite, 0, enemyBullets[i].width, enemyBullets[i].height }), (Vector2{ enemyBullets[i].posX,enemyBullets[i].posY }), WHITE);
					}
					
					for (int i = 0; i < numberOfEnemyHomingBullets; i++)
					{
						DrawTextureRec(enemyHomingBulletTexture, (Rectangle{ enemyHomingBullets[i].currentSprite, 0, enemyHomingBullets[i].width, enemyHomingBullets[i].height }), (Vector2{ enemyHomingBullets[i].posX,enemyHomingBullets[i].posY }), WHITE);
					}

					DrawTextureRec(sprite, (Rectangle{ player.currentSprite, 0, player.width, player.height }), (Vector2{ player.posX, player.posY }), WHITE);
					DrawCircle(player.posX, player.posY, 1, PINK);


					//ui stuff (needs to be above shiiiiitttt
					for (int i = 0; i < numberOfBugs; i++)
					{
						if (bugs[i].isDead)
						{
							if (!bugs[i].isScored)
							{
								score += 50;
								bugs[i].isScored = true;
							}
							//DrawText(text, x, y, fontSize, Color)
							DrawText("50", (bugs[i].posX - 10), (bugs[i].posY - 5), 17, YELLOW);
						}
					}

					for (int i = 0; i < numberOFEnemyShips; i++)
					{

						if (enemyShips[i].isDead)
						{
							if (!enemyShips[i].isScored)
							{
								score += 100;
								enemyShips[i].isScored = true;
							}
							//DrawText(text, x, y, fontSize, Color)
							//DrawText("50", (bugs[i].posX - 10), (bugs[i].posY - 5), 17, YELLOW);

							DrawText("100", (enemyShips[i].posX - 10), (enemyShips[i].posY - 5), 17, YELLOW);
						}
					}

					for (int i = 0; i < numberOfEnemyHomingShips; i++)
					{
						if (enemyHomingShips[i].isDead)
						{
							if (!enemyHomingShips[i].isScored)
							{
								score += 150;
								enemyHomingShips[i].isScored = true;
							}
							//DrawText(text, x, y, fontSize, Color)
							DrawText("150", (enemyHomingShips[i].posX - 10), (enemyHomingShips[i].posY - 5), 17, YELLOW);
						}
					}

					for (int i = 0; i < player.hp; i++)
					{
						float displacement = (12 + 27 * i);
						DrawTextureRec(playerHeart, (Rectangle{ 0, 0, 21, 21 }), (Vector2{ displacement, 12 }), WHITE);
					}

					char textScore[6] = "score";
					DrawText(TextFormat("Score: %08i", score), 300, 5, 26, RED);

					EndDrawing();

					if (player.hp <= 0)
					{
						startGame = false;
						break;
					}
				}
				//while game
			}
			//DrawTextureEx(lossBg, { 0,0 }, 0, (145 / 100), WHITE);
		}
		else
		{	
			BeginDrawing();
			DrawText("You Died", 256, 190, 20, WHITE);
			DrawTextureRec(dMenuOptions, (Rectangle{ dMenuOptionSelected, 0, 189, 127 }), (Vector2{ 303, 233 }), WHITE);
			if ((IsKeyPressed(KEY_DOWN)) or (IsKeyPressed(KEY_S)))
			{
				dMenuOptionSelected += dMenuOptionWidth;
			}
			if ((IsKeyPressed(KEY_UP)) or (IsKeyPressed(KEY_W)))
			{
				dMenuOptionSelected -= dMenuOptionWidth;
			}
			
			if (dMenuOptionSelected >= dMenuOptionWidth * 4) 
			{
				dMenuOptionSelected = dMenuOptionWidth;
			}
			
			else if (dMenuOptionSelected < 0) 
			{
				dMenuOptionSelected = dMenuOptionWidth * 3;
			}
			
			if ((IsKeyPressed(KEY_SPACE)) or (IsKeyPressed(KEY_ENTER)))
			{
				int optionSelected = (dMenuOptionSelected / 189) + 1;

				if (optionSelected == 1)
				{
					startGame = true;
				}
				
				else if (optionSelected == 2)
				{
					mainMenu = true;
				}
				
				else if (optionSelected == 3)
				{
					break;
				}
			}
			EndDrawing();
		}
	}//menu
	
	UnloadTexture(sprite);
	UnloadTexture(defaultEnemySpriteGreen);
	UnloadTexture(defaultEnemySpriteBlue);
	UnloadTexture(defaultEnemySpritePink);
	UnloadTexture(defaultEnemySpriteYellow);
	UnloadTexture(enemyShipSprite);
	UnloadTexture(bg);
	UnloadTexture(playerBulletTexture);
	UnloadTexture(enemyBulletTexture);
	UnloadTexture(playerHeart);
	UnloadTexture(lossBg);
	UnloadTexture(sMenuBg);
	UnloadTexture(dMenuOptions);
	UnloadTexture(sMenuOptions);
	CloseWindow();//use this to properly close the window
}