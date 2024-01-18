// Assignment.cpp: A program using the TL-Engine

#include "TL-Engine11.h" // TL-Engine11 include file and namespace
using namespace tle;

int main()
{
	// Create a 3D engine (using TL11 engine here) and open a window for it
	TLEngine* myEngine = New3DEngine(kTLX);
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder("Media");
	myEngine->AddMediaFolder("C:\\Users\\Public\\Documents\\TL-Engine11\\Media");


    /**** Set up your scene here ****/

	//States
	bool Ready = false;

	//speed of the objects
	const float KGameSpeed = 1.0f;

	//floor
	IMesh* floorMesh = myEngine->LoadMesh("Floor.X");
	IModel* floor = floorMesh->CreateModel();

	//sky
	const float sky_X = 0.0f;
	const float sky_Y = -1000.0f;
	const float sky_Z = 0.0f;

	IMesh* skyBoxMesh = myEngine->LoadMesh("Skybox_Hell.x");
	IModel* skyBox = skyBoxMesh->CreateModel(sky_X, sky_Y, sky_Z);

	//marble
	const float marble_x = 50.0f;
	const float marble_y = 1.0f;
	const float marble_z = 0.0f;

	IMesh* marbleMesh = myEngine->LoadMesh("Marble.x");
	IModel* marble = marbleMesh->CreateModel(marble_x, marble_y, marble_z);

	//Arrow
	const float Arrow_x = 0.0f;
	const float Arrow_y = 1.0f;
	const float Arrow_z = 0.0f;

	IMesh* ArrowMesh = myEngine->LoadMesh("Arrow.x");
	IModel* Arrow = ArrowMesh->CreateModel(Arrow_x, Arrow_y, Arrow_z);
	Arrow->AttachToParent(marble);
	Arrow->MoveZ(-10.0f);

	//Blocks
	float xPos = 0.0f;
	float YPos = 4.0f;
	float ZPos = 120.0f;
	const int BlockArray_length = 10;

	IMesh* blockMesh = myEngine->LoadMesh("Block.x");
	IModel* BlockModels[BlockArray_length];

	//positioning all the models
	for (int i = 0; i < BlockArray_length; i++) {
		BlockModels[i] = blockMesh->CreateModel(xPos, YPos, ZPos);
		xPos += 12;
	};

	//Camera setup
	const float Camera_x = 50.0f;
	const float Camera_y = 30.0f;
	const float Camera_z = -60.0f;
	const float Camera_Rotation = 10.0f;

	ICamera* mycamera;
	mycamera = myEngine->CreateCamera(ManualCamera, Camera_x, Camera_y, Camera_z);
	mycamera->RotateX(Camera_Rotation);

	//rotation of the sphere
	float rotation = 0.0f;
	const float Rotation_limmit = 50.0f;
	const float Rotation_speed = KGameSpeed - 0.5f;


	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();

		/**** Update your scene each frame here ****/
		
		//moving the marble
		if (myEngine->KeyHeld(Key_Z) && rotation < Rotation_limmit)
		{
			marble->RotateY(Rotation_speed);
			rotation += Rotation_speed;
		};

		if (myEngine->KeyHeld(Key_X) && rotation > -Rotation_limmit ) {
			marble->RotateY(-Rotation_speed);
			rotation -= Rotation_speed;
		};

		// Stop if the Escape key is pressed
		if (myEngine->KeyHit(Key_Escape))
		{
			myEngine->Stop();
		};
	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}
