#pragma once

#include <GameEngineBase/GameEngineSoundManager.h>
#include "GameEngineConstantBufferManager.h"
#include "GameEngineVertexBufferManager.h"
#include "GameEngineVertexShaderManager.h"
#include "GameEngineIndexBufferManager.h"
#include "GameEngineRasterizerManager.h"
#include "GameEnginePixelShaderManager.h"
#include "GameEngineRenderTargetManager.h"
#include "GameEngineFolderTextureManager.h"
#include "GameEngineTextureManager.h"
#include "GameEngineSamplerManager.h"
#include "GameEngineBlendManager.h"
#include "GameEngineDepthStencilManager.h"
#include "GameEngineDepthBufferManager.h"
#include "GameEngineFontManager.h" 
#include "GameEngineFBXMeshManager.h"
#include "GameEngineFBXAnimationManager.h"
#include "GameEngineStructuredBufferManager.h"

#include "GameEngineRenderingPipeLineManager.h"

#include "GameEngineFont.h"
#include "GameEngineConstantBuffer.h"
#include "GameEngineVertexBuffer.h"
#include "GameEngineVertexShader.h"
#include "GameEngineRenderTarget.h"
#include "GameEngineIndexBuffer.h"
#include "GameEnginePixelShader.h"
#include "GameEngineRasterizer.h"
#include "GameEngineFolderTexture.h"
#include "GameEngineTexture.h"
#include "GameEngineSampler.h"
#include "GameEngineBlend.h"
#include "GameEngineDepthStencil.h"
#include "GameEngineDepthBuffer.h"
#include "GameEngineFBXMesh.h"
#include "GameEngineStructuredBuffer.h"

#include "GameEngineRenderingPipeLine.h"



class GameEngineManagerHelper 
{
public:
	static void ManagerRelease()
	{
		GameEngineSoundManager::Destroy();

		GameEngineFBXAnimationManager::Destroy();
		GameEngineFBXMeshManager::Destroy();
		GameEngineFontManager::Destroy();
		GameEngineRenderingPipeLineManager::Destroy();
		GameEngineConstantBufferManager::Destroy();
		GameEngineFolderTextureManager::Destroy();
		GameEngineIndexBufferManager::Destroy();
		GameEngineVertexBufferManager::Destroy();
		GameEngineVertexShaderManager::Destroy();
		GameEngineRasterizerManager::Destroy();
		GameEngineRenderTargetManager::Destroy();
		GameEnginePixelShaderManager::Destroy();
		GameEngineSamplerManager::Destroy();
		GameEngineTextureManager::Destroy();
		GameEngineBlendManager::Destroy();
		GameEngineDepthStencilManager::Destroy();
		GameEngineDepthBufferManager::Destroy();
		GameEngineFBXAnimationManager::Destroy();
		GameEngineFBXMeshManager::Destroy();
		GameEngineStructuredBufferManager::Destroy();
	}
};
