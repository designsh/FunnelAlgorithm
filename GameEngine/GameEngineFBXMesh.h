#pragma once
#include "GameEngineFBX.h"
#include "EngineVertex.h"
#include "GameEngineIndexBuffer.h"
#include "GameEngineVertexBuffer.h"
#include <GameEngineBase/GameEngineDirectory.h>

class FbxExIW
{
public:
	int Index;
	double Weight;
};

class FbxExRenderingPipeLineSettingData
{
public:
	std::string Name;
	// �ȼ����̴����� �׳� �������� �����ָ� �Ǵ� ����.
	float4			 DifColor;
	float4			 AmbColor; // ��
	float4			 SpcColor; // ��
	float4			 EmvColor; // ��
	float			 SpecularPower;		// ���� ����
	float			 TransparencyFactor;	// ���� ����
	float			 Shininess;			// ���� ����
	std::string DifTexturePath;	// �ؽ�ó��� 
	std::string NorTexturePath; // �ؽ�ó���
	std::string SpcTexturePath; // �ؽ�ó���

	std::string DifTextureName;	// �ؽ�ó��� 
	std::string NorTextureName; // �ؽ�ó���
	std::string SpcTextureName; // �ؽ�ó���

public:
	void Write(GameEngineFile* _File) const
	{
		_File->Write(Name);
		_File->Write(DifColor);
		_File->Write(AmbColor); // ��
		_File->Write(SpcColor); // ��
		_File->Write(EmvColor); // ��
		_File->Write(SpecularPower);
		_File->Write(TransparencyFactor);
		_File->Write(Shininess);
		_File->Write(DifTexturePath);	// �ؽ�ó
		_File->Write(NorTexturePath); // �ؽ�ó
		_File->Write(SpcTexturePath); // �ؽ�ó
		_File->Write(DifTextureName);	// �ؽ�ó
		_File->Write(NorTextureName); // �ؽ�ó
		_File->Write(SpcTextureName); // �ؽ�ó

	}

	void Read(GameEngineFile* _File)
	{
		_File->Read(Name);
		_File->Read(DifColor);
		_File->Read(AmbColor); // ��
		_File->Read(SpcColor); // ��
		_File->Read(EmvColor); // ��
		_File->Read(SpecularPower);
		_File->Read(TransparencyFactor);
		_File->Read(Shininess);
		_File->Read(DifTexturePath); // �ؽ�ó
		_File->Read(NorTexturePath); // �ؽ�ó
		_File->Read(SpcTexturePath); // �ؽ�ó
		_File->Read(DifTextureName);	// �ؽ�ó
		_File->Read(NorTextureName); // �ؽ�ó
		_File->Read(SpcTextureName); // �ؽ�ó

		std::filesystem::path path = std::filesystem::current_path();
		path = path.parent_path();
		std::string prevPath;

		if (false == GameEngineDirectory::IsExist(DifTexturePath) &&
			"" != DifTexturePath)
		{
			prevPath = DifTexturePath;
			prevPath.erase(DifTexturePath.find(path.filename().string()) + path.filename().string().size());
			DifTexturePath.replace(0, prevPath.size(), path.string());
		}

		if (false == GameEngineDirectory::IsExist(NorTexturePath) &&
			"" != NorTexturePath)
		{
			prevPath = NorTexturePath;
			prevPath.erase(NorTexturePath.find(path.filename().string()) + path.filename().string().size());
			NorTexturePath.replace(0, prevPath.size(), path.string());
		}

		if (false == GameEngineDirectory::IsExist(SpcTexturePath) &&
			"" != SpcTexturePath)
		{
			prevPath = SpcTexturePath;
			prevPath.erase(SpcTexturePath.find(path.filename().string()) + path.filename().string().size());
			SpcTexturePath.replace(0, prevPath.size(), path.string());
		}
	}

public:

	FbxExRenderingPipeLineSettingData()
		: SpecularPower(0.f)
		, TransparencyFactor(0.f)
		, Shininess(0.f)
	{
	}
	~FbxExRenderingPipeLineSettingData() {}
};


struct FbxMeshSet
{
public:
	int Index;
	int IsLodLv;
	bool IsLod;

