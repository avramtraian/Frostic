#pragma once
#include <Frostic.h>

class Sandbox2D : public Frostic::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Frostic::Timestep ts) override;
	virtual void OnEvent(Frostic::Event& e) override;

	void OnLoadScene(Frostic::Ref<Frostic::Scene>& scene);
	void OnUnloadScene(int i);

private:
	Frostic::Ref<Frostic::Scene> m_ActiveScene;
};