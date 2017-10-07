#ifndef GameObject_h__
#define GameObject_h__
#pragma once

#include "Texture.h"
#include "Vec2D.h"

class Renderer;

class GameObject
{
public:
	enum AnchorPt { TOPLEFT, TOPCENTRE, TOPRIGHT, LEFT, CENTRE, RIGHT, LOWLEFT, LOWCENTRE, LOWRIGHT, CUSTOM };

	GameObject();
	virtual ~GameObject() noexcept;

	bool CreateTexture(Renderer& renderer, std::string filename);
	bool CreateTextureFromText(Renderer& renderer, std::string text, FontTTF& font);
	void Render(Renderer& renderer) const;

	const Vec2D& GetVel() const noexcept { return m_Velocity; }
	const Vec2D& GetPos() const noexcept { return m_Position; }

	Vec2D& GetVel() { return const_cast<Vec2D&>(static_cast<const GameObject*>(this)->GetVel()); }
	Vec2D& GetPos() { return const_cast<Vec2D&>(static_cast<const GameObject*>(this)->GetPos()); }

	int GetWidth()	const noexcept { return m_Texture.GetWidth(); }
	int GetHeight() const noexcept { return m_Texture.GetHeight(); }

	void SetVelocity(const Vec2D& velocity) { m_Velocity = velocity; }
	void SetPosition(const Vec2D& position) { m_Position = position; }

	void SetAnchorPt(AnchorPt anchor);
	void SetAnchorPt(Vec2D anchorpt) { m_AnchorPt = anchorpt; }

	void Update(double dt);

protected:
	Vec2D m_AnchorPt = { 0., 0. };

	Vec2D m_Position;
	Vec2D m_Velocity;

	Texture m_Texture;
};

#endif GameObject_h__