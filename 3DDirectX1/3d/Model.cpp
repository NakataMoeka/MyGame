#include "Model.h"
#include <d3dcompiler.h>
#include <DirectXTex.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

using namespace std;

ID3D12Device* Model::dev = nullptr;
UINT Model::descriptorHandleIncrementSize = 0;

bool Model::StaticInitialize(ID3D12Device* dev)
{
	// nullptr�`�F�b�N
	assert(dev);

	Model::dev = dev;

	return true;
}

Model* Model::Create(const std::string& modelname, bool smoothing)
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	Model* model = new Model();
	if (model == nullptr)
	{
		return nullptr;
	}

	// ������
	if (!model->Initialize(modelname,smoothing))
	{
		delete model;
		assert(0);
		return nullptr;
	}

	return model;
}

bool Model::InitializeDescriptorHeap()
{
	HRESULT result = S_FALSE;
	//�ݒ�\����
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NumDescriptors = 1;

	result = dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeap));

	if (FAILED(result)) {
		assert(0);
		return false;
	}
	descriptorHandleIncrementSize = dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	return true;
}

bool Model::LoadTexture(const std::string& directoryPath, const std::string& filename)
{
	HRESULT result = S_FALSE;
	// WIC�e�N�X�`���̃��[�h
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	string filepath = directoryPath + filename;
	wchar_t wfilepath[128];
	int iBufferSize = MultiByteToWideChar(CP_ACP, 0, filepath.c_str(),
		-1, wfilepath, _countof(wfilepath));
	result = LoadFromWICFile(
		wfilepath, WIC_FLAGS_NONE,
		&metadata, scratchImg);
	if (FAILED(result)) {
		return result;
	}

	const Image* img = scratchImg.GetImage(0, 0, 0); // ���f�[�^���o
	// ���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format, metadata.width,
		(UINT)metadata.height,
		(UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels
	);

	// �e�N�X�`���p�o�b�t�@�̐���
	result = dev->CreateCommittedResource(		//GPU���\�[�X�̐���
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,		//�e�N�X�`���p�ݒ�
		nullptr,
		IID_PPV_ARGS(&texbuff));
	if (FAILED(result)) {
		return result;
	}
	//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
	result = texbuff->WriteToSubresource(
		0,
		nullptr,		//�S�̈�փR�s�[
		img->pixels,
		(UINT)img->rowPitch,
		(UINT)img->slicePitch
	);
	if (FAILED(result)) {
		return result;
	}


	// �V�F�[�_���\�[�X�r���[�쐬
	cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap->GetCPUDescriptorHandleForHeapStart(), 0, descriptorHandleIncrementSize);
	gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeap->GetGPUDescriptorHandleForHeapStart(), 0, descriptorHandleIncrementSize);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // �ݒ�\����
	D3D12_RESOURCE_DESC resDesc = texbuff->GetDesc();

	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;

	dev->CreateShaderResourceView(texbuff.Get(), //�r���[�Ɗ֘A�t����o�b�t�@
		&srvDesc, //�e�N�X�`���ݒ���
		cpuDescHandleSRV
	);

	return true;
}

