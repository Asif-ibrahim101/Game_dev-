// Assignment2.cpp: A program using the TL-Engine

#include "TL-Engine11.h" // TL-Engine11 include file and namespace
using namespace tle;

int main()
{
	// Create a 3D engine (using TL11 engine here) and open a window for it
	TLEngine* myEngine = New3DEngine( kTLX );
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder( "C:\\Users\\Public\\Documents\\TL-Engine11\\Media" );
	myEngine->AddMediaFolder("Media");

	/**** Set up your scene here ****/
	float frameTime = 0.0f;
	float kGameSpeed = 10.0f;

	//skybox
	const float  SkyBoxX = 0.0f;
	const float  SkyBoxY = -960.0f;
	const float  SkyBoxZ = 0.0f;

	IMesh* skyBoxMesh = myEngine->LoadMesh("Skybox 07.x");
	IModel* skyBox = skyBoxMesh->CreateModel(SkyBoxX, SkyBoxY, SkyBoxZ);

	//checkpoints
	const float CheckPointRotate = 90.0f;

	//for checkpoint 1
	const float CheckPoint1X = 10.0f;
	const float CheckPoint1Y = 0.0f;
	const float CheckPoint1Z = 120.0f;

	//for checkpoint 2
	const float CheckPoint2X = 25.0f;
	const float CheckPoint2Y = 0.0f;
	const float CheckPoint2Z = 56.0f;

	IMesh* checkPointMesh = myEngine->LoadMesh("checkpoint.x");
	const int CheckPointSize = 3;
	IModel* CheckPoints[CheckPointSize];

	for (int i = 0; i < CheckPointSize; i++)
	{
		CheckPoints[i] = checkPointMesh->CreateModel();

	};
	CheckPoints[1]->SetPosition(CheckPoint1X, CheckPoint1Y, CheckPoint1Z);
	CheckPoints[1]->RotateY(CheckPointRotate);
	CheckPoints[2]->SetPosition(CheckPoint2X, CheckPoint2Y, CheckPoint2Z);

	//car
	IMesh* CarMesh = myEngine->LoadMesh("Race2.x");
	IModel* Car = CarMesh->CreateModel(0 ,0 , -20.0f);
	float carSpeed = frameTime * kGameSpeed;

	//Floor
	IMesh* GroundMesh = myEngine->LoadMesh("ground.x");
	IModel* ground = GroundMesh->CreateModel();

	//camera
	ICamera* camera;
	camera = myEngine->CreateCamera(kManual, 0.0f, 30.0f, -100.0f);

	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();

		/**** Update your scene each frame here ****/

		// Stop if the Escape key is pressed
		if (myEngine->KeyHit( Key_Escape ))
		{
			myEngine->Stop();
		}
	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}

		/*if (myEngine->AverageFPS() <= 0) {
			frameTime = kGameSpeed * frameTime;
		}
		else {
			frameTime = kGameSpeed / myEngine->AverageFPS();
		};
		carSpeed = frameTime * kGameSpeed;

		Car->MoveLocalZ(carSpeed);*/
		//frameTime = myEngine->Timer();