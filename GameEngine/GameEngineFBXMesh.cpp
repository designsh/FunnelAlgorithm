#include "PreCompile.h"
#include "GameEngineFBXMesh.h"
#include "GameEngineStructuredBuffer.h"
#include <GameEngineBase/GameEngineFile.h>

GameEngineFBXMesh::GameEngineFBXMesh() 
	: IsAnimationLoadOnce(false)
	, AnimationBuffers()
{
}

GameEngineFBXMesh::~GameEngineFBXMesh() 
{
	if (0 != AnimationBuffers.size())
	{
		for (size_t i = 0; i < AnimationBuffers.size(); i++)
		{
			if (nullptr == AnimationBuffers[i])
			{
				continue;
			}

			delete AnimationBuffers[i];
		}
	}
}


void GameEngineFBXMesh::Load(const std::string& _Path)
{
	if (false == CreateFBXSystemInitialize(_Path))
	{
		GameEngineDebug::MsgBoxError("FBX �̴ϼȶ���� �����߽��ϴ�.");
		return;
	}

	FBXConvertScene();

	MeshLoad();

	ImportBone();
}



void GameEngineFBXMesh::ImportCluster()
{
	size_t meshCount = MeshInfos.size();
	if (0 == meshCount)
	{
		return;
	}

	// 36 
	// 0
	// 0 
	// 0
	// 0
	// 36 

	for (size_t n = 0; n < meshCount; ++n)
	{
		FbxExMeshInfo& meshInfo = MeshInfos.at(n);
		fbxsdk::FbxNode* pNode = meshInfo.Mesh->GetNode();
		fbxsdk::FbxMesh* FbxMesh = meshInfo.Mesh;
		const int SkinDeformerCount = FbxMesh->GetDeformerCount(fbxsdk::FbxDeformer::eSkin);

		std::vector<FbxClusterData>& vecClusterData = ClusterData.emplace_back();

		// ������ �������� �ٲ��ڵ�� ���� ���̵� ����Ʈ�� ����ų���𸨴ϴ�.
		if (0 == SkinDeformerCount)
		{
			continue;
		}

		for (int DeformerIndex = 0; DeformerIndex < SkinDeformerCount; DeformerIndex++)
		{
			fbxsdk::FbxSkin* Skin = (fbxsdk::FbxSkin*)FbxMesh->GetDeformer(DeformerIndex, fbxsdk::FbxDeformer::eSkin);
			for (int ClusterIndex = 0; ClusterIndex < Skin->GetClusterCount(); ClusterIndex++)
			{
				vecClusterData.push_back(FbxClusterData());
				FbxClusterData& clusterData = vecClusterData[vecClusterData.size() - 1];
				clusterData.Cluster = Skin->GetCluster(ClusterIndex);
				clusterData.Mesh = FbxMesh;
				clusterData.LinkName = clusterData.Cluster->GetLink()->GetName();
			}
		}
	}
}



fbxsdk::FbxNode* GameEngineFBXMesh::RecursiveFindParentLodGroup(fbxsdk::FbxNode* parentNode)
{
	if (nullptr == parentNode)
	{
		return nullptr;
	}
	fbxsdk::FbxNodeAttribute* attribute = parentNode->GetNodeAttribute();
	if (nullptr != attribute && attribute->GetAttributeType() == fbxsdk::FbxNodeAttribute::eLODGroup)
	{
		return parentNode;
	}
	return RecursiveFindParentLodGroup(parentNode->GetParent());
}



fbxsdk::FbxNode* GameEngineFBXMesh::RecursiveGetFirstMeshNode(fbxsdk::FbxNode* Node, fbxsdk::FbxNode* NodeToFind)
{
	if (Node == nullptr)
	{
		return nullptr;
	}

	if (Node->GetMesh() != nullptr)
	{
		return Node;
	}

	for (int ChildIndex = 0; ChildIndex < Node->GetChildCount(); ++ChildIndex)
	{
		fbxsdk::FbxNode* MeshNode = RecursiveGetFirstMeshNode(Node->GetChild(ChildIndex), NodeToFind);
		if (NodeToFind == nullptr)
		{
			if (MeshNode != nullptr)
			{
				return MeshNode;
			}
		}
		else if (MeshNode == NodeToFind)
		{
			return MeshNode;
		}
	}
	return nullptr;
}


fbxsdk::FbxNode* GameEngineFBXMesh::FindLODGroupNode(fbxsdk::FbxNode* NodeLodGroup, int LodIndex, fbxsdk::FbxNode* NodeToFind)
{
	if (NodeLodGroup->GetChildCount() < LodIndex)
	{
		GameEngineDebug::MsgBoxError("�߸��� �ε���");
		return nullptr;
	}
	fbxsdk::FbxNode* childNode = NodeLodGroup->GetChild(LodIndex);
	if (nullptr == childNode)
	{
		return nullptr;
	}
	return RecursiveGetFirstMeshNode(childNode, NodeToFind);
}


void GameEngineFBXMesh::MeshNodeCheck()
{
	int geometryCount = Scene->GetGeometryCount();
	for (int i = 0; i < geometryCount; i++)
	{
		// ����߿��� ���ϱ����� �����༮���� �̾Ƴ��°��̰�.
		fbxsdk::FbxGeometry* geoMetry = Scene->GetGeometry(i);
		fbxsdk::FbxNode* geoMetryNode = geoMetry->GetNode();

		// �̾Ƴ� �ֵ��߿��� �� Ÿ����
		if (geoMetry->GetAttributeType() == fbxsdk::FbxNodeAttribute::eMesh)
		{
			MeshInfos.push_back(FbxExMeshInfo());
			FbxExMeshInfo& Info = MeshInfos[MeshInfos.size() - 1];

			if (geoMetry->GetName()[0] != '\0')
			{
				Info.Name = GameEngineString::AnsiToUTF8Return(geoMetry->GetName());
			}
			else 
			{
				Info.Name = nullptr != geoMetryNode ? GameEngineString::AnsiToUTF8Return(geoMetryNode->GetName()) : "None";
			}

			Info.Mesh = reinterpret_cast<fbxsdk::FbxMesh*>(geoMetry);
			Info.bTriangulated = Info.Mesh->IsTriangleMesh();
			Info.MaterialNum = geoMetryNode ? geoMetryNode->GetMaterialCount() : 0;
			Info.FaceNum = Info.Mesh->GetPolygonCount();
			Info.VertexNum = Info.Mesh->GetControlPointsCount();
			Info.UniqueId = Info.Mesh->GetUniqueID();


			// ���߿� �����Ҽ� ������ ����.
			Info.LODGroup = "";
			if (nullptr != geoMetryNode)
			{
				fbxsdk::FbxNode* ParentNode = RecursiveFindParentLodGroup(geoMetryNode->GetParent());
				if (ParentNode != nullptr && ParentNode->GetNodeAttribute() 
					&& ParentNode->GetNodeAttribute()->GetAttributeType() == fbxsdk::FbxNodeAttribute::eLODGroup)
				{
					Info.LODGroup = GameEngineString::AnsiToUTF8Return(ParentNode->GetName());
					Info.bIsLodGroup = true;
					for (int LODIndex = 0; LODIndex < ParentNode->GetChildCount(); LODIndex++)
					{
						fbxsdk::FbxNode* MeshNode = FindLODGroupNode(ParentNode, LODIndex, geoMetryNode);
						if (geoMetryNode == MeshNode)
						{
							Info.LodLevel = LODIndex;
							break;
						}
					}
				}
			}

			if (Info.Mesh->GetDeformerCount(FbxDeformer::eSkin) > 0)
			{
				Info.bIsSkelMesh = true;
				Info.MorphNum = Info.Mesh->GetShapeCount();
				fbxsdk::FbxSkin* Skin = (fbxsdk::FbxSkin*)Info.Mesh->GetDeformer(0, FbxDeformer::eSkin);
				int ClusterCount = Skin->GetClusterCount();
				fbxsdk::FbxNode* Link = NULL;
				for (int ClusterId = 0; ClusterId < ClusterCount; ++ClusterId)
				{
					fbxsdk::FbxCluster* Cluster = Skin->GetCluster(ClusterId);
					// ���� ����� 
					Link = Cluster->GetLink();
					while (Link && Link->GetParent() && Link->GetParent()->GetSkeleton())
					{
						Link = Link->GetParent();
					}

					if (Link != NULL)
					{
						break;
					}
				}

				Info.SkeletonRoot = nullptr != Link ? GameEngineString::UTF8ToAnsiReturn(Link->GetName()) : "None";
				Info.SkeletonElemNum = nullptr != Link ? Link->GetChildCount(true) : 0;

				//if (nullptr != Link)
				//{
				//	fbxsdk::FbxTimeSpan AnimTimeSpan(fbxsdk::FBXSDK_TIME_INFINITE, fbxsdk::FBXSDK_TIME_MINUS_INFINITE);
				//	Link->GetAnimationInterval(AnimTimeSpan);
				//	GlobalTimeSpan.UnionAssignment(AnimTimeSpan);
				//}
			}
			else
			{
				Info.bIsSkelMesh = false;
				Info.SkeletonRoot = "";
			}
		}
	}
}

