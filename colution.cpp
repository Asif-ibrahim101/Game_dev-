// colution.cpp: A program using the TL-Engine

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
	IMesh* gridMesh = myEngine->LoadMesh("Grid.x");
	IModel* grid = gridMesh->CreateModel();

	IMesh* cubeMesh = myEngine->LoadMesh("Cube.x");
	IModel* cube = cubeMesh->CreateModel();


	IMesh* sphereMesh = myEngine->LoadMesh("Sphere.x");
	IModel* sphere = sphereMesh->CreateModel(-30.0f, 10.0f, -10.0f);
	sphere->AttachToParent(cube);

	//camera
	ICamera* myCamera;
	myCamera = myEngine->CreateCamera(FPSCamera);

	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();

		/**** Update your scene each frame here ****/

		//moving the cube
		if (myEngine->KeyHeld(Key_Up)) {
			cube->MoveZ(0.05f);
		}
		else if (myEngine->KeyHeld(Key_Left))
		{
			cube->MoveX(-0.05f);
		}
		else if (myEngine->KeyHeld(Key_Down))
		{
			cube->MoveZ(-0.05f);
		}
		else if (myEngine->KeyHeld(Key_Right))
		{
			cube->MoveX(0.05f);
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
