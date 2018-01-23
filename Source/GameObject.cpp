#include "GameObject.h"

#include "Renderer.h"

GameObject::GameObject()
{

}

GameObject::~GameObject()
{
	m_Texture.Release();
}

bool GameObject::CreateTexture(Renderer& renderer, std::string filename)
{
	return m_Texture.CreateFromFile(renderer, filename);
}

bool GameObject::CreateTextureFromText(Renderer& renderer, std::string text, FontTTF& font)
{
	return m_Texture.CreateFromText(renderer, text, font);
}

void GameObject::Render(Renderer& renderer) const
{
	Vec2D render_pos = m_Position + m_AnchorPt;
	m_Texture.Render(renderer, render_pos.asPoint());
}

void GameObject::SetAnchorPt(AnchorPt anchor)
{
	if (anchor == CENTRE || anchor == TOPCENTRE || anchor == LOWCENTRE)
		m_AnchorPt.x = -m_Texture.GetWidth() / 2;
	else if (anchor == RIGHT || anchor == TOPRIGHT || anchor == LOWRIGHT)
		m_AnchorPt.x = -m_Texture.GetWidth();
	else
		m_AnchorPt.x = 0;

	if (anchor == LEFT || anchor == CENTRE || anchor == RIGHT)
		m_AnchorPt.y = -m_Texture.GetHeight() / 2;
	else if (anchor == LOWLEFT || anchor == LOWCENTRE || anchor == LOWRIGHT)
		m_AnchorPt.y = -m_Texture.GetHeight();
	else
		m_AnchorPt.y = 0;
}
void GameObject::Update(double dt)
{
	m_Position += m_Velocity*dt;
}