void GameEngineFBXMesh::MeshLoad()
{
	// �̰� ���� ���ü� �ִ� ������ �����ѰŰ�
	MeshNodeCheck();
	// ������ ������ �������� ��¥ ������ ����� ������ �̴ϴ�.
	VertexBufferCheck();
}

fbxsdk::FbxAMatrix GameEngineFBXMesh::ComputeTotalMatrix(fbxsdk::FbxNode* Node)
{
	fbxsdk::FbxAMatrix Geometry;
	fbxsdk::FbxVector4 Translation, Rotation, Scaling;
	Translation = Node->GetGeometricTranslation(fbxsdk::FbxNode::eSourcePivot);
	Rotation = Node->GetGeometricRotation(fbxsdk::FbxNode::eSourcePivot);
	Scaling = Node->GetGeometricScaling(fbxsdk::FbxNode::eSourcePivot);
	Geometry.SetT(Translation);
	Geometry.SetR(Rotation);
	Geometry.SetS(Scaling);

	fbxsdk::FbxAMatrix& GlobalTransform = Scene->GetAnimationEvaluator()->GetNodeGlobalTransform(Node);
	return GlobalTransform * Geometry;
}

bool GameEngineFBXMesh::IsOddNegativeScale(const fbxsdk::FbxAMatrix& TotalMatrix)
{
	fbxsdk::FbxVector4 Scale = TotalMatrix.GetS();
	int NegativeNum = 0;

	if (Scale[0] < 0) NegativeNum++;
	if (Scale[1] < 0) NegativeNum++;
	if (Scale[2] < 0) NegativeNum++;

	return NegativeNum == 1 || NegativeNum == 3;
}


void GameEngineFBXMesh::LoadBinormal(fbxsdk::FbxMesh* _Mesh, fbxsdk::FbxAMatrix _MeshMatrix, std::vector<GameEngineVertex>& _ArrVtx, int VtxId, int _Index)
{
	int iCount = _Mesh->GetElementBinormalCount();

	if (0 == iCount)
	{
		return;

	}

	FbxGeometryElementBinormal* pElement = _Mesh->GetElementBinormal();
	int iDataIndex = VtxId;

	if (pElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		if (FbxGeometryElement::eIndexToDirect == pElement->GetReferenceMode())
		{
			iDataIndex = pElement->GetIndexArray().GetAt(VtxId);
		}
		else
		{
			iDataIndex = VtxId;
		}
	}

	else if (pElement->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		if (FbxGeometryElement::eDirect == pElement->GetReferenceMode())
		{
			iDataIndex = _Index;
		}
		else if (FbxGeometryElement::eIndexToDirect == pElement->GetReferenceMode())
		{
			iDataIndex = pElement->GetIndexArray().GetAt(_Index);
		}
		else {
			GameEngineDebug::MsgBoxError("���� ����� ó���Ҽ� ���� ����Դϴ�.");
		}
	}

	FbxVector4 BiNormal = pElement->GetDirectArray().GetAt(iDataIndex);

	fbxsdk::FbxAMatrix conversionMeshMatrix = _MeshMatrix.Transpose();
	BiNormal = conversionMeshMatrix.MultT(BiNormal);


	_ArrVtx[_Index].BINORMAL.x = (float)BiNormal.mData[0];
	_ArrVtx[_Index].BINORMAL.y = (float)BiNormal.mData[1];
	_ArrVtx[_Index].BINORMAL.z = -(float)BiNormal.mData[2];
	_ArrVtx[_Index].BINORMAL.w = (float)BiNormal.mData[3];
	_ArrVtx[_Index].BINORMAL.Normalize3D();
}

void GameEngineFBXMesh::LoadTangent(fbxsdk::FbxMesh* _Mesh, fbxsdk::FbxAMatrix _MeshMatrix, std::vector<GameEngineVertex>& _ArrVtx, int VtxId, int _Index)
{
	int iCount = _Mesh->GetElementTangentCount();

	if (0 == iCount)
	{
		return;
	}
	FbxGeometryElementTangent* pElement = _Mesh->GetElementTangent();
	int iDataIndex = VtxId;

	if (pElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		if (FbxGeometryElement::eIndexToDirect == pElement->GetReferenceMode())
		{
			iDataIndex = pElement->GetIndexArray().GetAt(VtxId);
		}
		else
		{
			iDataIndex = VtxId;
		}
	}
	else if (pElement->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		if (FbxGeometryElement::eDirect == pElement->GetReferenceMode())
		{
			iDataIndex = _Index;
		}
		else if (FbxGeometryElement::eIndexToDirect == pElement->GetReferenceMode())
		{
			iDataIndex = pElement->GetIndexArray().GetAt(_Index);
		}
		else {
			GameEngineDebug::MsgBoxError("���� ����� ó���Ҽ� ���� ����Դϴ�.");
		}
	}

	FbxVector4 Tangent = pElement->GetDirectArray().GetAt(iDataIndex);


	fbxsdk::FbxAMatrix conversionMeshMatrix = _MeshMatrix.Transpose();
	Tangent = conversionMeshMatrix.MultT(Tangent);


	_ArrVtx[_Index].TANGENT.x = (float)Tangent.mData[0];
	_ArrVtx[_Index].TANGENT.y = (float)Tangent.mData[1];
	_ArrVtx[_Index].TANGENT.z = -(float)Tangent.mData[2];
	_ArrVtx[_Index].TANGENT.w = (float)Tangent.mData[3];
	_ArrVtx[_Index].TANGENT.Normalize3D();
}

void GameEngineFBXMesh::LoadNormal(fbxsdk::FbxMesh* _Mesh, fbxsdk::FbxAMatrix _MeshMatrix, std::vector<GameEngineVertex>& _ArrVtx, int VtxId, int _Index)
{
	int iCount = _Mesh->GetElementNormalCount();

	if (0 == iCount)
	{
		GameEngineDebug::MsgBoxError("GetElementNormalCount�� ������ �Դϴ�.");
	}


	FbxGeometryElementNormal* pElement = _Mesh->GetElementNormal();
	int iDataIndex = VtxId;

	if (pElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		if (FbxGeometryElement::eIndexToDirect == pElement->GetReferenceMode())
		{
			iDataIndex = pElement->GetIndexArray().GetAt(VtxId);
		}
		else
		{
			iDataIndex = VtxId;
		}
	}
	else if (pElement->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		if (FbxGeometryElement::eDirect == pElement->GetReferenceMode())
		{
			iDataIndex = _Index;
		}
		else if (FbxGeometryElement::eIndexToDirect == pElement->GetReferenceMode())
		{
			iDataIndex = pElement->GetIndexArray().GetAt(_Index);
		}
		else {
			GameEngineDebug::MsgBoxError("���� ����� ó���Ҽ� ���� ����Դϴ�.");
		}
	}

	FbxVector4 Normal = pElement->GetDirectArray().GetAt(iDataIndex);

	fbxsdk::FbxAMatrix conversionMeshMatrix = _MeshMatrix.Transpose();
	Normal = conversionMeshMatrix.MultT(Normal);

	_ArrVtx[_Index].NORMAL.x = (float)Normal.mData[0];
	_ArrVtx[_Index].NORMAL.y = (float)Normal.mData[1];
	_ArrVtx[_Index].NORMAL.z = -(float)Normal.mData[2];
	_ArrVtx[_Index].NORMAL.w = (float)Normal.mData[3];
	// _ArrVtx[_Index].Normal.w = 0.0f;
	_ArrVtx[_Index].NORMAL.Normalize3D();
}

