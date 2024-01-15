// Lab1.cpp: A program using the TL-Engine

#include "TL-Engine11.h" // TL-Engine11 include file and namespace
using namespace tle;

int main()
{
	// Create a 3D engine (using TL11 engine here) and open a window for it
	TLEngine* myEngine = New3DEngine( TL11 );
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder( "C:\\ProgramData\\TL-Engine11\\Media" );

	/**** Set up your scene here ****/
	IMesh* gridMesh;
	IModel* grid;
	gridMesh = myEngine->LoadMesh("Grid.x");
	grid = gridMesh->CreateModel();

	IMesh * cubeMesh;
	IModel* cube;
	cubeMesh = myEngine->LoadMesh("Cube.x");
	cube = cubeMesh->CreateModel(20, 5, 50);


	ICamera* myCamera;
	myCamera = myEngine->CreateCamera( kFPS );

	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();

		/**** Update your scene each frame here ****/
		if (myEngine->KeyHeld( Key_L ))
		{
			cube->MoveX(0.02);
		}

		if (myEngine->KeyHeld(Key_J))
		{
			cube->MoveX(-0.02);
		}		
		
		if (myEngine->KeyHeld(Key_I))
		{
			cube->MoveZ(0.02);
		}	

		if (myEngine->KeyHeld(Key_K))
		{
			cube->MoveZ(-0.02);
		}

		// Stop if the Escape key is pressed
		if (myEngine->KeyHit( Key_Escape ))
		{
			myEngine->Stop();
		}
	}

	// Delete the 3D engine now we are finished with it
}
