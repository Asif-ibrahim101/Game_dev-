// Assignment2.cpp: A program using the TL-Engine

#include "TL-Engine11.h" // TL-Engine11 include file and namespace
#include <string>
using namespace tle;

//float kGameSpeed = 1.0f; //All Movement is multiplied by this

//Keyboard Controls
//Car
const EKeyCode accelerate = Key_W;
const EKeyCode turnLeft = Key_A;
const EKeyCode decelerate = Key_S;
const EKeyCode turnRight = Key_D;
const float ThurstFactor = 0.000005f;
const float coeffitiantOfDrag = -0.001;

//Camera
const EKeyCode toggleChaseCam = Key_1;
const EKeyCode toggleMouseCapture = Key_Tab;
const EKeyCode camUp = Key_Up;
const EKeyCode camDown = Key_Down;
const EKeyCode camLeft = Key_Left;
const EKeyCode camRight = Key_Right;

//constants and variables
float frameTime = 0;
float elaspeedTime = 0.0f;
float countDownTime = 3.0f;

struct Vector2d
{
	float x;
	float z;
};

Vector2d scalar(float s, Vector2d v)
{
	return { s * v.x, s * v.z };
};

Vector2d addthere(Vector2d v1, Vector2d v2, Vector2d v3) {
	return { v1.x + v2.x + v3.x, v1.z + v2.z + v3.z };
};

bool CheckAndHandleCollisionIsle(Model* car, Model* isle, float isle_width, float isle_depth, float Car_radius);
bool CheckAndHandleCollisionWalls(Model* car, Model* Wall, float Wall_width, float Wall_depth, float Car_radius);


