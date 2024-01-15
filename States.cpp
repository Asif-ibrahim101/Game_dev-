// States.cpp: A program using the TL-Engine

#include "TL-Engine11.h" // TL-Engine11 include file and namespace
using namespace tle;

int main()
{
	// Create a 3D engine (using TL11 engine here) and open a window for it
	TLEngine* myEngine = New3DEngine( TL11 );
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder("Media");
	myEngine->AddMediaFolder( "C:\\Users\\Public\\Documents\\TL-Engine11\\Media" );


	/**** Set up your scene here ****/

	//state
	int isSphereVisible = 0;

	//state for colour
	enum EColourState { Red, Green, Blue };
	EColourState sphereColour = Red;

	//state for rotation

	//Roatation speed of the sphere
	const float Kspeed = 0.05f;
	float rotationSpeed = Kspeed;

	enum Rotation {Up, right, Left, Down};
	Rotation SphereRotation = Up;

	IMesh* floorMesh = myEngine->LoadMesh("Floor.X");
	IModel* floor = floorMesh->CreateModel();

	IMesh* sphereMesh = myEngine->LoadMesh("Sphere.x");
	IModel* sphere= sphereMesh->CreateModel(0, 10, 0);

	//camera
	ICamera* myCamera;
	myCamera = myEngine->CreateCamera(kManual, 0, 70, -100);
	myCamera->RotateLocalX(20);

	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();

		/**** Update your scene each frame here ****/

		//if entre key is hit
		if (myEngine->KeyHit(Key_Return)) {
			if (isSphereVisible == 0) {
				sphereMesh->RemoveModel(sphere);
				isSphereVisible = 1;
			}
			else
			{
				sphere = sphereMesh->CreateModel(0.0f, 10.0f, 0.0f);
				isSphereVisible = 0;
			};
		};

		//if space key is hit change colour
		if (myEngine->KeyHit(Key_Space) && isSphereVisible == 0) {
			if (sphereColour == Red) {
				sphereColour = Green;
				sphere->SetSkin("Venus.jpg");
			}
			else if (sphereColour == Green)
			{
				sphereColour = Blue;
				sphere->SetSkin("Sun.jpg");
			}
			else if (sphereColour == Blue)
			{
				sphereColour = Red;
				sphere->SetSkin("RedBall.jpg");
			}
		};


		//cheking the sphere Rotating way
		if (myEngine->KeyHeld(Key_Up)) {
			SphereRotation = Up;
			rotationSpeed = Kspeed;
		}
		else if (myEngine->KeyHeld(Key_Down))
		{
			SphereRotation = Down;
			rotationSpeed = -Kspeed;
		}
		else if (myEngine->KeyHeld(Key_Left))
		{
			SphereRotation = Left;
			rotationSpeed = -Kspeed;
		}
		else if (myEngine->KeyHeld(Key_Right))
		{
			SphereRotation = right;
			rotationSpeed = Kspeed;
		}

		//Rotating the sphere
		if (SphereRotation == Up) {
			sphere->RotateX(rotationSpeed);
		}
		else if (SphereRotation == Down) 
		{
			sphere->RotateX(rotationSpeed);
		}
		else if (SphereRotation == Left)
		{
			sphere->RotateY(rotationSpeed);
		}
		else if (SphereRotation == right)
		{
			sphere->RotateY(rotationSpeed);
		}

		// Stop if the Escape key is pressed
		if (myEngine->KeyHit( Key_Escape ))
		{
			myEngine->Stop();
		}
	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}