void GameEngineFBXMesh::DrawSetWeightAndIndexSetting(FbxMeshSet* _DrawSet, fbxsdk::FbxMesh* _Mesh, fbxsdk::FbxCluster* _Cluster, int _BoneIndex)
{
	if (nullptr == _DrawSet)
	{
		return;
	}

	int iIndexCount = _Cluster->GetControlPointIndicesCount();

	for (size_t i = 0; i < iIndexCount; i++)
	{
		FbxExIW IW;
		IW.Index = _BoneIndex;

		IW.Weight = _Cluster->GetControlPointWeights()[i];
		int ControlPointIndices = _Cluster->GetControlPointIndices()[i];

		_DrawSet->MapWI[_Mesh][ControlPointIndices].push_back(IW);
	}
}


void GameEngineFBXMesh::LoadAnimationVertexData(FbxMeshSet* _MeshSet, const std::vector<FbxClusterData>& vecClusterData)
{
	for (auto& clusterData : vecClusterData)
	{
		if (nullptr == clusterData.Cluster->GetLink())
		{
			continue;
		}

		std::string StrBoneName = clusterData.LinkName;
		Bone* pBone = FindBone(_MeshSet->Index, StrBoneName);
		if (nullptr == pBone)
		{
			continue;
		}

		DrawSetWeightAndIndexSetting(_MeshSet, clusterData.Mesh, clusterData.Cluster, pBone->Index);
	}
}


void GameEngineFBXMesh::CalAnimationVertexData(FbxMeshSet& _DrawSet)
{
	for (auto& _WISet : _DrawSet.MapWI)
	{
		std::vector<GameEngineVertex>* Ptr = &_DrawSet.Vertexs;

		if (nullptr == Ptr)
		{
			GameEngineDebug::MsgBoxError("���ؽ� �����Ϳ� ������ ����ġ �����Ͱ� ��Ī���� �ʽ��ϴ�.");
		}

		std::vector<GameEngineVertex>& VertexData = *Ptr;

		for (auto& _WI : _WISet.second)
		{
			double dWeight = 0.0;
			for (int n = 0; n < _WI.second.size(); ++n)
			{
				dWeight += _WI.second[n].Weight;
			}

			for (int n = 0; n < _WI.second.size(); ++n)
			{
				_WI.second[n].Weight /= dWeight;
			}

			if (_WI.second.size() > 4)
			{
				std::sort(_WI.second.begin(), _WI.second.end(),
					[](const FbxExIW& _Left, const FbxExIW& _Right) {
						return _Left.Weight > _Right.Weight;
					}
				);

				double dInterPolate = 0.0;

				std::vector<FbxExIW>::iterator IterErase = _WI.second.begin() + 4;
				for (; IterErase != _WI.second.end(); ++IterErase)
				{
					dInterPolate += IterErase->Weight;
				}
				IterErase = _WI.second.begin() + 4;

				_WI.second.erase(IterErase, _WI.second.end());
				_WI.second[0].Weight += dInterPolate;
			};

			float Weight[4] = { 0.0f };
			int Index[4] = { 0 };

			for (size_t i = 0; i < _WI.second.size(); i++)
			{
				Weight[i] = (float)_WI.second[i].Weight;
				Index[i] = _WI.second[i].Index;
			}

			memcpy_s(VertexData[_WI.first].WEIGHT.Arr1D, sizeof(float4), Weight, sizeof(float4));
			memcpy_s(VertexData[_WI.first].BLENDINDICES, sizeof(float4), Index, sizeof(float4));
		}
	}
}


//
//// �ε����� ����ġ�� �������ִ� �Լ�
void GameEngineFBXMesh::LoadSkinAndCluster()
{
	ImportCluster();

	for (int i = 0; i < AllMeshMap.size(); ++i)
	{
		FbxMeshSet& MeshInfo = AllMeshMap[i];
		std::vector<FbxClusterData>& ClusterInfo = ClusterData[i];

		// Ŭ�����ʹ� ����ġ ������ �ε��� ������ ������ �ִµ�
		// Ŭ�����͸� ���ؼ� ������ ��������
		LoadAnimationVertexData(&MeshInfo, ClusterInfo);

		// ��¥ ����ġ�� ����Ѵ�.
		CalAnimationVertexData(MeshInfo);
	}
}


float4 GameEngineFBXMesh::MaterialColor(fbxsdk::FbxSurfaceMaterial* pMtrl, const char* _ColorName, const char* _FactorName)
{
	FbxDouble3 vResult(0, 0, 0);
	double dFactor = 0;
	FbxProperty ColorPro = pMtrl->FindProperty(_ColorName);
	FbxProperty FactorPro = pMtrl->FindProperty(_FactorName);

	if (true == ColorPro.IsValid() && true == FactorPro.IsValid())
	{
		vResult = ColorPro.Get<FbxDouble3>();
		dFactor = FactorPro.Get<FbxDouble>();

		if (dFactor != 1)
		{
			vResult[0] *= dFactor;
			vResult[1] *= dFactor;
			vResult[2] *= dFactor;
		}
	}

	return float4((float)vResult[0], (float)vResult[1], (float)vResult[2]);
}


float GameEngineFBXMesh::MaterialFactor(fbxsdk::FbxSurfaceMaterial* pMtrl, const char* _FactorName)
{
	double dFactor = 0;
	FbxProperty FactorPro = pMtrl->FindProperty(_FactorName);

	if (true == FactorPro.IsValid())
	{
		dFactor = FactorPro.Get<FbxDouble>();
	}

	return (float)dFactor;
}



std::string GameEngineFBXMesh::MaterialTex(fbxsdk::FbxSurfaceMaterial* pMtrl, const char* _FactorName)
{
	 fbxsdk::FbxProperty TexturePro = pMtrl->FindProperty(_FactorName);

	std::string Str;
	if (true == TexturePro.IsValid())
	{
		fbxsdk::FbxObject* pFileTex = TexturePro.GetFbxObject();
		
		 int iTexCount = TexturePro.GetSrcObjectCount<FbxFileTexture>();

		if (iTexCount > 0)
		{
			FbxFileTexture* pFileTex = TexturePro.GetSrcObject<FbxFileTexture>(0);

			if (nullptr != pFileTex)
			{
				Str = pFileTex->GetFileName();
			}
		}
		else
		{
			return "";
		}
	}
	else
	{
		return "";
	}

	 return Str;
}