int main()
{
	// Create a 3D engine (using TL11 engine here) and open a window for it
	TLEngine* myEngine = New3DEngine(kTLX);
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder("C:\\Users\\Public\\Documents\\TL-Engine11\\Media");
	myEngine->AddMediaFolder("Media");

	/**** Set up your scene here ****/
	float frameRate = myEngine->FrameTime();

	//states
	enum GameStates { Start, Countdown, Ready };
	GameStates gameState = Start;

	//Fonts and spites
	IFont* myFont = myEngine->LoadFont("Times New Roman", 80);
	Sprite* mySpite = myEngine->CreateSprite("ui_backdrop.jpg", 0.0f, 800.0f);

	//Cameara Control
	const float KCameraMove = frameRate * 1;
	const float KMouseWheelMove = frameRate * 10.0f;
	const float KMouseRotation = frameRate * 0.3f;

	//skybox
	const float  SkyBoxX = 0.0f;
	const float  SkyBoxY = -960.0f;
	const float  SkyBoxZ = 0.0f;

	IMesh* skyBoxMesh = myEngine->LoadMesh("Skybox 07.x");
	IModel* skyBox = skyBoxMesh->CreateModel(SkyBoxX, SkyBoxY, SkyBoxZ);

	//car
	const float carX = 0.0f;
	const float carY = 1.0f;
	const float carZ = -20.0f;
	const float car_Radius = 5.0f;
	IMesh* CarMesh = myEngine->LoadMesh("Race2.x");
	IModel* Car = CarMesh->CreateModel(carX, carY, carZ);

	Vector2d momentum = { 0.0f, 0.0f };
	Vector2d thurst = { 0.0f, 0.0f };
	Vector2d drag = { 0.0f, 0.0f };
	Vector2d facingVector = { 0.0f, 1.0f };
	float matrix[4][4]; //place to store the model matrix

	//checkpoints
	const int CheckPointSize = 3;
	const float CheckPointRotate = 90.0f;
	IMesh* checkPointMesh = myEngine->LoadMesh("checkpoint.x");
	IModel* CheckPoints[CheckPointSize];

	const float CheckPountY[CheckPointSize] = { 0, 0, 0 };
	const float CheckPointX[CheckPointSize] = { 0, 10, 45 };
	const float CheckPointZ[CheckPointSize] = { 0, 120, 56 };

	for (int i = 0; i < CheckPointSize; i++)
	{
		CheckPoints[i] = checkPointMesh->CreateModel(CheckPointX[i], CheckPountY[i], CheckPointZ[i]);
		CheckPoints[i]->Scale(1.1);
	};
	CheckPoints[1]->RotateY(CheckPointRotate);

	//isle
	const float isle_depth = 2.0f;
	const float isle_width = 2.0f;

	const int isleSIze = 4;
	IModel* isle[isleSIze];
	IMesh* isleMesh = myEngine->LoadMesh("IsleStraight.x");

	const float IsleY = 0;
	const float IsleX[isleSIze] = { -10, 10, -10, 10, };
	const float IsleZ[isleSIze] = { 40, 40, 94, 94, };

	for (int i = 0; i < isleSIze; i++)
	{
		isle[i] = isleMesh->CreateModel(IsleX[i], IsleY, IsleZ[i]);
		isle[i]->SetSkin("Brickwall3_Texture.png");
		isle[i]->Scale(1.5);
	};

	//walls

	const float Wall_depth = 2.5f;
	const float Wall_width = 2.5f;

	const int WallSize = 24;
	IMesh* WallMesh = myEngine->LoadMesh("Wall.x");
	IModel* Wall[WallSize];

	/// for the walls
	const float radius_Wall = 2.0f;
	const float WallY = 0;
	const float WallX[WallSize] = { -10, 10, -10, 10,  -10, 10, -10, 10, -10, 10, -10, 10,  -10, 10, -10, 10,  -10, 10, -10, 10, -10, 10, -10, 10 };
	const float WallZ[WallSize] = { 48, 48, 64, 64, 50, 50, 70, 70, 52,52, 76, 76, 54, 54, 82, 82, 56, 56, 88,88, 58, 58, 94, 94 };

	for (int i = 0; i < WallSize; i++)
	{
		Wall[i] = WallMesh->CreateModel(WallX[i], WallY, WallZ[i]);
		Wall[i]->SetSkin("Brickwall2_Texture.png");
		Wall[i]->Scale(1.5);
	};

	//Floor
	IMesh* GroundMesh = myEngine->LoadMesh("ground.x");
	IModel* ground = GroundMesh->CreateModel();

	//mouse
	int mouseMoveX = 0;
	int mouseMoveY = 0;
	bool mouseCaptureActive = true; //state of mouse
	myEngine->StartMouseCapture();

	//camera
	bool ChaseCamActive = false; //state of camera 
	const float CameraX = 0.0f;
	const float CameraY = 10.0f;
	const float CameraZ = -25.0f;

	ICamera* camera;
	camera = myEngine->CreateCamera(kManual, CameraX, CameraY, CameraZ); //kManual, 0.0f, 30.0f, -100.0f
	camera->AttachToParent(Car);

	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();
		frameTime = myEngine->Timer();

		if (gameState == Start)
		{
			myFont->Draw("Press Space to Start", 800, 300, kRed, CentreAlign); //player to press start

			if (myEngine->KeyHit(Key_Space))
			{
				gameState = Countdown;
			}
		};

		if (gameState == Countdown)
		{
			countDownTime -= frameTime;
			if (countDownTime > 2)
			{
				myFont->Draw("3", 800, 300, kRed, CentreAlign);
			}
			else if (countDownTime > 1)
			{
				myFont->Draw("2", 800, 300, kRed, CentreAlign);
			}
			else if (countDownTime > 0)
			{
				myFont->Draw("1", 800, 300, kRed, CentreAlign);
			}
			else if (countDownTime > -0.5f)
			{
				myFont->Draw("G0", 800, 300, kRed, CentreAlign);
			}
			else
			{
				gameState = Ready;
			};
		};

		//Collision with isles
		bool collision_isle = false;
		for (int i = 0; i < isleSIze; i++)
		{
			if (CheckAndHandleCollisionIsle(Car, Wall[i], car_Radius, Wall_width, Wall_depth))
				collision_isle = true;
		};

		//resolving collision with isle
		if (collision_isle)
		{
			momentum.x *= -1;
			momentum.z *= -1;
		};

		//Collision with walls
		bool collision_Walls = false;
		for (int i = 0; i < isleSIze; i++)
		{
			if (CheckAndHandleCollisionIsle(Car, isle[i], car_Radius, isle_width, isle_depth))
				collision_Walls = true;
		};

		//resolving collision with Walls
		if (collision_Walls)
		{
			momentum.x *= -1;
			momentum.z *= -1;
		};

		//Get the facing vector form the model mattrix
		Car->GetMatrix(&matrix[0][0]);  //get the model mattrix
		facingVector = { matrix[2][0], matrix[2][2] }; //local z axis's x and z components

		//calculate thurst 
		if (myEngine->KeyHeld(accelerate))
		{
			thurst = scalar(ThurstFactor, facingVector);
		}
		else if (myEngine->KeyHeld(decelerate))
		{
			thurst = scalar(-ThurstFactor, facingVector);
		}
		else
		{
			thurst = { 0.0f, 0.0f };
		};
		if (myEngine->KeyHeld(turnLeft)) Car->RotateY(-0.01f);
		if (myEngine->KeyHeld(turnRight)) Car->RotateY(0.01f);

		//calculate drag
		drag = scalar(coeffitiantOfDrag, momentum);

		//calculate new momentum
		momentum = addthere(momentum, drag, thurst);

		//Move Car
		Car->Move(momentum.x, 0.0f, momentum.z);

		//Cameara Control
		/////////////////

		//keyboard controlled camera movement
		if (myEngine->KeyHeld(camUp)) camera->MoveLocalZ(KCameraMove);
		if (myEngine->KeyHeld(camDown)) camera->MoveLocalZ(-KCameraMove);
		if (myEngine->KeyHeld(camRight)) camera->MoveLocalX(KCameraMove);
		if (myEngine->KeyHeld(camLeft)) camera->MoveLocalX(-KCameraMove);

		////toggle chase cam mode
		if (myEngine->KeyHit(toggleChaseCam))
		{
			ChaseCamActive = !ChaseCamActive; //making it true
			if (ChaseCamActive)
			{
				//camera->AttachToParent(Car);
				camera->SetPosition(CameraX, CameraY, CameraZ);
			}
		};

		////mouse controlled camera movement
		if (mouseCaptureActive && !ChaseCamActive)
		{
			mouseMoveX += myEngine->GetMouseMovementX(); //maintain movemnet
			mouseMoveY += myEngine->GetMouseMovementY();

			camera->ResetOrientation(); //reset cameara before moving
			camera->RotateLocalY(mouseMoveX * KMouseRotation); //the mouse rotation reduces rotation speed
			camera->RotateLocalX(mouseMoveX * KMouseRotation);

			float mouseMoveWheel = myEngine->GetMouseWheelMovement();
			camera->MoveLocalZ(mouseMoveWheel * KMouseWheelMove);
		};

		//toggle mouse capture mode
		if (myEngine->KeyHit(toggleMouseCapture))
		{
			mouseCaptureActive = !mouseCaptureActive;
			if (mouseCaptureActive)
			{
				myEngine->StartMouseCapture();
				!mouseCaptureActive;
			}
			else
			{
				myEngine->StopMouseCapture();
			}
		};



		// Stop if the Escape key is pressed
		if (myEngine->KeyHit(Key_Escape))
		{
			myEngine->Stop();
		}
	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}


