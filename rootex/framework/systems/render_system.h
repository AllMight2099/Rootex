#pragma once

#include "framework/components/visual/camera_component.h"
#include "framework/system.h"
#include "framework/systems/hierarchy_system.h"
#include "main/window.h"

class RenderSystem : public System
{
	CameraComponent* m_Camera;

	Ptr<Renderer> m_Renderer;
	Vector<Matrix> m_TransformationStack;
	Vector<Matrix> m_UITransformationStack;

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_VSPerFrameConstantBuffer, m_VSProjectionConstantBuffer, m_PSPerFrameConstantBuffer;

	RenderSystem();
	RenderSystem(RenderSystem&) = delete;
	~RenderSystem();

	void calculateTransforms(HierarchyComponent* hierarchyComponent);
	void renderPassRender(VisualComponent* vc, const RenderPass& renderPass);

public:
	static RenderSystem* GetSingleton();
	
	void render();
	void recoverLostDevice();

	void setCamera(CameraComponent* camera);
	void restoreCamera();

	void pushMatrix(const Matrix& transform);
	void pushMatrixOverride(const Matrix& transform);
	void popMatrix();
	void pushUIMatrix(const Matrix& transform);
	void popUIMatrix();

	void setProjectionConstantBuffers();
	void perFrameVSCBBinds();
	void perFramePSCBBinds();

	CameraComponent* getCamera() const { return m_Camera; }
	const Matrix& getTopMatrix() const;
	Matrix& getTopUIMatrix();
	const Renderer* getRenderer() const { return m_Renderer.get(); }
};