void Model::CreateModel(const std::string& name, bool smoothing)
{
	HRESULT result = S_FALSE;
	int index = 0;
	std::ifstream file;
	//file.open("Resources/triangle/triangle_mat.obj");
	const string modelname = name;
	const string filename = modelname + ".obj";
	const string directoryPath = "Resources/" + modelname + "/";
	file.open(directoryPath + filename);
	if (file.fail()) {
		assert(0);
	}

	int indexCountTex = 0;


	vector<XMFLOAT3>positions;
	vector<XMFLOAT3>normals;
	vector<XMFLOAT2>texcoords;

	string line;
	while (getline(file, line)) {
		std::istringstream line_stream(line);
		string key;
		getline(line_stream, key, ' ');
		//�擪������v�Ȃ璸�_���W
		if (key == "mtllib") {
			string filename;
			line_stream >> filename;
			LoadMaterial(directoryPath, filename);
		}

		if (key == "v") {
			//���W�ǂݍ���
			XMFLOAT3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			//���W�f�[�^�ɒǉ�
			positions.emplace_back(position);

		}
		if (key == "vt") {
			//UV�ǂݍ���
			XMFLOAT2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;
			//V�������]
			texcoord.y = 1.0f - texcoord.y;
			texcoords.emplace_back(texcoord);
		}
		if (key == "vn") {
			XMFLOAT3 normal{};
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;

			normals.emplace_back(normal);
		}
		if (key == "f") {
			int faceIndexCount = 0;
			string index_string;
			while (getline(line_stream, index_string, ' '))
			{
				std::istringstream index_stream(index_string);
				unsigned short indexPosition, indexNormal, indexTexcoord;
				index_stream >> indexPosition;
				
				if (material.textureFilename.size() > 0) {
					index_stream.seekg(1, ios_base::cur);
					index_stream >> indexTexcoord;
					index_stream.seekg(1, ios_base::cur);
					index_stream >> indexNormal;
					//���_�f�[�^�̒ǉ�
					VertexPosNormalUv vertex{};
					vertex.pos = positions[indexPosition - 1];
					vertex.normal = normals[indexNormal - 1];
					vertex.uv = texcoords[indexTexcoord - 1];
					vertices.emplace_back(vertex);
					if (smoothing) {
						AddSmoothData(indexPosition, (unsigned short)vertices.size() - 1);
					}
				}
				else {
					char c;
					index_stream >> c;
					// �X���b�V��2�A���̏ꍇ�A���_�ԍ��̂�
					if (c == '/') {
						// ���_�f�[�^�̒ǉ�
						VertexPosNormalUv vertex{};
						vertex.pos = positions[indexPosition - 1];
						vertex.normal = { 0, 0, 1 };
						vertex.uv = { 0, 0 };
						vertices.emplace_back(vertex);
					}
					else {
						index_stream.seekg(-1, ios_base::cur); // 1�����߂�
						index_stream >> indexTexcoord;
						index_stream.seekg(1, ios_base::cur); // �X���b�V�����΂�
						index_stream >> indexNormal;
						// ���_�f�[�^�̒ǉ�
						VertexPosNormalUv vertex{};
						vertex.pos = positions[indexPosition - 1];
						vertex.normal = normals[indexNormal - 1];
						vertex.uv = { 0, 0 };
						vertices.emplace_back(vertex);
						if (smoothing) {
							AddSmoothData(indexPosition, (unsigned short)vertices.size() - 1);
						}
					}
				}
				// �C���f�b�N�X�f�[�^�̒ǉ�
				if (faceIndexCount >= 3) {
					// �l�p�`�|���S����4�_�ڂȂ̂ŁA
					// �l�p�`��0,1,2,3�̓� 2,3,0�ŎO�p�`���\�z����
					indices.emplace_back(indexCountTex - 1);
					indices.emplace_back(indexCountTex);
					indices.emplace_back(indexCountTex - 3);
				}
				else
				{
					indices.emplace_back(indexCountTex);
				}
				indexCountTex++;
				faceIndexCount++;

			}


		}

	}
	file.close();
	if (smoothing) {
		CalculateSmoothedVertexNormals();
	}
	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	UINT sizeVB = static_cast<UINT>(sizeof(VertexPosNormalUv) * vertices.size());
	//�C���f�b�N�X�f�[�^�̑S�T�C�Y
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * indices.size());


	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr, IID_PPV_ARGS(&vertBuff)
	);

	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeIB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr, IID_PPV_ARGS(&indexBuff)
	);

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	VertexPosNormalUv* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		std::copy(vertices.begin(), vertices.end(), vertMap);
		vertBuff->Unmap(0, nullptr);
	}
	if (FAILED(result)) {
		assert(0);
		return;
	}

	// �C���f�b�N�X�o�b�t�@�ւ̃f�[�^�]��
	unsigned short* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	if (SUCCEEDED(result)) {
		//�S�C���f�b�N�X�ɑ΂���
		std::copy(indices.begin(), indices.end(), indexMap);
		//�Ȃ��������
		indexBuff->Unmap(0, nullptr);
	}



	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(vertices[0]);

	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;

}
void Model::LoadMaterial(const std::string& directoryPath, const std::string& filename)
{
	//�t�@�C���X�g���[��
	std::ifstream file;
	//�}�e���A���t�@�C�����J��
	file.open(directoryPath + filename);
	//�t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail())
	{
		assert(0);
	}

	//1�s���ǂݍ���
	string line;
	while (getline(file, line))
	{
		//1�s���̕�������X�g���[���ɕϊ�
		std::istringstream line_stream(line);

		//���p�X�y�[�X��؂�ōs�̐擪��������擾
		string key;
		getline(line_stream, key, ' ');

		//�擪�̃^�u�����͖�������
		if (key[0] == '\t')
		{
			key.erase(key.begin()); //�擪�̕������폜
		}

		//�擪�̕�����newmtl�Ȃ�}�e���A����
		if (key == "newmtl")
		{//�}�e���A�����ǂݍ���
			line_stream >> material.name;
		}

		//�擪�̕�����Ka�Ȃ�A���r�G���g�F
		if (key == "Ka")
		{
			line_stream >> material.ambient.x;
			line_stream >> material.ambient.y;
			line_stream >> material.ambient.z;
		}

		//�擪�̕�����Kd�Ȃ�f�B�t���[�Y�F
		if (key == "Kd")
		{
			line_stream >> material.diffuse.x;
			line_stream >> material.diffuse.y;
			line_stream >> material.diffuse.z;
		}

		//�擪�̕�����Ks�Ȃ�X�y�L�����[�F
		if (key == "Ks")
		{
			line_stream >> material.specular.x;
			line_stream >> material.specular.y;
			line_stream >> material.specular.z;
		}

		//�擪�̕�����map_Kd�Ȃ�e�N�X�`���t�@�C����
		if (key == "map_Kd")
		{
			//�e�N�X�`���̃t�@�C�����ǂݍ���
			line_stream >> material.textureFilename;
			InitializeDescriptorHeap();
			//�e�N�X�`���ǂݍ���
			LoadTexture(directoryPath, material.textureFilename);
		}
	}
	//�t�@�C�������
	file.close();
}