	//       �ڽ��� ������ 
	//       ��� �ִ� node
	//       �� mesh
	//       �̸Ž�����  �������� ���´ٶ�� ���� �˴ϴ�.
	//       ���� ���鿡 ���� ��� �����̰�.
	//       ���� �ʿ��ϴٸ� �� ���� �Ҽ��� �ִ�.
	std::map<FbxMesh*, std::vector<GameEngineVertex>*> FbxVertexMap;

	//       �ִϸ��̼��� �ִٸ� ä���� �����̴ϴ�.
	std::map<FbxMesh*, std::map<int, std::vector<FbxExIW>>> MapWI;

	std::vector<GameEngineVertex> Vertexs;

	std::vector<std::vector<FbxExRenderingPipeLineSettingData>> MaterialData;

	std::vector<std::vector<std::vector<unsigned int>>> Indexs; // �޽�<����<�ε���>>>

	std::vector<GameEngineVertexBuffer*> GameEngineVertexBuffers;
	std::vector<std::vector<GameEngineIndexBuffer*>> GameEngineIndexBuffers;
	// std::vector<std::vector<std::shared_ptr<DirectMesh>>> m_Mesh;


	// FbxMeshSet(const FbxMeshSet& _Other) = delete;
	// FbxMeshSet(FbxMeshSet&& _Other) noexcept = delete;
	// FbxMeshSet& operator=(const FbxMeshSet& _Other) = delete;
	// FbxMeshSet& operator=(FbxMeshSet&& _Other) noexcept = delete;

	FbxMeshSet() :
		IsLod(false),
		IsLodLv(-1),
		Index(0)
	{
	}

	~FbxMeshSet()
	{
		for (size_t i = 0; i < GameEngineVertexBuffers.size(); i++)
		{
			if (nullptr == GameEngineVertexBuffers[i])
			{
				continue;
			}
			delete GameEngineVertexBuffers[i];
			GameEngineVertexBuffers[i] = nullptr;
		}

		// ����� ������
		for (size_t i = 0; i < GameEngineIndexBuffers.size(); i++)
		{
			for (size_t j = 0; j < GameEngineIndexBuffers[i].size(); j++)
			{
				if (nullptr == GameEngineIndexBuffers[i][j])
				{
					continue;
				}
				delete GameEngineIndexBuffers[i][j];
				GameEngineIndexBuffers[i][j] = nullptr;
			}
		}

	}
};



struct JointPos
{
	float4 Scale;
	float4 Rotation; // ���ʹϿ�.
	float4 Translation;

	float4 GlobalScale;
	float4 GlobalRotation;
	float4 GlobalTranslation;

	float4x4 Offset;
	float4x4 SotredOffset;
	float4x4 Local;
	float4x4 Global;

	float Length;
	float XSize;
	float YSize;
	float ZSize;

	void Write(GameEngineFile* _File) const
	{
		_File->Write(Scale);
		_File->Write(Rotation); // ���ʹϿ�.
		_File->Write(Translation);
		_File->Write(GlobalScale);
		_File->Write(GlobalRotation);
		_File->Write(GlobalTranslation);
		_File->Write(Offset);
		_File->Write(SotredOffset);
		_File->Write(Local);
		_File->Write(Global);
		_File->Write(Length);
		_File->Write(XSize);
		_File->Write(YSize);
		_File->Write(ZSize);
	}

	void Read(GameEngineFile* _File)
	{
		_File->Read(Scale);
		_File->Read(Rotation); // ���ʹϿ�.
		_File->Read(Translation);
		_File->Read(GlobalScale);
		_File->Read(GlobalRotation);
		_File->Read(GlobalTranslation);
		_File->Read(Offset);
		_File->Read(SotredOffset);
		_File->Read(Local);
		_File->Read(Global);
		_File->Read(Length);
		_File->Read(XSize);
		_File->Read(YSize);
		_File->Read(ZSize);
	}

	void BuildMatrix()
	{
		Global = float4x4::Transformation(GlobalScale, GlobalRotation, GlobalTranslation);
		Local = float4x4::Transformation(Scale, Rotation, Translation);
		Offset = Global.InverseReturn();
	}

	void SetScale(fbxsdk::FbxVector4& scale)
	{
		Scale.x = (float)scale.mData[0];
		Scale.y = (float)scale.mData[1];
		Scale.z = (float)scale.mData[2];
		Scale.w = 0.0f;
	}