void GameEngineFBXMesh::FbxMeshSetMaterialSetting(fbxsdk::FbxNode* _Node, FbxMeshSet* _RenderData)
{
	int MtrlCount = _Node->GetMaterialCount();

	if (MtrlCount > 0)
	{
		_RenderData->MaterialData.push_back(std::vector<FbxExRenderingPipeLineSettingData>());

		std::vector<FbxExRenderingPipeLineSettingData>& MatrialSet = _RenderData->MaterialData[_RenderData->MaterialData.size() - 1];

		for (int i = 0; i < MtrlCount; i++)
		{
			fbxsdk::FbxSurfaceMaterial* pMtrl = _Node->GetMaterial(i);

			if (nullptr == pMtrl)
			{
				GameEngineDebug::MsgBoxError("if (nullptr == pMtrl) ���׸��� ������ �������� �ʽ��ϴ�");
				continue;
			}

			MatrialSet.push_back(FbxExRenderingPipeLineSettingData());
			FbxExRenderingPipeLineSettingData& MatData = MatrialSet[MatrialSet.size() - 1];
			MatData.Name = pMtrl->GetName();
			// fbxsdk::FbxSurfaceMaterial::sDiffuse = 0x00007ff61122bf40 "DiffuseColor"
			// fbxsdk::FbxSurfaceMaterial::sDiffuseFactor = 0x00007ff61122bf50 "DiffuseFactor"
			MatData.DifColor = MaterialColor(pMtrl, "DiffuseColor", "DiffuseFactor");
			MatData.AmbColor = MaterialColor(pMtrl, "AmbientColor", "AmbientFactor");
			MatData.SpcColor = MaterialColor(pMtrl, "SpecularColor", "SpecularFactor");
			MatData.EmvColor = MaterialColor(pMtrl, "EmissiveColor" , "EmissiveFactor");
			MatData.SpecularPower = MaterialFactor(pMtrl, "SpecularFactor");
			// fbxsdk::FbxSurfaceMaterial::sShininess = 0x00007ff61122bf80 "ShininessExponent"
			// fbxsdk::FbxSurfaceMaterial::sTransparencyFactor = 0x00007ff61122bfd8 "TransparencyFactor"
			MatData.Shininess = MaterialFactor(pMtrl, "ShininessExponent");
			MatData.TransparencyFactor = MaterialFactor(pMtrl, "TransparencyFactor");

			MatData.DifTexturePath = MaterialTex(pMtrl, "DiffuseColor");
			// fbxsdk::FbxSurfaceMaterial::sNormalMap = 0x00007ff68291bfa0 "NormalMap"
			MatData.NorTexturePath = MaterialTex(pMtrl, "NormalMap");
			MatData.SpcTexturePath = MaterialTex(pMtrl, "SpecularColor");
		}

	}
	else {
		GameEngineDebug::MsgBoxError("�Ž��� ���������� ������ �������� �ʽ��ϴ�.");
	}

}

//void LoadUVInformation(FbxMesh* pMesh, std::vector<GameEngineVertex>& VtxData)
//{
//
//	//get all UV set names
//	FbxStringList lUVSetNameList;
//	pMesh->GetUVSetNames(lUVSetNameList);
//
//	//iterating over all uv sets
//	for (int lUVSetIndex = 0; lUVSetIndex < lUVSetNameList.GetCount(); lUVSetIndex++)
//	{
//		//get lUVSetIndex-th uv set
//		const char* lUVSetName = lUVSetNameList.GetStringAt(lUVSetIndex);
//		const FbxGeometryElementUV* lUVElement = pMesh->GetElementUV(lUVSetName);
//
//		if (!lUVElement)
//			continue;
//
//		// only support mapping mode eByPolygonVertex and eByControlPoint
//		if (lUVElement->GetMappingMode() != FbxGeometryElement::eByPolygonVertex &&
//			lUVElement->GetMappingMode() != FbxGeometryElement::eByControlPoint)
//			return;
//
//		//index array, where holds the index referenced to the uv data
//		const bool lUseIndex = lUVElement->GetReferenceMode() != FbxGeometryElement::eDirect;
//		const int lIndexCount = (lUseIndex) ? lUVElement->GetIndexArray().GetCount() : 0;
//
//		//iterating through the data by polygon
//		const int lPolyCount = pMesh->GetPolygonCount();
//
//		if (lUVElement->GetMappingMode() == FbxGeometryElement::eByControlPoint)
//		{
//			for (int lPolyIndex = 0; lPolyIndex < lPolyCount; ++lPolyIndex)
//			{
//				// build the max index array that we need to pass into MakePoly
//				const int lPolySize = pMesh->GetPolygonSize(lPolyIndex);
//				for (int lVertIndex = 0; lVertIndex < lPolySize; ++lVertIndex)
//				{
//					FbxVector2 lUVValue;
//
//					//get the index of the current vertex in control points array
//					int lPolyVertIndex = pMesh->GetPolygonVertex(lPolyIndex, lVertIndex);
//
//					//the UV index depends on the reference mode
//					int lUVIndex = lUseIndex ? lUVElement->GetIndexArray().GetAt(lPolyVertIndex) : lPolyVertIndex;
//
//					lUVValue = lUVElement->GetDirectArray().GetAt(lUVIndex);
//
//					VtxData[lUVIndex].TEXTURECOORD.x = static_cast<float>(lUVValue.mData[0]);
//					VtxData[lUVIndex].TEXTURECOORD.y = static_cast<float>(lUVValue.mData[1]);
//
//					//User TODO:
//					//Print out the value of UV(lUVValue) or log it to a file
//				}
//			}
//		}
//		else if (lUVElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
//		{
//			// GameEngineDebug::AssertFalse();
//
//			int lPolyIndexCounter = 0;
//			for (int lPolyIndex = 0; lPolyIndex < lPolyCount; ++lPolyIndex)
//			{
//				// build the max index array that we need to pass into MakePoly
//				const int lPolySize = pMesh->GetPolygonSize(lPolyIndex);
//				for (int lVertIndex = 0; lVertIndex < lPolySize; ++lVertIndex)
//				{
//					if (lPolyIndexCounter < lIndexCount)
//					{
//						FbxVector2 lUVValue;
//
//						//the UV index depends on the reference mode
//						int lUVIndex = lUseIndex ? lUVElement->GetIndexArray().GetAt(lPolyIndexCounter) : lPolyIndexCounter;
//
//						lUVValue = lUVElement->GetDirectArray().GetAt(lUVIndex);
//
//						VtxData[lUVIndex].TEXTURECOORD.x = static_cast<float>(lUVValue.mData[0]);
//						VtxData[lUVIndex].TEXTURECOORD.y = 1.0f - static_cast<float>(lUVValue.mData[1]);
//
//						//User TODO:
//						//Print out the value of UV(lUVValue) or log it to a file
//
//						lPolyIndexCounter++;
//					}
//				}
//			}
//		}
//	}
//}

void GameEngineFBXMesh::LoadUv(fbxsdk::FbxMesh* _Mesh, fbxsdk::FbxAMatrix _MeshMatrix, std::vector<GameEngineVertex>& _ArrVtx, int VtxId, int VertexCount,int _Index)
{
	int iCount = _Mesh->GetElementUVCount();

	if (0 == iCount)
	{
		return;

	}

	float4 result;

	FbxGeometryElementUV* pElement = _Mesh->GetElementUV();
	int iDataIndex = VtxId;
	switch (pElement->GetMappingMode())
		//switch (vertexTangnet->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
		switch (pElement->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			result.x = static_cast<float>(pElement->GetDirectArray().GetAt(_Index).mData[0]);
			result.y = static_cast<float>(pElement->GetDirectArray().GetAt(_Index).mData[1]);
			//result.z = static_cast<float>(pElement->GetDirectArray().GetAt(_Index).mData[2]);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int index = pElement->GetIndexArray().GetAt(_Index);
			result.x = static_cast<float>(pElement->GetDirectArray().GetAt(index).mData[0]);
			result.y = static_cast<float>(pElement->GetDirectArray().GetAt(index).mData[1]);
			//result.z = static_cast<float>(pElement->GetDirectArray().GetAt(index).mData[2]);
		}
		break;
		default:
		{
		}
			break;
		}
		break;

	case FbxGeometryElement::eByPolygonVertex:
		switch (pElement->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			result.x = static_cast<float>(pElement->GetDirectArray().GetAt(VtxId).mData[0]);
			result.y = static_cast<float>(pElement->GetDirectArray().GetAt(VtxId).mData[1]);
			//result.z = static_cast<float>(pElement->GetDirectArray().GetAt(VtxId).mData[2]);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int index = pElement->GetIndexArray().GetAt(VertexCount);
			result.x = static_cast<float>(pElement->GetDirectArray().GetAt(index).mData[0]);
			result.y = static_cast<float>(pElement->GetDirectArray().GetAt(index).mData[1]);
		//	result.z = static_cast<float>(pElement->GetDirectArray().GetAt(index).mData[2]);
		}
		break;
		default:
		{
		}
			break;
		}
		break;
	}


	_ArrVtx[_Index].TEXTURECOORD.x = (float)result.x;
	_ArrVtx[_Index].TEXTURECOORD.y = 1.0f - (float)result.y;
}

