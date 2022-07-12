#include "player.h"
#include"input.h"
#include <cassert>
using namespace DirectX;
Player::Player()
{
}
Player::~Player()
{
}
void Player::Initialize()
{
	model = model->Create("car", false);
	playerObj = Object3d::Create(model);
	model2 = model2->Create("bullet", false);
	SphereObj = Object3d::Create(model2);
	//Createの後に書かないとclient.hのInternalRelease()でエラーが起こる//Createの後に書かないとclient.hのInternalRelease()でエラーが起こる
	playerObj->CreateGraphicsPipeline(L"Resources/shaders/OBJPS.hlsl", L"Resources/shaders/OBJVS.hlsl");
	SphereObj->CreateGraphicsPipeline(L"Resources/shaders/OBJPS.hlsl", L"Resources/shaders/OBJVS.hlsl");


}

void Player::Init()
{
	sphere.radius = r;
	sphere.center = XMVectorSet(spherePos.x, spherePos.y, spherePos.z, 1);
	playerPos={ 0,-0.8,-50 };
	playerObj->SetPosition(playerPos);
}


void Player::Move()
{
	XMVECTOR moveUD = { 0,0,0.1f,0 };//上下方向用の移動ベクトル
	XMVECTOR moveLR = { 0.1f,0,0,0 };//左右方向の移動用ベクトル
	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(playerAngle.y));//y 軸を中心に回転するマトリックスを作成
	moveUD = XMVector3TransformNormal(moveUD, matRot);
	moveLR = XMVector3TransformNormal(moveLR, matRot);
	if (Input::GetInstance()->PushKey(DIK_RIGHTARROW))
	{
		playerAngle.y += 1;
		sphereAngle.y += 1;

	}
	else if (Input::GetInstance()->PushKey(DIK_LEFTARROW))
	{
		playerAngle.y -= 1;
		sphereAngle.y -= 1;
	}
	if (Input::GetInstance()->PushKey(DIK_W))
	{
		playerPos.x += moveUD.m128_f32[0];
		playerPos.z += moveUD.m128_f32[2];
		sphereAngle.x += 10;
	}
	else if (Input::GetInstance()->PushKey(DIK_S))
	{
		playerPos.x -= moveUD.m128_f32[0];
		playerPos.z -= moveUD.m128_f32[2];
		sphereAngle.x -= 10;
	}
	else if (Input::GetInstance()->PushKey(DIK_D))
	{
		playerPos.x += moveLR.m128_f32[0];
		playerPos.z += moveLR.m128_f32[2];
		sphereAngle.z += 10;
	}
	else if (Input::GetInstance()->PushKey(DIK_A))
	{
		playerPos.x -= moveLR.m128_f32[0];
		playerPos.z -= moveLR.m128_f32[2];
		sphereAngle.z -= 10;
	}
	sphere.radius = r;
	sphere.center = XMVectorSet(spherePos.x, spherePos.y, spherePos.z, 1);

	playerObj->SetPosition(playerPos);
	playerObj->SetRotation(playerAngle);
	playerObj->SetScale({ 0.3, 0.3, 0.3 });
	playerObj->Update();

}

void Player::Ball()
{
#pragma region カメラ追従とほぼ同じ
	XMFLOAT3 V0 = { 0,0,1.6 };
	//2
	XMMATRIX  rotM = XMMatrixIdentity();
	rotM *= XMMatrixRotationY(XMConvertToRadians(playerAngle.y));//Y軸
	rotM *= XMMatrixRotationX(XMConvertToRadians(0));//X軸
	//3
	XMVECTOR v3 = { V0.x,V0.y,V0.z };
	XMVECTOR v = XMVector3TransformNormal(v3, rotM);

	//4
	XMFLOAT3 f3 = { v.m128_f32[0],v.m128_f32[1],v.m128_f32[2] };
	spherePos.x = playerPos.x + f3.x;
	//spherePos.y = playerPos.y + f3.y;
	spherePos.z = playerPos.z + f3.z;
#pragma endregion

	SphereObj->SetPosition(spherePos);
	SphereObj->SetScale(sphereSize);
	SphereObj->SetRotation(sphereAngle);
	SphereObj->Update();

}

void Player::Update()
{
	Move();
	Ball();

}


void Player::Draw()
{
	playerObj->Draw();
	SphereObj->Draw();
}
