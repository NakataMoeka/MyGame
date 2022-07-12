#pragma once
#include"Object3d.h"
#include"Model.h"
#include"Collision.h"
#include<DirectXMath.h>
/// <summary>
/// �X�e�[�W�ɔz�u����Ă���I�u�W�F�N�g�̃N���X
/// </summary>
class GameObject
{
private:
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	GameObject();
	~GameObject();
	void Initialize();
	void Init();
	void Update();
	void Draw();
	Box	GetCBox() { return cBox; }
	Sphere GetCSphere() { return cSphere; }
	XMFLOAT3 GetPos() { return position; }
	XMFLOAT3 SetPos(XMFLOAT3 pos) { return this->position=pos; }
	XMFLOAT3 GetSize() { return size; }
	XMFLOAT3 SetSize(XMFLOAT3 size) { return this->size = size; }
	Object3d* GetObject() { return cube; }
private:
	Object3d* cube;
	Model* modelCube;
	Box cBox;
	Sphere cSphere;
	float r = 1;
	XMFLOAT3 position = { 5,0.5,0 };
	XMFLOAT3 size = { 0.1,0.1,0.1 };
};