void GameEngineFBXMesh::VertexBufferCheck()
{
	int meshInfoSize = static_cast<int>(MeshInfos.size());
	for (int meshInfoIndex = 0; meshInfoIndex < meshInfoSize; ++meshInfoIndex)
	{
		FbxExMeshInfo& meshInfo = MeshInfos.at(meshInfoIndex);
		fbxsdk::FbxNode* pMeshNode = meshInfo.Mesh->GetNode();
		fbxsdk::FbxMesh* pMesh = meshInfo.Mesh;

		FbxMeshSet& DrawMesh = AllMeshMap.emplace_back();
		DrawMesh.Index = meshInfoIndex;

		if (DrawMesh.MapWI.end() == DrawMesh.MapWI.find(pMesh))
		{
			DrawMesh.MapWI.insert(std::make_pair(pMesh, std::map<int, std::vector<FbxExIW>>()));
		}

		DrawMesh.IsLod = meshInfo.bIsLodGroup;
		DrawMesh.IsLodLv = meshInfo.LodLevel;
		//DrawMesh.Vertexs.push_back(std::vector<GameEngineVertex>());
		//DrawMesh.Indexs.push_back(std::vector<std::vector<UINT>>());
		std::vector<GameEngineVertex>& VtxData = DrawMesh.Vertexs;
		std::vector<std::vector<UINT>>& IdxData = DrawMesh.Indexs.emplace_back();

		int controlPointsCount = pMesh->GetControlPointsCount();

		// �̰� fbx�ȿ� ����ִ� ������ ������
		fbxsdk::FbxVector4* pControlPoints = pMesh->GetControlPoints();

		VtxData.resize(controlPointsCount);

		fbxsdk::FbxAMatrix meshMatrix = ConvertMatrix;

		meshMatrix = ComputeTotalMatrix(pMeshNode);

		if (false == meshInfo.bIsSkelMesh)
		{
			meshMatrix = JointMatrix * meshMatrix;
		}

		// ũ�Ⱑ -�ΰ� �ִ��� Ȯ��
		bool isOddNegativeScale = IsOddNegativeScale(meshMatrix);

		// �������� ���´�.
		for (int controlPointIndex = 0; controlPointIndex < controlPointsCount; ++controlPointIndex)
		{
			fbxsdk::FbxVector4 controlPoint = pControlPoints[controlPointIndex];
			// VtxData[controlPointIndex].Pos *= 2.0F;
			fbxsdk::FbxVector4 calculateControlPoint = meshMatrix.MultT(controlPoint);
			VtxData[controlPointIndex].POSITION = FbxVecToTransform(calculateControlPoint);
			VtxData[controlPointIndex].POSITION.w = 1.0f;
		}

		// 
		FbxMeshSetMaterialSetting(pMeshNode, &DrawMesh);

		// LoadUVInformation(pMesh, VtxData);




		//fbxsdk::FbxStringList UVSetNameList;
		//pMesh->GetUVSetNames(UVSetNameList);
		//int uvSetCount = UVSetNameList.GetCount();
		//for (int uvSetIndex = 0; uvSetIndex < uvSetCount; ++uvSetIndex)
		//{
		//	const char* uvSetName = UVSetNameList.GetStringAt(uvSetIndex);
		//	const fbxsdk::FbxGeometryElementUV* pUVElement = pMesh->GetElementUV(uvSetName);
		//	if (nullptr == pUVElement)
		//	{
		//		continue;
		//	}

		//	fbxsdk::FbxGeometryElement::EMappingMode eMappingMode = pUVElement->GetMappingMode();
		//	fbxsdk::FbxGeometryElement::EReferenceMode eReferenceMode = pUVElement->GetReferenceMode();
		//	bool useIndex = fbxsdk::FbxGeometryElement::EReferenceMode::eDirect != eReferenceMode;
		//	int IndexCount = true == useIndex ? pUVElement->GetIndexArray().GetCount() : 0;

		//	int nPolygonCount = pMesh->GetPolygonCount();
		//	for (int PolygonIndex = 0; PolygonIndex < nPolygonCount; ++PolygonIndex)
		//	{
		//		const int PolygonSize = pMesh->GetPolygonSize(PolygonIndex);
		//		if (3 != PolygonSize)
		//		{
		//			GameEngineDebug::MsgBoxError("�ﰢ���� �ƴ� ���� �߰߉���ϴ�.");
		//		}

		//		for (int PositionInPolygon = 0; PositionInPolygon < PolygonSize; ++PositionInPolygon)
		//		{
		//			int convertUvIndex = isOddNegativeScale ? 2 - PositionInPolygon : PositionInPolygon;
		//			int ControlPointIndex = pMesh->GetPolygonVertex(PolygonIndex, PositionInPolygon);
		//			int UVMapIndex = (fbxsdk::FbxGeometryElement::EMappingMode::eByControlPoint == eMappingMode) ?
		//				ControlPointIndex : PolygonIndex * 3 + PositionInPolygon;

		//			int UvIndex = useIndex ? pUVElement->GetIndexArray().GetAt(UVMapIndex) : UVMapIndex;
		//			fbxsdk::FbxVector2 uvValue = pUVElement->GetDirectArray().GetAt(UvIndex);

		//			VtxData[ControlPointIndex].TEXTURECOORD.x = static_cast<float>(uvValue.mData[0]);
		//			VtxData[ControlPointIndex].TEXTURECOORD.y = static_cast<float>(uvValue.mData[1]);

		//			// VtxData[ControlPointIndex].TEXTURECOORD.x = static_cast<float>(uvValue.mData[0]);
		//			// VtxData[ControlPointIndex].TEXTURECOORD.y = 1.f - static_cast<float>(uvValue.mData[1]);
		//		}
		//	}
		//}

		pMesh->GetElementMaterialCount();
		fbxsdk::FbxGeometryElementMaterial* pGeometryElementMaterial = pMesh->GetElementMaterial();
		fbxsdk::FbxGeometryElementNormal* pGeometryElementNormal = pMesh->GetElementNormal();

		// pMeshNode->GetMaterialIndex()
		int materialCount = pMeshNode->GetMaterialCount();
		IdxData.resize(materialCount);

		UINT VtxId = 0;

		int nPolygonCount = pMesh->GetPolygonCount();
		for (int PolygonIndex = 0; PolygonIndex < nPolygonCount; ++PolygonIndex)
		{
			int PolygonSize = pMesh->GetPolygonSize(PolygonIndex);
			if (3 != PolygonSize)
			{
				GameEngineDebug::MsgBoxError("�ﰢ���� �ƴ� ���� �߰߉���ϴ�.");
			}

			int IndexArray[3] = { 0, };
			for (int PositionInPolygon = 0; PositionInPolygon < PolygonSize; ++PositionInPolygon)
			{
				int ControlPointIndex = pMesh->GetPolygonVertex(PolygonIndex, PositionInPolygon);
				IndexArray[PositionInPolygon] = ControlPointIndex;


				LoadNormal(pMesh, meshMatrix, VtxData, VtxId, ControlPointIndex);
				LoadTangent(pMesh, meshMatrix, VtxData, VtxId, ControlPointIndex);
				LoadBinormal(pMesh, meshMatrix, VtxData, VtxId, ControlPointIndex);
				LoadUv(pMesh, meshMatrix, VtxData, pMesh->GetTextureUVIndex(PolygonIndex, PositionInPolygon), VtxId, ControlPointIndex);

				++VtxId;
			}

			int materialId = pGeometryElementMaterial->GetIndexArray().GetAt(PolygonIndex);
			IdxData[materialId].push_back(IndexArray[0]);
			IdxData[materialId].push_back(IndexArray[2]);
			IdxData[materialId].push_back(IndexArray[1]);
		}

		DrawMesh.FbxVertexMap.insert(std::make_pair(pMesh, &VtxData));
	}

	MeshInfos;
	AllMeshMap;

	int a = 0;

}

void GameEngineFBXMesh::CreateRenderingBuffer()
{
	CreateVertexBuffer();
	CreateIndexBuffer();
}

