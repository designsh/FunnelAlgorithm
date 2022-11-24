#include "PreCompile.h"
#include "GameEngineFBXAnimation.h"
#include "GameEngineFBXMeshManager.h"

GameEngineFBXAnimation::GameEngineFBXAnimation()
{
}

GameEngineFBXAnimation::~GameEngineFBXAnimation()
{
}

void GameEngineFBXAnimation::Load(const std::string& _Path)
{
	if (false == CreateFBXSystemInitialize(_Path))
	{
		GameEngineDebug::MsgBoxError("FBX �̴ϼȶ���� �����߽��ϴ�.");
		return;
	}

	FBXConvertScene();
	CheckAnimation();
}



// �ִϸ��̼��� �����ϴ��� �������ϴ����� �˾Ƴ��� �Լ�.
bool GameEngineFBXAnimation::CheckAnimation()
{
	fbxsdk::FbxArray<FbxString*> AniNameArray;
	Scene->FillAnimStackNameArray(AniNameArray);

	if (0 == AniNameArray.Size())
	{
		return false;
	}

	// fbxsdk::FbxAnimStack::ClassId

	AnimationDatas.resize(AniNameArray.Size());

	for (unsigned int i = 0; i < AnimationDatas.size(); i++)
	{
		//fbxsdk::FbxAnimStack* pAniStatic = Scene->FindMember<fbxsdk::FbxAnimStack>(AniNameArray[i]->Buffer());

		//if (nullptr == pAniStatic)
		//{
		//	GameEngineDebug::MsgBoxError("if (nullptr == pAniStatic)");
		//	continue;
		//}

		// AnimationDatas[i].AniName = GameEngineString::UTF8ToAnsiReturn(pAniStatic->GetName());

		AnimationDatas[i].AniName = GameEngineString::UTF8ToAnsiReturn(AniNameArray[i]->Buffer());

		FbxTakeInfo* TakeInfo = Scene->GetTakeInfo(AnimationDatas[i].AniName.c_str());

		AnimationDatas[i].StartTime = TakeInfo->mLocalTimeSpan.GetStart();
		AnimationDatas[i].EndTime = TakeInfo->mLocalTimeSpan.GetStop();

		// �ִϸ��̼��� ������ �ƴ��� �ʷ� �ƴ��� ���뼼����� �ƴ��� ������ ������� ����ƴ��� ���
		AnimationDatas[i].TimeMode = Scene->GetGlobalSettings().GetTimeMode();
		AnimationDatas[i].TimeStartCount = AnimationDatas[i].StartTime.GetFrameCount(AnimationDatas[i].TimeMode);

		if (0 >= AnimationDatas[i].TimeStartCount)
		{
			AnimationDatas[i].TimeStartCount *= (FbxLongLong)-1;
		}

		AnimationDatas[i].TimeEndCount = AnimationDatas[i].EndTime.GetFrameCount(AnimationDatas[i].TimeMode);
		AnimationDatas[i].FrameCount = AnimationDatas[i].TimeEndCount - AnimationDatas[i].TimeStartCount;

		AnimationDatas[i].FbxModeCount = (long long)fbxsdk::FbxTime::GetFrameRate(AnimationDatas[i].TimeMode);
		AnimationDatas[i].FbxModeRate = (double)fbxsdk::FbxTime::GetFrameRate(AnimationDatas[i].TimeMode);
	}

	for (int i = 0; i < AniNameArray.Size(); i++)
	{
		delete AniNameArray[i];
	}

	return true;
}


void GameEngineFBXAnimation::ProcessAnimationLoad(GameEngineFBXMesh* _Mesh, fbxsdk::FbxNode* pNode, int _index)
{
	// ��� ��带 �� �ȱ� ���ؼ�

	fbxsdk::FbxNodeAttribute* pNodeAttribute = pNode->GetNodeAttribute();
	if (nullptr != pNodeAttribute)
	{
		fbxsdk::FbxNodeAttribute::EType typetype = pNodeAttribute->GetAttributeType();
		switch (pNodeAttribute->GetAttributeType())
		{
		case fbxsdk::FbxNodeAttribute::eLODGroup:
		{
			if (0 != pNode->GetChildCount())
			{
				ProcessAnimationLoad(_Mesh, pNode->GetChild(0), _index);
				return;
			}
		}
		break;
		case fbxsdk::FbxNodeAttribute::eMesh:
		{
			AnimationLoad(_Mesh, pNode, _index);
		}
		break;
		default:
			break;
		}
	}

	for (int n = 0; n < pNode->GetChildCount(); ++n)
	{
		ProcessAnimationLoad(_Mesh, pNode->GetChild(n), _index);
	}
}


