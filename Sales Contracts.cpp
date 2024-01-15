// Sales Contracts.cpp: A program using the TL-Engine

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
	float xPos = -60.0f;
	float zPos = 15.0f;
	const int CubesArrysSize = 9;

	//floor
	IMesh* floorMesh = myEngine->LoadMesh("Floor.x");
	IModel* floor = floorMesh->CreateModel();

	//cubes
	IMesh* cubeMesh = myEngine->LoadMesh("Cube.x"); 
	IModel* cubeModels[CubesArrysSize];

	for (int i = 0; i < CubesArrysSize; i++) {
		if (i % 2 == 0) {
			cubeModels[i] = cubeMesh->CreateModel(xPos, 5.0f, zPos);
		}
		else if (i % 2 != 0)
		{
			cubeModels[i] = cubeMesh->CreateModel(xPos, 5.0f, zPos);
		};
		xPos += 15.0f;
	};

	//camera
	ICamera* mycamera;
	mycamera = myEngine->CreateCamera(kManual, 0, 100.0f, -60.0f);
	mycamera->RotateX(50);

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