void GameEngineFBXMesh::CreateVertexBuffer()
{
	for (auto& Data : AllMeshMap)
	{
		//for (size_t i = 0; i < Data.Vertexs.size(); i++)
		//{
			GameEngineVertexBuffer* NewRes = new GameEngineVertexBuffer();
			NewRes->Create(Data.Vertexs, D3D11_USAGE::D3D11_USAGE_DEFAULT);
			Data.GameEngineVertexBuffers.push_back(NewRes);
		//}
	}
}

void GameEngineFBXMesh::CreateIndexBuffer()
{
	for (auto& Data : AllMeshMap)
	{
		for (size_t i = 0; i < Data.Indexs.size(); i++)
		{
			Data.GameEngineIndexBuffers.push_back(std::vector<GameEngineIndexBuffer*>());
			for (size_t j = 0; j < Data.Indexs[i].size(); j++)
			{
				GameEngineIndexBuffer* NewRes = new GameEngineIndexBuffer();
				NewRes->Create(Data.Indexs[i][j], D3D11_USAGE::D3D11_USAGE_DEFAULT);
				Data.GameEngineIndexBuffers[i].push_back(NewRes);
			}
		}
	}
}


bool GameEngineFBXMesh::RetrievePoseFromBindPose(fbxsdk::FbxScene* pScene, const std::vector<fbxsdk::FbxNode*>& NodeArray, fbxsdk::FbxArray<fbxsdk::FbxPose*>& PoseArray)
{
	const int PoseCount = pScene->GetPoseCount();
	for (int PoseIndex = 0; PoseIndex < PoseCount; PoseIndex++)
	{
		fbxsdk::FbxPose* CurrentPose = pScene->GetPose(PoseIndex);

		if (CurrentPose && CurrentPose->IsBindPose())
		{
			std::string PoseName = CurrentPose->GetName();
			fbxsdk::FbxStatus Status;

			for (auto Current : NodeArray)
			{
				std::string CurrentName = Current->GetName();
				fbxsdk::NodeList pMissingAncestors, pMissingDeformers, pMissingDeformersAncestors, pWrongMatrices;

				if (CurrentPose->IsValidBindPoseVerbose(Current, pMissingAncestors, pMissingDeformers, pMissingDeformersAncestors, pWrongMatrices, 0.0001, &Status))
				{
					PoseArray.Add(CurrentPose);
					break;
				}
				else
				{
					for (int i = 0; i < pMissingAncestors.GetCount(); i++)
					{
						fbxsdk::FbxAMatrix mat = pMissingAncestors.GetAt(i)->EvaluateGlobalTransform(FBXSDK_TIME_ZERO);
						CurrentPose->Add(pMissingAncestors.GetAt(i), mat);
					}

					pMissingAncestors.Clear();
					pMissingDeformers.Clear();
					pMissingDeformersAncestors.Clear();
					pWrongMatrices.Clear();

					if (CurrentPose->IsValidBindPose(Current))
					{
						PoseArray.Add(CurrentPose);
						break;
					}
					else
					{
						fbxsdk::FbxNode* ParentNode = Current->GetParent();
						while (ParentNode)
						{
							fbxsdk::FbxNodeAttribute* Attr = ParentNode->GetNodeAttribute();
							if (Attr && Attr->GetAttributeType() == fbxsdk::FbxNodeAttribute::eNull)
							{
								break;
							}

							ParentNode = ParentNode->GetParent();
						}

						if (ParentNode && CurrentPose->IsValidBindPose(ParentNode))
						{
							PoseArray.Add(CurrentPose);
							break;
						}
						else
						{
							std::string ErrorString = Status.GetErrorString();
							std::string CurrentName = Current->GetName();

							GameEngineDebug::MsgBoxError(ErrorString + "_" + CurrentName);
						}
					}
				}
			}
		}
	}

	return (PoseArray.Size() > 0);
}



fbxsdk::FbxNode* GameEngineFBXMesh::GetRootSkeleton(fbxsdk::FbxScene* pScene, fbxsdk::FbxNode* Link)
{
	fbxsdk::FbxNode* RootBone = Link;

	while (RootBone && RootBone->GetParent())
	{
		bool bIsBlenderArmatureBone = false;

		fbxsdk::FbxNodeAttribute* Attr = RootBone->GetParent()->GetNodeAttribute();
		if (Attr &&
			(Attr->GetAttributeType() == fbxsdk::FbxNodeAttribute::eMesh ||
				(Attr->GetAttributeType() == fbxsdk::FbxNodeAttribute::eNull && !bIsBlenderArmatureBone) ||
				Attr->GetAttributeType() == fbxsdk::FbxNodeAttribute::eSkeleton) &&
			RootBone->GetParent() != pScene->GetRootNode())
		{
			if (Attr->GetAttributeType() == fbxsdk::FbxNodeAttribute::eMesh)
			{
				fbxsdk::FbxMesh* Mesh = (fbxsdk::FbxMesh*)Attr;
				if (Mesh->GetDeformerCount(fbxsdk::FbxDeformer::eSkin) > 0)
				{
					break;
				}
			}

			RootBone = RootBone->GetParent();
		}
		else
		{
			break;
		}
	}

	return RootBone;
}



bool GameEngineFBXMesh::IsBone(fbxsdk::FbxNode* Link)
{
	fbxsdk::FbxNodeAttribute* Attr = Link->GetNodeAttribute();
	if (Attr)
	{
		fbxsdk::FbxNodeAttribute::EType AttrType = Attr->GetAttributeType();
		if (AttrType == fbxsdk::FbxNodeAttribute::eSkeleton ||
			AttrType == fbxsdk::FbxNodeAttribute::eMesh ||
			AttrType == fbxsdk::FbxNodeAttribute::eNull)
		{
			return true;
		}
	}

	return false;
}

bool GameEngineFBXMesh::IsNull(fbxsdk::FbxNode* Link)
{
	fbxsdk::FbxNodeAttribute* Attr = Link->GetNodeAttribute();
	if (Attr)
	{
		fbxsdk::FbxNodeAttribute::EType AttrType = Attr->GetAttributeType();
		if (AttrType == fbxsdk::FbxNodeAttribute::eNull)
		{
			return true;
		}
	}
	return false;
}


void GameEngineFBXMesh::RecursiveBuildSkeleton(fbxsdk::FbxNode* Link, std::vector<fbxsdk::FbxNode*>& OutSortedLinks)
{
	if (IsBone(Link))
	{
		if (false == IsNull(Link))
		{
			OutSortedLinks.push_back(Link);
		}
		int ChildIndex;
		for (ChildIndex = 0; ChildIndex < Link->GetChildCount(); ChildIndex++)
		{
			RecursiveBuildSkeleton(Link->GetChild(ChildIndex), OutSortedLinks);
		}
	}
}



void GameEngineFBXMesh::BuildSkeletonSystem(fbxsdk::FbxScene* pScene, std::vector<fbxsdk::FbxCluster*>& ClusterArray, std::vector<fbxsdk::FbxNode*>& OutSortedLinks)
{
	fbxsdk::FbxNode* Link;
	std::vector<fbxsdk::FbxNode*> RootLinks;
	size_t ClusterIndex;
	for (ClusterIndex = 0; ClusterIndex < ClusterArray.size(); ClusterIndex++)
	{
		Link = ClusterArray[ClusterIndex]->GetLink();
		if (Link)
		{
			Link = GetRootSkeleton(pScene, Link);
			size_t LinkIndex;
			for (LinkIndex = 0; LinkIndex < RootLinks.size(); LinkIndex++)
			{
				if (Link == RootLinks[static_cast<int>(LinkIndex)])
				{
					break;
				}
			}

			if (LinkIndex == RootLinks.size())
			{
				RootLinks.push_back(Link);
			}
		}
	}

	for (size_t LinkIndex = 0; LinkIndex < RootLinks.size(); LinkIndex++)
	{
		RecursiveBuildSkeleton(RootLinks[LinkIndex], OutSortedLinks);
	}
}


Bone* GameEngineFBXMesh::FindBone(int NodeIndex, std::string _Name)
{
	if (0 == AllBones.size())
	{
		ImportBone();
	}

	if (0 == AllBones[NodeIndex].size())
	{
		return nullptr;
	}

	if (0 == AllFindMap[NodeIndex].size())
	{
		GameEngineDebug::MsgBoxError("���� ã�� �۾��� ���� ���� �Ž��Դϴ�");
	}

	if (AllFindMap[NodeIndex].end() == AllFindMap[NodeIndex].find(_Name))
	{
		return nullptr;
	}

	return AllFindMap[NodeIndex][_Name];
}