void GameEngineFBXAnimation::ProcessAnimationCheckState(GameEngineFBXMesh* _Fbx, int userAniDataIndex)
{
	// �ڴ�
	FbxExAniData& userAniData = AnimationDatas.at(userAniDataIndex);
	fbxsdk::FbxLongLong fbxTime = userAniData.EndTime.Get() - userAniData.StartTime.Get() + 1;

	// ����
	for (size_t MeshIndex = 0; MeshIndex < userAniData.AniFrameData.size(); MeshIndex++)
	{
		// 30�������� ������
		size_t aniFrameDataSize = userAniData.AniFrameData[MeshIndex].size();
		for (size_t aniFrameDataIndex = 0; aniFrameDataIndex < aniFrameDataSize; ++aniFrameDataIndex)
		{
			FbxExBoneFrame& aniFrameData = userAniData.AniFrameData[MeshIndex].at(aniFrameDataIndex);
			// ���������.
			if (0 == aniFrameData.BoneMatData.size())
			{
				aniFrameData.BoneMatData.resize(fbxTime);
				Bone& curBone = _Fbx->AllBones[0][aniFrameDataIndex];
				aniFrameData.BoneIndex = curBone.Index;
				aniFrameData.BoneParentIndex = curBone.ParentIndex;
				if (-1 != curBone.ParentIndex)
				{
					FbxExBoneFrame& parentAniFrameData = userAniData.AniFrameData[MeshIndex].at(curBone.ParentIndex);
					for (fbxsdk::FbxLongLong start = 0; start < fbxTime; ++start)
					{
						aniFrameData.BoneMatData[start].Time = parentAniFrameData.BoneMatData[start].Time;
						aniFrameData.BoneMatData[start].LocalAnimation = float4x4ToFbxAMatrix(curBone.BonePos.Local);
						aniFrameData.BoneMatData[start].GlobalAnimation = parentAniFrameData.BoneMatData[start].GlobalAnimation * aniFrameData.BoneMatData[start].LocalAnimation;
						aniFrameData.BoneMatData[start].FrameMat = FbxMatTofloat4x4(aniFrameData.BoneMatData[start].GlobalAnimation);
						aniFrameData.BoneMatData[start].S = FbxVecTofloat4(aniFrameData.BoneMatData[start].GlobalAnimation.GetS());
						aniFrameData.BoneMatData[start].Q = FbxQuaternionTofloat4(aniFrameData.BoneMatData[start].GlobalAnimation.GetQ());
						aniFrameData.BoneMatData[start].T = FbxVecToTransform(aniFrameData.BoneMatData[start].GlobalAnimation.GetT());
					}
				}
				else
				{
					for (fbxsdk::FbxLongLong start = 0; start < fbxTime; ++start)
					{
						aniFrameData.BoneMatData[start].Time = 0;
						aniFrameData.BoneMatData[start].LocalAnimation = float4x4ToFbxAMatrix(curBone.BonePos.Local);
						aniFrameData.BoneMatData[start].GlobalAnimation = aniFrameData.BoneMatData[start].LocalAnimation;
						aniFrameData.BoneMatData[start].FrameMat = FbxMatTofloat4x4(aniFrameData.BoneMatData[start].GlobalAnimation);
						aniFrameData.BoneMatData[start].S = FbxVecTofloat4(aniFrameData.BoneMatData[start].GlobalAnimation.GetS());
						aniFrameData.BoneMatData[start].Q = FbxQuaternionTofloat4(aniFrameData.BoneMatData[start].GlobalAnimation.GetQ());
						aniFrameData.BoneMatData[start].T = FbxVecToTransform(aniFrameData.BoneMatData[start].GlobalAnimation.GetT());
					}
				}
			}
		}
	}

}