bool CheckAndHandleCollisionIsle(Model* car, Model* isle, float isle_width, float isle_depth, float Car_radius) {
	// Calculate collision bounds
	//sphere to box collision
	float minX = isle->GetX() - (isle_width / 2) - Car_radius;
	float maxX = isle->GetX() + (isle_width / 2) + Car_radius;
	float minZ = isle->GetZ() - (isle_depth / 2) - Car_radius;
	float maxZ = isle->GetZ() - (isle_depth / 2) + Car_radius;

	return (
		car->GetX() > minX && car->GetX() < maxX
		&& car->GetZ() > minZ && car->GetZ() < maxZ
		);
};


bool CheckAndHandleCollisionWalls(Model* car, Model* Wall, float Wall_width, float Wall_depth, float Car_radius) {
	// Calculate collision bounds
	//sphere to box collision
	float minX = Wall->GetX() - (Wall_width / 2) - Car_radius;
	float maxX = Wall->GetX() + (Wall_width / 2) + Car_radius;
	float minZ = Wall->GetZ() - (Wall_depth / 2) - Car_radius;
	float maxZ = Wall->GetZ() - (Wall_depth / 2) + Car_radius;

	return (
		car->GetX() > minX && car->GetX() < maxX
		&& car->GetZ() > minZ && car->GetZ() < maxZ
		);
};














