Bone* GameEngineFBXMesh::FindBone(int MeshIndex, int _BoneIndex)
{
	// m_vecRefBones ���ͷ� ��� �ִ¾�

	if (AllBones.size() <= MeshIndex)
	{
		GameEngineDebug::MsgBoxError("�������� �ʴ� �Ž��� ���� ���������� �߽��ϴ�.");
		return nullptr;
	}

	if (AllBones[MeshIndex].size() <= _BoneIndex)
	{
		GameEngineDebug::MsgBoxError("�����ϴ� ���� ������ �Ѱ���ϴ�.");
		return nullptr;
	}

	return &AllBones[MeshIndex][_BoneIndex];
}


// ���� ������ �������°��̱� ������ 
bool GameEngineFBXMesh::ImportBone()
{
	if (0 != AllBones.size())
	{
		return false;
	}
	

	size_t meshCount = MeshInfos.size();
	if (0 == meshCount)
	{
		return false;
	}

	// �̰� ��Ű�� ���� ������ �ִ� �༮.
	std::vector<fbxsdk::FbxNode*> NodeArray;
	// ��Ű�� ���� ������ �� ����ִ�. �̰� ������ ���� �Լ�
	std::vector<std::vector<fbxsdk::FbxCluster*>> ClusterArray;
	fbxsdk::FbxNode* Link = nullptr;
	int SkelType = 0;

	ClusterArray.resize(MeshInfos.size());

	for (size_t meshindex = 0; meshindex < meshCount; ++meshindex)
	{

		FbxExMeshInfo& meshInfo = MeshInfos.at(meshindex);

		fbxsdk::FbxNode* pNode = meshInfo.Mesh->GetNode();
		fbxsdk::FbxMesh* FbxMesh = meshInfo.Mesh;

		NodeArray.push_back(pNode);
		const int SkinDeformerCount = FbxMesh->GetDeformerCount(fbxsdk::FbxDeformer::eSkin);
		for (int DeformerIndex = 0; DeformerIndex < SkinDeformerCount; DeformerIndex++)
		{
			fbxsdk::FbxSkin* Skin = (fbxsdk::FbxSkin*)FbxMesh->GetDeformer(DeformerIndex, fbxsdk::FbxDeformer::eSkin);
			for (int ClusterIndex = 0; ClusterIndex < Skin->GetClusterCount(); ClusterIndex++)
			{
				ClusterArray[meshindex].push_back(Skin->GetCluster(ClusterIndex));
			}
		}
	}

	for (size_t Clusterindex = 0; Clusterindex < ClusterArray.size(); Clusterindex++)
	{
		AllBones.emplace_back();
		std::map<std::string, Bone*>& FindMap = AllFindMap.emplace_back();

		if (0 == ClusterArray[Clusterindex].size())
		{
			continue;
		}

		std::vector<fbxsdk::FbxNode*> SortedLinks;

		fbxsdk::FbxNode* SkeletalMeshNode = nullptr;
		SkeletalMeshNode = NodeArray[Clusterindex];

		fbxsdk::PoseList PoseArray;

		if (RetrievePoseFromBindPose(Scene, NodeArray, PoseArray) == false)
		{
			const int PoseCount = Scene->GetPoseCount();
			for (int PoseIndex = PoseCount - 1; PoseIndex >= 0; --PoseIndex)
			{
				fbxsdk::FbxPose* CurrentPose = Scene->GetPose(PoseIndex);

				if (CurrentPose && CurrentPose->IsBindPose())
				{
					Scene->RemovePose(PoseIndex);
					CurrentPose->Destroy();
				}
			}

			Manager->CreateMissingBindPoses(Scene);
			if (RetrievePoseFromBindPose(Scene, NodeArray, PoseArray) == false)
			{
				GameEngineDebug::MsgBoxError("Recreating bind pose failed.");
			}
			else
			{
			}
		}

		BuildSkeletonSystem(Scene, ClusterArray[Clusterindex], SortedLinks);

		if (SortedLinks.size() == 0)
		{
			// GameEngineDebug::MsgBoxError("����� ��ũ�� ����.");
			continue;
		}

		std::map<fbxsdk::FbxString, int> m_NameBoneCheck;
		size_t LinkIndex;

		for (LinkIndex = 0; LinkIndex < SortedLinks.size(); ++LinkIndex)
		{
			Link = SortedLinks[LinkIndex];
			m_NameBoneCheck.insert(std::make_pair(Link->GetName(), 0));

			for (size_t AltLinkIndex = LinkIndex + 1; AltLinkIndex < SortedLinks.size(); ++AltLinkIndex)
			{
				fbxsdk::FbxNode* AltLink = SortedLinks[AltLinkIndex];

				fbxsdk::FbxString tempLinkName = Link->GetName();
				fbxsdk::FbxString tempAltLinkName = AltLink->GetName();

				std::map<fbxsdk::FbxString, int>::iterator FindBoneNameIter = m_NameBoneCheck.find(tempAltLinkName);

				// ���� �̸��� ���� �־
				// ���� ���� ��ġ�� �̸��� �������ش�.
				if (FindBoneNameIter != m_NameBoneCheck.end())
				{
					fbxsdk::FbxString newName = FindBoneNameIter->first;
					newName += "_";
					newName += (++FindBoneNameIter->second);

					// ���࿡ �ٲ�µ��� ��ģ �� �־�
					std::map<fbxsdk::FbxString, int>::iterator RevertIter = m_NameBoneCheck.find(newName);

					while (RevertIter != m_NameBoneCheck.find(newName))
					{
						newName = FindBoneNameIter->first;
						newName += "_";
						newName += (++FindBoneNameIter->second);
						RevertIter = m_NameBoneCheck.find(newName);
					}

					// ��ġ�� �̸��� ��ũ�� �� ���̸��� �ٲٸ�
					// ������ ������ ���� ���õ� ��������
					// �� �̸����� ���ɰ��̹Ƿ� ������ �ʿ䰡 ��������.
					AltLink->SetName(newName.Buffer());

					// GameEngineDebug::OutPutMsg(tempLinkName + L"  " + tempAltLinkName);
					// GameEngineDebug::AssertMsg(L"���� ��ũ");
				}
			}
		}

		fbxsdk::FbxArray<fbxsdk::FbxAMatrix> GlobalsPerLink;
		GlobalsPerLink.Grow(static_cast<int>(SortedLinks.size()));
		GlobalsPerLink[0] = ConvertMatrix;

		bool GlobalLinkFoundFlag;
		fbxsdk::FbxVector4 LocalLinkT;
		fbxsdk::FbxQuaternion LocalLinkQ;
		fbxsdk::FbxVector4	LocalLinkS;
		fbxsdk::FbxVector4 GlobalLinkT;
		fbxsdk::FbxQuaternion GlobalLinkQ;
		fbxsdk::FbxVector4	GlobalLinkS;

		bool bAnyLinksNotInBindPose = false;
		std::string LinksWithoutBindPoses;
		int NumberOfRoot = 0;

		int RootIdx = -1;

		for (LinkIndex = 0; LinkIndex < SortedLinks.size(); LinkIndex++)
		{
			Bone& tempBoneData = AllBones[Clusterindex].emplace_back();
			// Bone& tempBoneData = m_vecRefBones.at(m_vecRefBones.size() - 1);
			tempBoneData.Index = static_cast<int>(AllBones[Clusterindex].size() - 1);

			Link = SortedLinks[LinkIndex];

			int ParentIndex = -1;
			fbxsdk::FbxNode* LinkParent = Link->GetParent();
			if (LinkIndex)
			{
				for (int ll = 0; ll < LinkIndex; ++ll)
				{
					fbxsdk::FbxNode* Otherlink = SortedLinks[ll];
					if (Otherlink == LinkParent)
					{
						ParentIndex = ll;
						break;
					}
				}
			}

			if (ParentIndex == -1)
			{
				++NumberOfRoot;
				RootIdx = static_cast<int>(LinkIndex);
				//	unreal ������ ��Ʈ�� �ϳ��� ��������� 
				//	����Ƽ������ �������� ����ؼ� ���� �ڵ带 ������
				/*if (NumberOfRoot > 1)
				{
					AMSG(L"�������� ��Ʈ");
				}*/
			}

			GlobalLinkFoundFlag = false;
			if (!SkelType)
			{
				if (PoseArray.GetCount())
				{
					for (int PoseIndex = 0; PoseIndex < PoseArray.GetCount(); PoseIndex++)
					{
						int PoseLinkIndex = PoseArray[PoseIndex]->Find(Link);
						if (PoseLinkIndex >= 0)
						{
							fbxsdk::FbxMatrix NoneAffineMatrix = PoseArray[PoseIndex]->GetMatrix(PoseLinkIndex);
							fbxsdk::FbxAMatrix Matrix = *(fbxsdk::FbxAMatrix*)(double*)&NoneAffineMatrix;
							GlobalsPerLink[static_cast<int>(LinkIndex)] = Matrix;
							GlobalLinkFoundFlag = true;
							break;
						}
					}
				}

				if (!GlobalLinkFoundFlag)
				{
					for (int ClusterIndex = 0; ClusterIndex < ClusterArray.size(); ClusterIndex++)
					{
						fbxsdk::FbxCluster* Cluster = ClusterArray[0][ClusterIndex];
						if (Link == Cluster->GetLink())
						{
							Cluster->GetTransformLinkMatrix(GlobalsPerLink[static_cast<int>(LinkIndex)]);
							GlobalLinkFoundFlag = true;
							break;
						}
					}
				}
			}

			if (!GlobalLinkFoundFlag)
			{
				GlobalsPerLink[static_cast<int>(LinkIndex)] = Link->EvaluateGlobalTransform();
			}

			GlobalsPerLink[static_cast<int>(LinkIndex)] = GlobalsPerLink[static_cast<int>(LinkIndex)];
			if (LinkIndex &&
				-1 != ParentIndex)
			{
				fbxsdk::FbxAMatrix	Matrix;
				Matrix = GlobalsPerLink[static_cast<int>(ParentIndex)].Inverse() * GlobalsPerLink[static_cast<int>(LinkIndex)];
				LocalLinkT = Matrix.GetT();
				LocalLinkQ = Matrix.GetQ();
				LocalLinkS = Matrix.GetS();
				GlobalLinkT = GlobalsPerLink[static_cast<int>(LinkIndex)].GetT();
				GlobalLinkQ = GlobalsPerLink[static_cast<int>(LinkIndex)].GetQ();
				GlobalLinkS = GlobalsPerLink[static_cast<int>(LinkIndex)].GetS();
			}
			else
			{
				GlobalLinkT = LocalLinkT = GlobalsPerLink[static_cast<int>(LinkIndex)].GetT();
				GlobalLinkQ = LocalLinkQ = GlobalsPerLink[static_cast<int>(LinkIndex)].GetQ();
				GlobalLinkS = LocalLinkS = GlobalsPerLink[static_cast<int>(LinkIndex)].GetS();
			}

			Bone& Bone = AllBones[Clusterindex][static_cast<int>(LinkIndex)];

			Bone.Name = Link->GetName();

			JointPos& BonePosData = Bone.BonePos;
			fbxsdk::FbxSkeleton* Skeleton = Link->GetSkeleton();
			if (Skeleton)
			{
				BonePosData.Length = (float)Skeleton->LimbLength.Get();
				BonePosData.XSize = (float)Skeleton->Size.Get();
				BonePosData.YSize = (float)Skeleton->Size.Get();
				BonePosData.ZSize = (float)Skeleton->Size.Get();
			}
			else
			{
				BonePosData.Length = 1.;
				BonePosData.XSize = 100.;
				BonePosData.YSize = 100.;
				BonePosData.ZSize = 100.;
			}

			Bone.ParentIndex = ParentIndex;
			Bone.NumChildren = 0;
			for (int ChildIndex = 0; ChildIndex < Link->GetChildCount(); ChildIndex++)
			{
				fbxsdk::FbxNode* Child = Link->GetChild(ChildIndex);
				if (IsBone(Child))
				{
					Bone.NumChildren++;
				}
			}

			BonePosData.SetTranslation(LocalLinkT);
			BonePosData.SetRotation(LocalLinkQ);
			BonePosData.SetScale(LocalLinkS);
			BonePosData.SetGlobalTranslation(GlobalLinkT);
			BonePosData.SetGlobalRotation(GlobalLinkQ);
			BonePosData.SetGlobalScale(GlobalLinkS);
			BonePosData.BuildMatrix();
		}


		for (size_t i = 0; i < AllBones[Clusterindex].size(); i++)
		{
			if (FindMap.end() == FindMap.find(AllBones[Clusterindex][i].Name))
			{
				FindMap.insert(std::make_pair(AllBones[Clusterindex][i].Name, &AllBones[Clusterindex][i]));
				continue;
			}

			std::multimap<std::string, Bone*>::iterator it, itlow, itup;

			itlow = FindMap.lower_bound(AllBones[Clusterindex][i].Name);  // itlow points to b
			itup = FindMap.upper_bound(AllBones[Clusterindex][i].Name);   // itup points to e (not d)

			int Count = 0;
			for (it = itlow; it != itup; ++it)
			{
				++Count;
			}

			std::string Name = AllBones[Clusterindex][i].Name + std::to_string(Count);
			FindMap.insert(std::make_pair(Name, &AllBones[Clusterindex][i]));
		}


	}

	CreateAnimationBuffer();

	LoadSkinAndCluster();

	return true;
}