	void SetRotation(fbxsdk::FbxQuaternion& quaternion)
	{
		Rotation.x = (float)quaternion.mData[0];
		Rotation.y = (float)quaternion.mData[1];
		Rotation.z = -(float)quaternion.mData[2];
		Rotation.w = -(float)quaternion.mData[3];
	}

	void SetTranslation(fbxsdk::FbxVector4& translation)
	{
		Translation.x = (float)translation.mData[0];
		Translation.y = (float)translation.mData[1];
		Translation.z = -(float)translation.mData[2];
		Translation.w = 0.0f;
	}

	void SetGlobalScale(fbxsdk::FbxVector4& scale)
	{
		GlobalScale.x = (float)scale.mData[0];
		GlobalScale.y = (float)scale.mData[1];
		GlobalScale.z = (float)scale.mData[2];
		GlobalScale.w = 0.0f;
	}

	void SetGlobalRotation(fbxsdk::FbxQuaternion& quaternion)
	{
		GlobalRotation.x = (float)quaternion.mData[0];
		GlobalRotation.y = (float)quaternion.mData[1];
		GlobalRotation.z = -(float)quaternion.mData[2];
		GlobalRotation.w = -(float)quaternion.mData[3];
	}

	void SetGlobalTranslation(fbxsdk::FbxVector4& translation)
	{
		GlobalTranslation.x = (float)translation.mData[0];
		GlobalTranslation.y = (float)translation.mData[1];
		GlobalTranslation.z = -(float)translation.mData[2];
		GlobalTranslation.w = 0.0f;
	}

	void SetFromMatrix(const float4x4& InMatrix)
	{
		float4x4 M = InMatrix;

		// Get the 3D scale from the matrix
		Scale = M.ExtractScaling();
		Scale.w = 0.0;

		// If there is negative scaling going on, we handle that here
		if (InMatrix.Determinant() < 0.f)
		{
			// Assume it is along X and modify transform accordingly. 
			// It doesn't actually matter which axis we choose, the 'appearance' will be the same
			float4 FloatMinus1_111(-1.f, 1.f, 1.f, 1.f);
			Scale *= FloatMinus1_111;// VectorMultiply(Scale, FloatMinus1_111);
			float4 XAxis = -float4(M.Arr2D[0][0], M.Arr2D[0][1], M.Arr2D[0][2], 0.0f);
			M.Arr2D[0][0] = XAxis.x;
			M.Arr2D[0][1] = XAxis.w;
			M.Arr2D[0][2] = XAxis.z;
		}

		Rotation = float4::MatrixToQuaternion(M);
		Translation = float4(InMatrix.Arr2D[3][0], InMatrix.Arr2D[3][1], InMatrix.Arr2D[3][2], 0.0f);

		// Normalize rotation
		__m128 Temp1 = _mm_mul_ps(Rotation.DirectVector, Rotation.DirectVector);
		__m128 Temp2 = _mm_shuffle_ps(Temp1, Temp1, _MM_SHUFFLE(1, 0, 3, 2));	// (Z,W,X,Y).
		Temp1 = _mm_add_ps(Temp1, Temp2);								// (X*X + Z*Z, Y*Y + W*W, Z*Z + X*X, W*W + Y*Y)
		Temp2 = _mm_shuffle_ps(Temp1, Temp1, _MM_SHUFFLE(0, 3, 2, 1));
		__m128 SquareSum = _mm_add_ps(Temp1, Temp2);
		const __m128 NonZeroMask = _mm_cmpge_ps(SquareSum, _mm_setr_ps(1.e-8f, 1.e-8f, 1.e-8f, 1.e-8f));
		const __m128 OneHalf = _mm_setr_ps(0.5f, 0.5f, 0.5f, 0.5f);
		const __m128 VecDivBy2 = _mm_mul_ps(SquareSum, OneHalf);

		// Initial estimate
		const __m128 x0 = _mm_rsqrt_ps(SquareSum);

		// First iteration
		__m128 x1 = _mm_mul_ps(x0, x0);
		x1 = _mm_sub_ps(OneHalf, _mm_mul_ps(VecDivBy2, x1));
		x1 = _mm_add_ps(_mm_mul_ps(x0, x1), x0);

		// Second iteration
		__m128 x2 = _mm_mul_ps(x1, x1);
		x2 = _mm_sub_ps(OneHalf, _mm_mul_ps(VecDivBy2, x2));
		const __m128 InvLength = _mm_add_ps(_mm_mul_ps(x1, x2), x1);

		const __m128 NormalizedVector = _mm_mul_ps(InvLength, Rotation.DirectVector);
		__m128 DefaultValue = _mm_setr_ps(0.0f, 0.0f, 0.0f, 1.0f);
		Rotation.DirectVector = _mm_xor_ps(DefaultValue, _mm_and_ps(NonZeroMask, _mm_xor_ps(NormalizedVector, DefaultValue)));
	}

