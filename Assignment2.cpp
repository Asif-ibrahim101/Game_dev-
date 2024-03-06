// Assignment2.cpp: A program using the TL-Engine

#include "TL-Engine11.h" // TL-Engine11 include file and namespace
using namespace tle;

float kGameSpeed = 1.0f; //All Movement is multiplied by this

//Camera
const EKeyCode toggleChaseCam = Key_1;
const EKeyCode toggleMouseCapture = Key_Tab;
const EKeyCode camUp = Key_Up;
const EKeyCode camDown = Key_Down;
const EKeyCode camLeft = Key_Left;
const EKeyCode camRight = Key_Right;

//Cameara Control
const float KCameraMove = kGameSpeed * 0.01;
const float KMouseWheelMove = kGameSpeed * 10.0f;
const float KMouseRotation = kGameSpeed * 0.3f;

int main()
{
	// Create a 3D engine (using TL11 engine here) and open a window for it
	TLEngine* myEngine = New3DEngine(kTLX);
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder("C:\\Users\\Public\\Documents\\TL-Engine11\\Media");
	myEngine->AddMediaFolder("Media");

	/**** Set up your scene here ****/
	float frameTime = 0.0f;

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

	//isle
	const float isleX = 10.0f;
	const float isleY = 0.0f;
	const float isleZ1 = 40.0f;
	const float isleZ2 = 56.0f;
	const float isleZ3 = 72.0f;

	IMesh* isleMesh = myEngine->LoadMesh("IsleStraight.x");
	const int isleSIze = 7;
	IModel* isle[isleSIze];

	for (int i = 0; i < isleSIze; i++)
	{
		isle[i] = isleMesh->CreateModel(-isleX, isleY, isleZ1);
	};

	isle[1]->SetPosition(-isleX, isleY, isleZ1);
	isle[2]->SetPosition(isleX, isleY, isleZ1);
	isle[3]->SetPosition(-isleX, isleY, isleZ2);
	isle[4]->SetPosition(isleX, isleY, isleZ2);
	isle[5]->SetPosition(-isleX, isleY, isleZ3);
	isle[6]->SetPosition(isleX, isleY, isleZ3);

	//walls
	const float WallX = 10.0f;
	const float WallY = 0.0f;
	const float WallZ1 = 48.0f;
	const float WallZ2 = 64.0f;

	IMesh* WallMesh = myEngine->LoadMesh("Wall.x");
	const int WallSize = 5;
	IModel* Wall[WallSize];

	for (int i = 0; i < WallSize; i++)
	{
		Wall[i] = WallMesh->CreateModel(-WallX, WallY, WallZ1);
	};


	Wall[1]->SetPosition(-WallX, WallY, WallZ1);
	Wall[2]->SetPosition(WallX, WallY, WallZ1);
	Wall[3]->SetPosition(-WallX, WallY, WallZ2);
	Wall[4]->SetPosition(WallX, WallY, WallZ2);


	//car
	IMesh* CarMesh = myEngine->LoadMesh("Race2.x");
	IModel* Car = CarMesh->CreateModel(0, 0, -20.0f);
	float carSpeed = frameTime * kGameSpeed;

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
	const float CameraY = 15.0f;
	const float CameraZ = -55.0f;

	ICamera* camera;
	camera = myEngine->CreateCamera(kManual, CameraX, CameraY, CameraZ); //kManual, 0.0f, 30.0f, -100.0f

	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();

		//Cameara Control
		/////////////////
		
		//keyboard controlled camera movement
		if (myEngine->KeyHeld(camUp)) camera->MoveLocalZ(KCameraMove);
		if (myEngine->KeyHeld(camDown)) camera->MoveLocalZ(-KCameraMove);
		if (myEngine->KeyHeld(camRight)) camera->MoveLocalX(KCameraMove);
		if (myEngine->KeyHeld(camLeft)) camera->MoveLocalX(-KCameraMove);

		//mouse controlled camera movement
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
			}
			else
			{
				myEngine->StopMouseCapture();
			}
		};

		//toggle chase cam mode

		if (myEngine->KeyHit(toggleChaseCam))
		{
			ChaseCamActive = !ChaseCamActive; //making it true
			if (ChaseCamActive)
			{
				camera->AttachToParent(Car);
				camera->SetPosition(CameraX, CameraY, CameraZ);
			}
			else
			{
				camera->DetachFromParent();
			};
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





















































/*if (myEngine->AverageFPS() <= 0) {
	frameTime = kGameSpeed * frameTime;
}
else {
	frameTime = kGameSpeed / myEngine->AverageFPS();
};
carSpeed = frameTime * kGameSpeed;

Car->MoveLocalZ(carSpeed);*/
//frameTime = myEngine->Timer();