void GameEngineFBXMesh::CreateAnimationBuffer()
{
	AnimationBuffers.resize(AllBones.size());

	for (size_t i = 0; i < AllBones.size(); i++)
	{
		if (0 == AllBones[i].size())
		{
			AnimationBuffers[i] = nullptr;
			continue;
		}

		AnimationBuffers[i] = new GameEngineStructuredBuffer();
		AnimationBuffers[i]->Create(sizeof(float4x4), static_cast<UINT>(AllBones[i].size()), nullptr);
	}
}

void GameEngineFBXMesh::UserSave(const std::string& _Path)
{
	// ������ ����ȭ�� �ϰ� �ִ°̴ϴ�.
	// int Hp;
	// int Def;
	// float ���ҷ�; Def / 2;
	// �װ� ������ ������ ������ �ʿ䰡 ���ݾ�.

	// ������ ����ȭ�� �ϰ� �ִ°̴ϴ�.
	// int Hp;
	// int Def;
	// void Damage(int Damge) 
	// {
	//    float ���ҷ� = Def / 2;
	//    Hp -= Damge * ���ҷ�;
	// }

	GameEngineFile NewFile = GameEngineFile(_Path, "wb");
	// NewFile.Open("wb");
	NewFile.Write(static_cast<int>(AllMeshMap.size()));
	for (auto& Data : AllMeshMap)
	{
		NewFile.Write(Data.MaterialData);
		NewFile.Write(Data.Vertexs);
		NewFile.Write(Data.Indexs);
	}

	NewFile.Write(AllBones);
}

void GameEngineFBXMesh::UserLoad(const std::string& _Path)
{
	AllMeshMap.clear();

	GameEngineFile NewFile = GameEngineFile(_Path, "rb");

	int Size = 0;

	NewFile.Read(Size);

	if (0 == Size)
	{
		GameEngineDebug::MsgBoxError("�Ž��� ���� �����Ͱ� �������� �ʴ� �Ž� �Դϴ�.");
	}

	AllMeshMap.resize(Size);

	for (size_t i = 0; i < Size; i++)
	{
		NewFile.Read(AllMeshMap[i].MaterialData);
		NewFile.Read(AllMeshMap[i].Vertexs);
		NewFile.Read(AllMeshMap[i].Indexs);
	}


	NewFile.Read(AllBones);

	CreateAnimationBuffer();
	CreateRenderingBuffer();
}