	void ToMatrixInternal(__m128& OutDiagonals, __m128& OutAdds, __m128& OutSubtracts) const
	{
		const __m128 RotationX2Y2Z2 = _mm_add_ps(Rotation.DirectVector, Rotation.DirectVector);	// x2, y2, z2
		const __m128 RotationXX2YY2ZZ2 = _mm_mul_ps(RotationX2Y2Z2, Rotation.DirectVector);	// xx2, yy2, zz2		

		// The diagonal terms of the rotation matrix are:
		//   (1 - (yy2 + zz2)) * scale
		//   (1 - (xx2 + zz2)) * scale
		//   (1 - (xx2 + yy2)) * scale
		const __m128 yy2_xx2_xx2 = _mm_shuffle_ps(RotationXX2YY2ZZ2, RotationXX2YY2ZZ2, _MM_SHUFFLE(0, 0, 0, 1));
		const __m128 zz2_zz2_yy2 = _mm_shuffle_ps(RotationXX2YY2ZZ2, RotationXX2YY2ZZ2, _MM_SHUFFLE(0, 1, 2, 2));
		const __m128 DiagonalSum = _mm_add_ps(yy2_xx2_xx2, zz2_zz2_yy2);
		const __m128 Diagonals = _mm_sub_ps(float4::ONE.DirectVector, DiagonalSum);
		OutDiagonals = _mm_mul_ps(Diagonals, Scale.DirectVector);

		// Grouping the non-diagonal elements in the rotation block by operations:
		//    ((x*y2,y*z2,x*z2) + (w*z2,w*x2,w*y2)) * scale.xyz and
		//    ((x*y2,y*z2,x*z2) - (w*z2,w*x2,w*y2)) * scale.yxz
		// Rearranging so the LHS and RHS are in the same order as for +
		//    ((x*y2,y*z2,x*z2) - (w*z2,w*x2,w*y2)) * scale.yxz

		// RotBase = x*y2, y*z2, x*z2
		// RotOffset = w*z2, w*x2, w*y2
		const __m128 x_y_x = _mm_shuffle_ps(Rotation.DirectVector, Rotation.DirectVector, _MM_SHUFFLE(0, 0, 1, 0)); // 1, 2, 2, 0
		const __m128 y2_z2_z2 = _mm_shuffle_ps(RotationX2Y2Z2, RotationX2Y2Z2, _MM_SHUFFLE(0, 2, 2, 1));
		const __m128 RotBase = _mm_mul_ps(x_y_x, y2_z2_z2);

		const __m128 w_w_w = _mm_shuffle_ps(Rotation.DirectVector, Rotation.DirectVector, _MM_SHUFFLE(3, 3, 3, 3));
		const __m128 z2_x2_y2 = _mm_shuffle_ps(RotationX2Y2Z2, RotationX2Y2Z2, _MM_SHUFFLE(0, 1, 0, 2));
		const __m128 RotOffset = _mm_mul_ps(w_w_w, z2_x2_y2);

		// Adds = (RotBase + RotOffset)*Scale3D :  (x*y2 + w*z2) * Scale3D.X , (y*z2 + w*x2) * Scale3D.Y, (x*z2 + w*y2) * Scale3D.Z
		// Subtracts = (RotBase - RotOffset)*Scale3DYZX :  (x*y2 - w*z2) * Scale3D.Y , (y*z2 - w*x2) * Scale3D.Z, (x*z2 - w*y2) * Scale3D.X
		const __m128 Adds = _mm_add_ps(RotBase, RotOffset);
		OutAdds = _mm_mul_ps(Adds, Scale.DirectVector);
		const __m128 Scale3DYZXW = _mm_shuffle_ps(Scale.DirectVector, Scale.DirectVector, _MM_SHUFFLE(3, 0, 2, 1));
		const __m128 Subtracts = _mm_sub_ps(RotBase, RotOffset);
		OutSubtracts = _mm_mul_ps(Subtracts, Scale3DYZXW);
	}