bool Model::Initialize(const std::string& modelname, bool smoothing)
{

	// nullptr�`�F�b�N
	assert(dev);

	HRESULT result;

	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB1) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr, IID_PPV_ARGS(&constBuffB1));

	CreateModel(modelname, smoothing);

	ConstBufferDataB1* constMap1 = nullptr;
	result = constBuffB1->Map(0, nullptr, (void**)&constMap1);
	constMap1->ambient = material.ambient;
	constMap1->diffuse = material.diffuse;
	constMap1->specular = material.specular;
	constMap1->alpha = material.alpha;
	constBuffB1->Unmap(0, nullptr);


	return true;
}

void Model::AddSmoothData(unsigned short indexPosition, unsigned short indexVertex)
{
	smoothData[indexPosition].emplace_back(indexVertex);
}

void Model::CalculateSmoothedVertexNormals()
{
	auto itr = smoothData.begin();
	for (; itr != smoothData.end(); ++itr) {
		//�e�ʗp�̋��ʒ��_�R���N�V����
		std::vector<unsigned short>& v = itr->second;
		//�S���_�̖@���𕽋ς���
		XMVECTOR normal = {};
		for (unsigned short index : v) {
			normal += XMVectorSet(vertices[index].normal.x, vertices[index].normal.y, vertices[index].normal.z, 0);
		}
		normal = XMVector3Normalize(normal / (float)v.size());
		//���ʖ@�����g�p����S�Ă̒��_�f�[�^�ɏ�������
		for (unsigned short index : v) {
			vertices[index].normal = { normal.m128_f32[0], normal.m128_f32[1], normal.m128_f32[2] };
		}
	}
}

void Model::Draw(ID3D12GraphicsCommandList* cmdList)
{
	// nullptr�`�F�b�N
	assert(dev);


	// ���_�o�b�t�@�̐ݒ�
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	// �C���f�b�N�X�o�b�t�@�̐ݒ�
	cmdList->IASetIndexBuffer(&ibView);

	// �f�X�N���v�^�q�[�v�̔z��
	ID3D12DescriptorHeap* ppHeaps[] = { descHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	// �萔�o�b�t�@�r���[���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(1, constBuffB1->GetGPUVirtualAddress());
	//// �V�F�[�_���\�[�X�r���[���Z�b�g
	cmdList->SetGraphicsRootDescriptorTable(2, gpuDescHandleSRV);

	// �`��R�}���h
	cmdList->DrawIndexedInstanced((UINT)indices.size(), 1, 0, 0, 0);
}