void GameEngineFBXAnimation::CalFbxExBoneFrameTransMatrix(GameEngineFBXMesh* _Mesh, int _AnimationIndex)
{
	if (0 == AnimationDatas.size())
	{
		GameEngineDebug::MsgBoxError("�ִϸ��̼� �����Ͱ� �������� �ʴ� �Ž��� �ִϸ��̼��� ������� �߽��ϴ�.");
		return;
	}

	if (0 != AnimationDatas[_AnimationIndex].AniFrameData.size())
	{
		// 0810 ������Ʈ : �������� �� �Լ��� ����ģ �޽ø� ����
		return;
	}


	AnimationDatas[_AnimationIndex].AniFrameData.resize(_Mesh->MeshInfos.size());
	for (UINT MeshCount = 0; MeshCount < _Mesh->MeshInfos.size(); MeshCount++)
	{
		AnimationDatas[_AnimationIndex].AniFrameData[MeshCount].resize(_Mesh->GetBoneCount(MeshCount));
	}

	ProcessAnimationLoad(_Mesh, _Mesh->RootNode, _AnimationIndex);

	ProcessAnimationCheckState(_Mesh, _AnimationIndex);


	AnimationDatas;
	//if (nullptr != _AniProgress)
	//{
	//	_AniProgress(100.0f);
	//}

}



// ��忡 �����ϴ� �������� �����ؼ� ��ķ� �������ݴϴ�.
fbxsdk::FbxAMatrix GameEngineFBXAnimation::GetGeometryTransformation(fbxsdk::FbxNode* pNode)
{
	fbxsdk::FbxVector4 translation = pNode->GetGeometricTranslation(fbxsdk::FbxNode::eSourcePivot);
	fbxsdk::FbxVector4 rotation = pNode->GetGeometricRotation(fbxsdk::FbxNode::eSourcePivot);
	fbxsdk::FbxVector4 scale = pNode->GetGeometricScaling(fbxsdk::FbxNode::eSourcePivot);

	return fbxsdk::FbxAMatrix(translation, rotation, scale);
}


