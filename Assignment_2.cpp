// Assignment_2.cpp: A program using the TL-Engine

#include "TL-Engine11.h" // TL-Engine11 include file and namespace
#include <format>
using namespace tle;

//structure for vectors and poinsts in 3d

struct Vector3D
{
	float x;
	float y;
	float z;
};

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

bool sphereTOBoxCollision(Vector2d Vmin, Vector2d Vmax, float radius, Vector2d currPos)
{
	return (
		currPos.x + radius > Vmin.x && currPos.x - radius < Vmax.x&& currPos.z + radius > Vmin.z && currPos.z - radius < Vmax.z
	);
};

float distanceSquired(Vector2d v1, Vector2d v2) 
{
	return (
			(v1.x - v2.x) + (v1.x - v2.x) + (v1.z - v2.z) + (v1.z - v2.z)
		);
};

bool SphereToSphereCllision(Vector2d sphereCentral, Vector2d sphereCentral2, float sphereRadius, float sphereRadius2)
{
	return (
		distanceSquired(sphereCentral, sphereCentral2) <= (sphereRadius + sphereRadius2) * (sphereRadius + sphereRadius2)
		);
};

int main()
{
	// Create a 3D engine (using TL11 engine here) and open a window for it
	TLEngine* myEngine = New3DEngine( kTLX );
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder( "C:\\Users\\Public\\Documents\\TL-Engine11\\Media" );
	myEngine->AddMediaFolder("Media");

	/**** Set up your scene here ****/
	
	//position of SkyBox. car
	const Vector3D skyBoxCoor = { 0.0f, -960.0f, 0.0f };
	const Vector3D CarCoor = { 0.0f, 0.0f, -10.0f };

	//Mesh and Model for SkyBox
	IMesh* skyBoxMesh = myEngine->LoadMesh("Skybox 07.x");
	IModel* skyBoxModel = skyBoxMesh->CreateModel(skyBoxCoor.x, skyBoxCoor.y, skyBoxCoor.z);

	//Mesh and Model for Car
	IMesh* CarMesh = myEngine->LoadMesh("Race2.x");
	IModel* CarModel = CarMesh->CreateModel(CarCoor.x, CarCoor.y, CarCoor.z);
	const float carScale = 0.5f;
	const float carRadius = 5.0f * carScale;
	CarModel->Scale(carRadius);

	//Mesh and model for floor
	IMesh* floorMesh = myEngine->LoadMesh("ground.x");
	IModel* floorModel = floorMesh->CreateModel();

	//camera initialization
	const Vector3D myCamCoor = { 0.0f, 20.0f, -50.0f };
	const EKeyCode camUp = Key_Up;
	const EKeyCode camDown = Key_Down;
	const EKeyCode camLeft = Key_Left;
	const EKeyCode camRight = Key_Right;

	ICamera* myCam = myEngine->CreateCamera(kManual, myCamCoor.x, myCamCoor.y, myCamCoor.z);
	myCam->AttachToParent(CarModel);

	//Mesh and Model for Chekpoints
	IMesh* checkpointMesh = myEngine->LoadMesh("checkpoint.x");
	const int CheckpointSize = 3;
	IModel* Checkpoints[CheckpointSize];

	const float checkpointsHalfLine = 7.29081f;
	const float checkpointsStructRadius = 1.28539f;

	//Array of checkpoints
	const Vector3D checkpontsPos[CheckpointSize] = {
		{0.0f, 0.0f, 0.0f},
		{10.0f, 0.0f, 120.0f},
		{25.0f, 0.0f, 56.0f}
	};

	//looping through the checkpoints
	for (int i = 0; i < CheckpointSize; i++)
	{
		Checkpoints[i] = checkpointMesh->CreateModel(checkpontsPos[i].x, checkpontsPos[i].y, checkpontsPos[i].z);
	};
	Checkpoints[1]->RotateY(90.0f);

	Vector2d checkPointsStructsCentrs[CheckpointSize * 2] = {
		{checkpontsPos[0].x - checkpointsHalfLine - checkpointsStructRadius,checkpontsPos[0].z},
		{checkpontsPos[0].x - checkpointsHalfLine + checkpointsStructRadius,checkpontsPos[0].z},
		{checkpontsPos[1].x - checkpontsPos[1].z -	checkpointsHalfLine - checkpointsStructRadius},
		{checkpontsPos[1].x - checkpontsPos[1].z +	checkpointsHalfLine + checkpointsStructRadius},
		{checkpontsPos[2].x - checkpointsHalfLine - checkpointsStructRadius,checkpontsPos[2].z},
		{checkpontsPos[2].x - checkpointsHalfLine + checkpointsStructRadius,checkpontsPos[2].z},
	};

	//mesh and array of models for iles
	IMesh* isleMesh = myEngine->LoadMesh("IsleStraight.x");
	const int IsleSize = 6;
	const float isleLength = 2.67529f;
	const float isleWidth = 3.41748f;

	IModel* isles[IsleSize];

	//Array of isle's position
	const Vector3D IslePos[IsleSize] = {
	{-10.0f, 0.0f, 40.0f},
	{10.0f, 0.0f, 40.0f},
	{-10.0f, 0.0f, 56.0f},
	{10.0f, 0.0f, 56.0f},
	{-10.0f, 0.0f, 72.0f},
	{10.0f, 0.0f, 72.0f},
	};

	//looping through the isle's
	for (int i = 0; i < IsleSize; i++)
	{
		isles[i] = isleMesh->CreateModel(IslePos[i].x, IslePos[i].y, IslePos[i].z);
	};

	struct Box {
		Vector2d Vmin;
		Vector3D Vmax;
	};

	const int numBoxs = 2;
	Box boxes[numBoxs];

	boxes[0] = { {isles[0]->GetX() - isleLength, isles[0]->GetZ() - isleWidth }, {isles[4]->GetX() + isleLength, isles[4]->GetZ() + isleWidth }, };
	boxes[1] = { {isles[1]->GetX() - isleLength, isles[1]->GetZ() - isleWidth }, {isles[5]->GetX() + isleLength, isles[5]->GetZ() + isleWidth }, };


	//mesh and array of models for iles
	IMesh* WallMesh = myEngine->LoadMesh("Wall.x");
	const int WallSize = 4;
	IModel* Wall[WallSize];

	//Array of isle's position
	const Vector3D WallPos[WallSize] = {
	{-10.0f, 0.0f, 40.0f},
	{10.0f, 0.0f, 40.0f},
	{-10.0f, 0.0f, 64.0f},
	{10.0f, 0.0f, 64.0f},
	};

	//looping through the isle's
	for (int i = 0; i < WallSize; i++)
	{
		Wall[i] = WallMesh->CreateModel(WallPos[i].x, WallPos[i].y, WallPos[i].z);
	};

	//constants and variables for speed and time
	float frameTime = 0.0f;
	const float kGameSpeed = 20.0f;
	float CarSpeed = frameTime * kGameSpeed;
	float KRotAngle = 20.0f;

	const float thrustFractor = 5.f;
	const float coeffDrag = -0.2f;

	float elaspedTime = 0.0f;
	float countDownTime = 3.0f;

	//vectors for momentum, thurst,  drug and time
	Vector2d momentum = { 0.0f, 0.0f };
	Vector2d thurst = { 0.0f, 0.0f };
	Vector2d drag = { 0.0f, 0.0f };
	Vector2d facingVector = { 0.0f, 1.0f };
	float matrix[4][4]; //place to store the model matrix

	const EKeyCode fwdThurstKey = Key_W;
	const EKeyCode revThurstKey = Key_S;

	//Camera control constants
	const float KCameraMove = kGameSpeed * 0.001f;
	const float KMouseWheelMove = kGameSpeed * 0.01f;
	const float KMouseRotation = kGameSpeed * 0.003f;
	bool mouseCaptureActive = false;
	const EKeyCode toogleMouseCapture = Key_T;
	const EKeyCode toogleChaseCam = Key_1;

	myEngine->StartMouseCapture();
	bool ChaseCam = true;
	int mouseMoveX = 0;
	int mouseMoveY = 0;

	//States
	enum GameStates {Intro, CountDown, Race, GameOver};
	enum RaceStates {Start, CP1Cross, CP2Cross, CP3Cross}; //CP1 - first checkpoint

	GameStates gameState = Race; //initial state
	RaceStates raceState = Start;

	//Fonts and spites
	IFont* myFontCountGo = myEngine->LoadFont("Times New Roman", 80);
	IFont* myFontTimer = myEngine->LoadFont("Arial", 30);
	IFont* myFontRaceState = myEngine->LoadFont("Arial", 30);
	Sprite* mySpite = myEngine->CreateSprite("ui_backdrop.jpg", 0.0f, myEngine->GetHeight() - 80.0f);

	Vector2d oldPos = { CarModel->GetX(), CarModel->GetZ() };

	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();
		frameTime = myEngine->Timer();

		if (gameState == Intro)
		{
			myFontCountGo->Draw("Press Space to Go", myEngine->GetWidth() / 3, myEngine->GetHeight() / 3, kRed, kCentre);
			if (myEngine->KeyHit(Key_Space)) 
			{
				gameState = CountDown;
			};
		};

		if (gameState == CountDown)
		{
			countDownTime -= frameTime;
			if (countDownTime > 2)
			{
				myFontCountGo->Draw("3", 800, 300, kRed, CentreAlign);
			}
			else if (countDownTime > 1)
			{
				myFontCountGo->Draw("2", 800, 300, kRed, CentreAlign);
			}
			else if (countDownTime > 0)
			{
				myFontCountGo->Draw("1", 800, 300, kRed, CentreAlign);
			}
			else if (countDownTime > -0.5f)
			{
				myFontCountGo->Draw("G0", 800, 300, kRed, CentreAlign);
			}
			else
			{
				gameState = Race;
				raceState = Start;
			};
		};

		// Stop if the Escape key is pressed
		if (myEngine->KeyHit( Key_Escape ))
		{
			myEngine->Stop();
		}
	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}
