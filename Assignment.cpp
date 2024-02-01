// Assignment.cpp: A program using the TL-Engine

#include "TL-Engine11.h" // TL-Engine11 include file and namespace
using namespace tle;

//float distance(IModel* Block_model, IModel* Sphere_model);
//bool Sphere2Sphere(IModel* Block, IModel* Sphere, float radius_block, float radius_sphere);

int main()
{
	// Create a 3D engine (using TL11 engine here) and open a window for it
	TLEngine* myEngine = New3DEngine(kTLX);
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder("Media");
	myEngine->AddMediaFolder("C:\\Users\\Public\\Documents\\TL-Engine11\\Media");

	//States
	bool Ready = false;
	bool Firing = false;
	bool Collision_state = false;
	bool over = false;

	/**** Set up your scene here ****/
	//speed of the objects
	float KGameSpeed = 1.0f;

	//Barriars1
	float barrier_xPos = 10.0f;
	float barrier_YPos = 4.0f;
	float barrier_ZPos = 0.0f;
	const int BarrierArray_length = 10;

	IMesh* BarrierMesh = myEngine->LoadMesh("Barrier.x");
	IModel* BarrierModels[BarrierArray_length];
	IModel* BarrierModels1[BarrierArray_length];


	//positioning all the models
	for (int i = 0; i < BarrierArray_length; i++) {
		BarrierModels[i] = BarrierMesh->CreateModel(-barrier_xPos, barrier_YPos, barrier_ZPos);
		BarrierModels[i]->SetSkin("barrier1a.BMP");
		barrier_ZPos += 14;
	};

	//Barrier2
	float barrier_xPos1 = 118.0f;
	float barrier_YPos1 = 4.0f;
	float barrier_ZPos1 = 0.0f;
	//const int BarrierArray_length1 = 10;

	//positioning all the models
	for (int i = 0; i < BarrierArray_length; i++) {
		BarrierModels1[i] = BarrierMesh->CreateModel(barrier_xPos1, barrier_YPos1, barrier_ZPos1);
		BarrierModels1[i]->SetSkin("barrier1a.BMP");
		barrier_ZPos1 += 14;
	};

	//floor
	IMesh* floorMesh = myEngine->LoadMesh("Floor.X");
	IModel* floor = floorMesh->CreateModel();

	//sky
	const float sky_X = 0.0f;
	const float sky_Y = -1000.0f;
	const float sky_Z = 0.0f;

	IMesh* skyBoxMesh = myEngine->LoadMesh("Skybox_Hell.x");
	IModel* skyBox = skyBoxMesh->CreateModel(sky_X, sky_Y, sky_Z);

	//Dummy
	const float dummy_x = 50.0f;
	const float dummy_y = 1.0f;
	const float dummy_z = 0.0f;

	IMesh* dummyMesh = myEngine->LoadMesh("Dummy.x");
	IModel* dummy = dummyMesh->CreateModel(dummy_x, dummy_y, dummy_z);


	//marbels
	const float marble_x = 0.0f;
	const float marble_y = 2.0f;
	const float marble_z = 0.0f;

	IMesh* marbleMesh = myEngine->LoadMesh("Marble.x");
	IModel* marble = marbleMesh->CreateModel(marble_x, marble_y, marble_z);
	marble->AttachToParent(dummy);

	//Arrow
	const float Arrow_x = 0.0f;
	const float Arrow_y = 1.0f;
	const float Arrow_z = 0.0f;

	IMesh* ArrowMesh = myEngine->LoadMesh("Arrow.x");
	IModel* Arrow = ArrowMesh->CreateModel(Arrow_x, Arrow_y, Arrow_z);
	Arrow->AttachToParent(dummy);
	Arrow->MoveZ(-10.0f);

	//Blocks
	float xPos = 0.0f;
	float YPos = 4.0f;
	float ZPos = 120.0f;
	const int BlockArray_length = 10;

	IMesh* blockMesh = myEngine->LoadMesh("Block.x");
	IModel* BlockModels[BlockArray_length];
	bool BlockHits[BlockArray_length];

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
	//mycamera = myEngine->CreateCamera(FPSCamera);
	mycamera = myEngine->CreateCamera(ManualCamera, Camera_x, Camera_y, Camera_z);
	mycamera->RotateX(Camera_Rotation);

	//rotation of the marble
	float rotation = 0.0f;
	const float Rotation_limmit = 50.0f;
	const float Rotation_speed = KGameSpeed / 6;


	//radius of the spheres
	float radius_marble = 2.0f;
	float radius_block = 6.0f;

	//for the blocks
	float bWidth = 12.0f;
	float bDepth = 12.0f;

	//for the barriers
	float BeWidth = 15.0f;
	float BeDepth = 15.0f;

	//maximum distance of the marble
	const float Max = 200.0f;

	//for counting amount of hit 
	int Hit = 0;

	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();

		//getting location of the marble
		float OldX = dummy->GetX();
		float OldZ = dummy->GetZ();

		//moving the marble and starting Ready State
		if (myEngine->KeyHeld(Key_Z) && rotation < Rotation_limmit)
		{
			if (Ready == false) {
				dummy->RotateY(Rotation_speed);
				rotation += Rotation_speed;
			};
		};

		if (myEngine->KeyHeld(Key_X) && rotation > -Rotation_limmit) {

			if (Ready == false) {
				dummy->RotateY(-Rotation_speed);
				rotation -= Rotation_speed;
			};
		};

		//Hiting State
		if (myEngine->KeyHeld(Key_Space)) {
			if (Firing == false) {
				marble->MoveLocalZ(KGameSpeed);
				Ready = true;
			};
		};

		//looking for collision
		if (!Firing) {

			//for blocks
			for (int i = 0; i < BlockArray_length; i++) {

				//sphere to box collision
				float minX = BlockModels[i]->GetX() - (BeWidth / 2) - radius_marble;
				float maxX = BlockModels[i]->GetX() + (BeWidth / 2) + radius_marble;
				float minZ = BlockModels[i]->GetZ() - (BeDepth / 2) - radius_marble;
				float maxZ = BlockModels[i]->GetZ() - (BeDepth / 2) + radius_marble;

				bool collision = marble->GetX() > minX && marble->GetX() < maxX
					&& marble->GetZ() > minZ && marble->GetZ() < maxZ;

				//resolving the collision
				if (collision) {
					Hit++;
					BlockModels[i]->SetSkin("tiles_red.jpg");
					//changing Blocks colour after collision
					Ready = true;
					Firing = true;
					Collision_state = true;
				};
			};

			//for barrier1
			for (int i = 0; i < BarrierArray_length; i++) {

				//sphere to box collision
				float minX = BarrierModels[i]->GetX() - (BeWidth / 2) - radius_marble;
				float maxX = BarrierModels[i]->GetX() + (BeWidth / 2) + radius_marble;
				float minZ = BarrierModels[i]->GetZ() - (BeDepth / 2) - radius_marble;
				float maxZ = BarrierModels[i]->GetZ() - (BeDepth / 2) + radius_marble;

				bool collision = marble->GetX() > minX && marble->GetX() < maxX
					&& marble->GetZ() > minZ && marble->GetZ() < maxZ;

				//resolving the collision
				if (collision) {
					//BlockModels[i]->SetSkin("tiles_red.jpg");
					//changing Blocks colour after collision
					Ready = true;
					Firing = true;
					Collision_state = true;
				};
			};

			//for barrier2
			for (int i = 0; i < BarrierArray_length; i++) {

				//sphere to box collision
				float minX = BarrierModels1[i]->GetX() - (bWidth / 2) - radius_marble;
				float maxX = BarrierModels1[i]->GetX() + (bWidth / 2) + radius_marble;
				float minZ = BarrierModels1[i]->GetZ() - (bDepth / 2) - radius_marble;
				float maxZ = BarrierModels1[i]->GetZ() - (bDepth / 2) + radius_marble;

				bool collision = marble->GetX() > minX && marble->GetX() < maxX
					&& marble->GetZ() > minZ && marble->GetZ() < maxZ;

				//resolving the collision
				if (collision) {
					//BlockModels[i]->SetSkin("tiles_red.jpg");
					//changing Blocks colour after collision
					Ready = true;
					Firing = true;
					Collision_state = true;
				};
			};

		};

		if (Collision_state == true) {
			//Returing the marble
			if (myEngine->KeyHit(Key_R)) {
				marble->SetX(OldX);
				marble->SetZ(OldZ);
				Ready = false;
				Firing = false;
				Collision_state = false;
			}
		};


		////checking if the hit is more than 3
		if (Hit > 4) {
			dummy->SetX(OldX);
			dummy->SetZ(OldZ);
			Ready = true;
			Firing = true;
			Collision_state = true;
			over = true;

			//Changing the marbles colour
			marble->SetSkin("glass_blue.jpg");

			//text on screen after hitting 3 blocks
			IFont* myFont = myEngine->LoadFont("Comic Sans MS", 56);
			myFont->Draw("Game Over", 200, 100);
		};

		//over state
		if (marble->GetLocalZ() >= Max && over == false) {
			marble->SetX(OldX);
			marble->SetZ(OldZ);
			Ready = true;
			Firing = true;
			Collision_state = true;

			//Returns of the original position
			if (myEngine->KeyHit(Key_R)) {
				marble->SetX(marble_x);
				marble->SetZ(marble_z);
				Ready = false;
				Firing = false;
				Collision_state = false;
			};
		};

		// Stop if the Escape key is pressed
		if (myEngine->KeyHit(Key_Escape))
		{
			myEngine->Stop();
		};
	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
};


////all functions
//float distance(IModel* Block_model, IModel* Sphere_model) {
//	float distx = Block_model->GetX() - Sphere_model->GetX();
//	float distz = Block_model->GetZ() - Sphere_model->GetZ();
//
//	return sqrt((distx * distx) + (distz * distz));
//};
//
//bool Sphere2Sphere(IModel* Block, IModel* Sphere, float radius_block, float radius_sphere) {
//	float dist = distance(Block, Sphere);
//	return dist < (radius_block + radius_sphere);
//};