bool GameEngineFBXAnimation::AnimationLoad(GameEngineFBXMesh* _Mesh, fbxsdk::FbxNode* _Node, int AnimationIndex)
{
	FbxAnimStack* stack = Scene->GetSrcObject<FbxAnimStack>(AnimationIndex);
	Scene->SetCurrentAnimationStack(stack);

	fbxsdk::FbxMesh* pCurrMesh = _Node->GetMesh();
	int deformerCount = pCurrMesh->GetDeformerCount();
	fbxsdk::FbxAMatrix geometryTransform = GetGeometryTransformation(_Node);

	fbxsdk::FbxTakeInfo* takeInfo = Scene->GetTakeInfo(AnimationDatas[AnimationIndex].AniName.c_str());
	fbxsdk::FbxTime start = takeInfo->mLocalTimeSpan.GetStart();
	fbxsdk::FbxTime end = takeInfo->mLocalTimeSpan.GetStop();
	fbxsdk::FbxTime::EMode timeMode = Scene->GetGlobalSettings().GetTimeMode();

	fbxsdk::FbxAMatrix currentTransformOffset;
	fbxsdk::FbxAMatrix localTransform;
	fbxsdk::FbxAMatrix globalTransform;

	fbxsdk::FbxCluster* pCurrCluster;
	std::string currJointName;
	Bone* pBone;

	fbxsdk::FbxAMatrix transformMatrix;
	fbxsdk::FbxAMatrix transformLinkMatrix;
	fbxsdk::FbxAMatrix globalBindposeInverseMatrix;

	fbxsdk::FbxLongLong fixIndex;

	std::string linkName;

	fbxsdk::FbxLongLong endTime;
	fbxsdk::FbxLongLong startTime;

	fbxsdk::FbxTime currTime;

	for (int deformerIndex = 0; deformerIndex < deformerCount; ++deformerIndex)
	{
		fbxsdk::FbxSkin* pCurrSkin = reinterpret_cast<fbxsdk::FbxSkin*>(pCurrMesh->GetDeformer(deformerIndex, fbxsdk::FbxDeformer::eSkin));
		if (nullptr == pCurrSkin)
		{
			continue;
		}

		unsigned int numOfClusters = pCurrSkin->GetClusterCount();

		endTime = end.GetFrameCount(timeMode);
		startTime = start.GetFrameCount(timeMode);

		FbxExAniData& CurAniData = AnimationDatas[AnimationIndex];
		CurAniData.EndTime = endTime;
		CurAniData.StartTime = startTime;
		CurAniData.TimeMode = timeMode;

		for (size_t MeshIndex = 0; MeshIndex < CurAniData.AniFrameData.size(); ++MeshIndex)
		{
			if (0 == CurAniData.AniFrameData[MeshIndex].size())
			{
				continue;
			}

			for (unsigned int clusterIndex = 0; clusterIndex < numOfClusters; ++clusterIndex)
			{
				pCurrCluster = pCurrSkin->GetCluster(clusterIndex);
				currJointName = pCurrCluster->GetLink()->GetName();
				pBone = _Mesh->FindBone(0, currJointName);

				pCurrCluster->GetTransformMatrix(transformMatrix);
				pCurrCluster->GetTransformLinkMatrix(transformLinkMatrix);
				globalBindposeInverseMatrix = transformLinkMatrix.Inverse();

				linkName = pCurrCluster->GetLink()->GetName();
				fbxsdk::FbxNode* pLinkNode = Scene->FindNodeByName(linkName.c_str());

				FbxExBoneFrame& Frame = CurAniData.AniFrameData[MeshIndex][pBone->Index];
				Frame.BoneMatData.resize(endTime - startTime + 1);
				Frame.BoneIndex = pBone->Index;
				Frame.BoneParentIndex = pBone->ParentIndex;

				for (fbxsdk::FbxLongLong i = startTime; i <= endTime; ++i)
				{
					fixIndex = i - startTime;

					FbxExBoneFrameData& FrameData = Frame.BoneMatData[fixIndex];

					currTime.SetFrame(fixIndex, timeMode);
					// ��
					currentTransformOffset = _Node->EvaluateGlobalTransform(currTime) * JointMatrix * geometryTransform;
					globalTransform = currentTransformOffset.Inverse() * pLinkNode->EvaluateGlobalTransform(currTime);

					localTransform.SetS(pLinkNode->EvaluateLocalScaling(currTime));
					localTransform.SetR(pLinkNode->EvaluateLocalRotation(currTime));
					localTransform.SetT(pLinkNode->EvaluateLocalTranslation(currTime));

					FrameData.Time = currTime.GetSecondDouble();
					FrameData.LocalAnimation = localTransform;
					FrameData.GlobalAnimation = globalTransform;
					FrameData.FrameMat = FbxMatTofloat4x4(FrameData.GlobalAnimation);
					FrameData.S = FbxVecTofloat4(FrameData.GlobalAnimation.GetS());
					FrameData.Q = FbxQuaternionTofloat4(FrameData.GlobalAnimation.GetQ());
					FrameData.T = FbxVecToTransform(FrameData.GlobalAnimation.GetT());
				}
			}
		}

	}

	return true;
}


void GameEngineFBXAnimation::UserSave(const std::string& _Path)
{
	GameEngineFile NewFile = GameEngineFile(_Path, "wb");
	//NewFile.Open("wb");

	//if (1 < AnimationDatas.size())
	//{
	//	for (int i = 0; i < AnimationDatas.size(); i++)
	//	{
	//		NewFile.Write(AnimationDatas[i]);
	//	}
	//
	//	return;
	//}

	NewFile.Write(AnimationDatas);
}
void GameEngineFBXAnimation::UserLoad(const std::string& _Path)
{
	GameEngineFile NewFile = GameEngineFile(_Path, "rb");
	// NewFile.Open("wb");
	NewFile.Read(AnimationDatas);
}