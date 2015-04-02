#pragma once
#include <directxmath.h>

struct CBNeverChanges
{
	DirectX::XMMATRIX mView;
};

struct CBChangeOnResize
{
	DirectX::XMMATRIX mProjection;
};

struct CBChangesEveryFrame
{
	DirectX::XMMATRIX mWorld;
	DirectX::XMFLOAT4 vMeshColor;
};

struct SimpleVertex
{
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT2 Tex;
};