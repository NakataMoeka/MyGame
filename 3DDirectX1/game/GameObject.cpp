#include "GameObject.h"
using namespace DirectX;
GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

void GameObject::Initialize()
{
	modelCube = modelCube->Create("cube", false);
	cube = Object3d::Create(modelCube);
	cube->CreateGraphicsPipeline(L"Resources/shaders/OBJPS.hlsl", L"Resources/shaders/OBJVS.hlsl");
}

void GameObject::Init()
{
	cSphere.radius = r;
	cSphere.center = XMVectorSet(position.x, position.y, position.z,1);
	cBox.minPosition = XMVectorSet(position.x - r, position.y - r, position.z - r, 1);
	cBox.maxPosition = XMVectorSet(position.x + r, position.y + r, position.z + r, 1);
}

void GameObject::Update()
{
	cSphere.radius = r;
	cSphere.center = XMVectorSet(position.x, position.y, position.z, 1);
	cBox.minPosition = XMVectorSet(position.x - r, position.y - r, position.z - r, 1);
	cBox.maxPosition = XMVectorSet(position.x + r, position.y + r, position.z + r, 1);

	cube->SetPosition(position);

	cube->SetScale(size);
	cube->Update();
}

void GameObject::Draw()
{
	cube->Draw();
}