	float4x4 ToMatrixWithScale()
	{
		float4x4 OutMatrix;
		__m128 DiagonalsXYZ;
		__m128 Adds;
		__m128 Subtracts;

		ToMatrixInternal(DiagonalsXYZ, Adds, Subtracts);
		union { __m128 v; __m128i i; } tmp;
		tmp.i = _mm_setr_epi32((unsigned int)0xffffffff, (unsigned int)0xffffffff, (unsigned int)0xffffffff, (unsigned int)0x00000000);
		const __m128 DiagonalsXYZ_W0 = _mm_and_ps(DiagonalsXYZ, tmp.v);

		// OutMatrix.M[0][0] = (1.0f - (yy2 + zz2)) * Scale.X;    // Diagonal.X
		// OutMatrix.M[0][1] = (xy2 + wz2) * Scale.X;             // Adds.X
		// OutMatrix.M[0][2] = (xz2 - wy2) * Scale.X;             // Subtracts.Z
		// OutMatrix.M[0][3] = 0.0f;                              // DiagonalsXYZ_W0.W
		const __m128 AddX_DC_DiagX_DC = _mm_shuffle_ps(Adds, DiagonalsXYZ_W0, _MM_SHUFFLE(0, 0, 0, 0));
		const __m128 SubZ_DC_DiagW_DC = _mm_shuffle_ps(Subtracts, DiagonalsXYZ_W0, _MM_SHUFFLE(0, 3, 0, 2));
		const __m128 Row0 = _mm_shuffle_ps(AddX_DC_DiagX_DC, SubZ_DC_DiagW_DC, _MM_SHUFFLE(2, 0, 0, 2));

		// OutMatrix.M[1][0] = (xy2 - wz2) * Scale.Y;             // Subtracts.X
		// OutMatrix.M[1][1] = (1.0f - (xx2 + zz2)) * Scale.Y;    // Diagonal.Y
		// OutMatrix.M[1][2] = (yz2 + wx2) * Scale.Y;             // Adds.Y
		// OutMatrix.M[1][3] = 0.0f;                            // DiagonalsXYZ_W0.W
		const __m128 SubX_DC_DiagY_DC = _mm_shuffle_ps(Subtracts, DiagonalsXYZ_W0, _MM_SHUFFLE(0, 1, 0, 0));
		const __m128 AddY_DC_DiagW_DC = _mm_shuffle_ps(Adds, DiagonalsXYZ_W0, _MM_SHUFFLE(0, 3, 0, 1));
		const __m128 Row1 = _mm_shuffle_ps(SubX_DC_DiagY_DC, AddY_DC_DiagW_DC, _MM_SHUFFLE(2, 0, 2, 0));

		// OutMatrix.M[2][0] = (xz2 + wy2) * Scale.Z;             // Adds.Z
		// OutMatrix.M[2][1] = (yz2 - wx2) * Scale.Z;             // Subtracts.Y
		// OutMatrix.M[2][2] = (1.0f - (xx2 + yy2)) * Scale.Z;    // Diagonals.Z
		// OutMatrix.M[2][3] = 0.0f;                              // DiagonalsXYZ_W0.W
		const __m128 AddZ_DC_SubY_DC = _mm_shuffle_ps(Adds, Subtracts, _MM_SHUFFLE(0, 1, 0, 2));
		const __m128 Row2 = _mm_shuffle_ps(AddZ_DC_SubY_DC, DiagonalsXYZ_W0, _MM_SHUFFLE(3, 2, 2, 0));

		_mm_store_ps(&(OutMatrix.Arr2D[0][0]), Row0);
		_mm_store_ps(&(OutMatrix.Arr2D[1][0]), Row1);
		_mm_store_ps(&(OutMatrix.Arr2D[2][0]), Row2);

		// OutMatrix.M[3][0] = Translation.X;
		// OutMatrix.M[3][1] = Translation.Y;
		// OutMatrix.M[3][2] = Translation.Z;
		// OutMatrix.M[3][3] = 1.0f;
		__m128 Temp1 = _mm_movehl_ps(float4::ONE.DirectVector, Translation.DirectVector);

		const __m128 Row3 = _mm_shuffle_ps(Translation.DirectVector, Temp1, _MM_SHUFFLE(3, 0, 1, 0));
		_mm_store_ps(&(OutMatrix.Arr2D[3][0]), Row3);

		return OutMatrix;
	}

