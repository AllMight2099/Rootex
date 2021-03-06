#pragma once

#include "visual_component.h"

class CameraComponent : public Component
{
	static Component* Create(const JSON::json& componentData);
	static Component* CreateDefault();

	bool m_Active;
	float m_FoV;
	float m_Near;
	float m_Far;
	Vector2 m_AspectRatio;
	TransformComponent* m_TransformComponent;
	Matrix m_ViewMatrix;
	Matrix m_ProjectionMatrix;

	CameraComponent(const Vector2& aspectRatio, float fov, float nearPlane, float farPlane);
	CameraComponent(CameraComponent&) = delete;
	~CameraComponent() = default;

	friend class EntityFactory;

	void refreshProjectionMatrix();
	void refreshViewMatrix();

public:
	virtual bool setup() override;
	void onRemove() override;

	virtual const Matrix& getViewMatrix();
	virtual const Matrix& getProjectionMatrix();
	virtual String getName() const override { return "CameraComponent"; }

	static const ComponentID s_ID = (ComponentID)ComponentIDs::CameraComponent;
	ComponentID getComponentID() const { return s_ID; }

	virtual JSON::json getJSON() const override;

#ifdef ROOTEX_EDITOR
	void draw() override;
#endif // ROOTEX_EDITOR
};
