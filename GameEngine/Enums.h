#pragma once

enum class CollisionType
{
	// 2D와 3D간의 충돌은 안되게 만들겁니다.
	Point2D,
	CirCle,
	Rect,
	OrientedRect,

	Point3D,
	Sphere3D,
	AABBBox3D,
	OBBBox3D,
	Ray,
	MAX,
};

enum class RenderingOrder
{
	BackBuffer, // 백버퍼에 직접 그리는건
	World,
	UI,
	Debug,
};