	void Reset()
	{
		Scale = float4::ONE;
		Rotation = float4::ZERO;
		Translation = float4::ZERO;

		Offset.Identity();
	}

	JointPos()
	{
		Scale = float4::ONE;
		Rotation = float4::ZERO;
		Translation = float4::ZERO;

		Offset.Identity();
	}
};

struct Bone
{
	std::string Name;

	unsigned int Flags;
	int			 NumChildren;
	int			 Index;
	int			 ParentIndex;
	JointPos	 BonePos;

	Bone()
	{
		Name = "";
		Flags = 0;
		NumChildren = 0;
		Index = -1;
		ParentIndex = 0;
		BonePos.Reset();
	}

	void Write(GameEngineFile* _File) const
	{
		_File->Write(Name);
		_File->Write(Flags);
		_File->Write(NumChildren);
		_File->Write(Index);
		_File->Write(ParentIndex);
		_File->Write(BonePos);
	}

	void Read(GameEngineFile* _File)
	{
		_File->Read(Name);
		_File->Read(Flags);
		_File->Read(NumChildren);
		_File->Read(Index);
		_File->Read(ParentIndex);
		_File->Read(BonePos);
	}
};


struct FbxExMeshInfo
{
	std::string Name;
	fbxsdk::FbxMesh* Mesh;
	bool bTriangulated;

	unsigned __int64 UniqueId;
	int FaceNum;
	int VertexNum;
	int MaterialNum;

	// �ִϸ��̼ǰ� ����Ǵ� �Ž��ΰ�?
	bool bIsSkelMesh;
	std::string SkeletonRoot;
	int SkeletonElemNum;

	bool bIsLodGroup;
	std::string LODGroup;
	int LodLevel;
	int MorphNum;

	FbxExMeshInfo()
	{
		Name = "";
		UniqueId = 0;
		FaceNum = 0;
		VertexNum = 0;
		bTriangulated = false;
		MaterialNum = 0;
		bIsSkelMesh = false;
		SkeletonRoot = "";
		SkeletonElemNum = 0;
		bIsLodGroup = false;
		LODGroup = "";
		LodLevel = -1;
		MorphNum = 0;
	}
};


class FbxClusterData
{
public:
	fbxsdk::FbxCluster* Cluster = nullptr;
	fbxsdk::FbxMesh* Mesh = nullptr;
	std::string LinkName;

public:
	FbxClusterData()
		: Mesh(nullptr)
		, Cluster(nullptr)
		, LinkName("")
	{

	}
};

// ���� :
class FBXAnimation;
class GameEngineStructuredBuffer;
class GameEngineFBXMesh : public GameEngineFBX
{
	friend FBXAnimation;
	friend GameEngineFBXAnimation;

public:
	// constrcuter destructer
	GameEngineFBXMesh();
	~GameEngineFBXMesh();

	// delete Function
	GameEngineFBXMesh(const GameEngineFBXMesh& _Other) = delete;
	GameEngineFBXMesh(GameEngineFBXMesh&& _Other) noexcept = delete;
	GameEngineFBXMesh& operator=(const GameEngineFBXMesh& _Other) = delete;
	GameEngineFBXMesh& operator=(GameEngineFBXMesh&& _Other) noexcept = delete;

	void Load(const std::string& _Path);
	void CreateRenderingBuffer();

	std::vector<FbxMeshSet>& GetAllMeshMap()
	{
		return AllMeshMap;
	}
	std::vector<FbxExMeshInfo>& GetMeshInfos()
	{
		return MeshInfos;
	}

	size_t GetBoneCount(UINT _Index)
	{
		return AllBones[_Index].size();
	}

	GameEngineStructuredBuffer* GetAnimationBuffer(int _Index)
	{
		if (AnimationBuffers.size() <= _Index)
		{
			return nullptr;
		}

		return AnimationBuffers[_Index];
	}

	bool ImportBone();

	void UserSave(const std::string& _Path);
	void UserLoad(const std::string& _Path);


protected:
	void MeshLoad();

private:
	bool IsAnimationLoadOnce;

	// Lhand RHand

	// �̳༮�� ����
	std::vector<FbxExMeshInfo> MeshInfos;
	std::vector<FbxMeshSet> AllMeshMap;
	std::vector<std::vector<Bone>> AllBones;
	std::vector<std::map<std::string, Bone*>> AllFindMap;
	std::vector<GameEngineStructuredBuffer*> AnimationBuffers;
	std::vector<std::vector<FbxClusterData>> ClusterData;


	fbxsdk::FbxNode* RecursiveFindParentLodGroup(fbxsdk::FbxNode* parentNode);

	void CreateAnimationBuffer();
	void MeshNodeCheck();
	void VertexBufferCheck();

	fbxsdk::FbxAMatrix ComputeTotalMatrix(fbxsdk::FbxNode* Node);
	bool IsOddNegativeScale(const fbxsdk::FbxAMatrix& TotalMatrix);


	// ź��Ʈ ���̳븻 ���
	void LoadBinormal(fbxsdk::FbxMesh* _Mesh, fbxsdk::FbxAMatrix _MeshMatrix, std::vector<GameEngineVertex>& _ArrVtx, int VtxId, int _Index);
	void LoadTangent(fbxsdk::FbxMesh* _Mesh, fbxsdk::FbxAMatrix _MeshMatrix, std::vector<GameEngineVertex>& _ArrVtx, int VtxId, int _Index);
	void LoadNormal(fbxsdk::FbxMesh* _Mesh, fbxsdk::FbxAMatrix _MeshMatrix, std::vector<GameEngineVertex>& _ArrVtx, int VtxId, int _Index);
	void LoadUv(fbxsdk::FbxMesh* _Mesh, fbxsdk::FbxAMatrix _MeshMatrix, std::vector<GameEngineVertex>& _ArrVtx, int VtxId, int VertexCount, int _Index);

	void CreateVertexBuffer();
	void CreateIndexBuffer();

	// ��������(�ؽ�ó)
	void FbxMeshSetMaterialSetting(fbxsdk::FbxNode* _Node, FbxMeshSet* _RenderData);
	float4 MaterialColor(fbxsdk::FbxSurfaceMaterial* pMtrl, const char* _ColorName, const char* _FactorName);
	float MaterialFactor(fbxsdk::FbxSurfaceMaterial* pMtrl, const char* _FactorName);
	std::string MaterialTex(fbxsdk::FbxSurfaceMaterial* pMtrl, const char* _FactorName);

	// �ִϸ��̼� ����


	Bone* FindBone(int NodeIndex, int _Index);
	Bone* FindBone(int NodeIndex, std::string _Name);
	bool IsBone(fbxsdk::FbxNode* Link);

	bool RetrievePoseFromBindPose(fbxsdk::FbxScene* pScene, const std::vector<fbxsdk::FbxNode*>& NodeArray, fbxsdk::FbxArray<fbxsdk::FbxPose*>& PoseArray);
	void BuildSkeletonSystem(fbxsdk::FbxScene* pScene, std::vector<fbxsdk::FbxCluster*>& ClusterArray, std::vector<fbxsdk::FbxNode*>& OutSortedLinks);
	fbxsdk::FbxNode* GetRootSkeleton(fbxsdk::FbxScene* pScene, fbxsdk::FbxNode* Link);
	void RecursiveBuildSkeleton(fbxsdk::FbxNode* Link, std::vector<fbxsdk::FbxNode*>& OutSortedLinks);
	bool IsNull(fbxsdk::FbxNode* Link);

	// ����ġ�� �ε��� ���� ���� �ְ� �ִϸ��̼��� �ҳ༮�鸸 �ϸ� �ȴ�.
	// Ŭ������ �����͸� ó��
	void ImportCluster();
	void LoadSkinAndCluster();
	void LoadAnimationVertexData(FbxMeshSet* _DrawData, const std::vector<FbxClusterData>& vecClusterData);
	void DrawSetWeightAndIndexSetting(FbxMeshSet* _DrawSet, fbxsdk::FbxMesh* _Mesh, fbxsdk::FbxCluster* _Cluster, int _BoneIndex);
	void CalAnimationVertexData(FbxMeshSet& _DrawSet);

	// Lod�׷� �Լ�

	fbxsdk::FbxNode* RecursiveGetFirstMeshNode(fbxsdk::FbxNode* Node, fbxsdk::FbxNode* NodeToFind);
	fbxsdk::FbxNode* FindLODGroupNode(fbxsdk::FbxNode* NodeLodGroup, int LodIndex, fbxsdk::FbxNode* NodeToFind);


};

