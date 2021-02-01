#include "frpch.h"
#include "EditorCamera.h"

#include "Frostic/Core/Input.h"
#include "Frostic/Core/KeyCodes.h"
#include "Frostic/Core/MouseButtonCodes.h"

#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#if 0
	EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip)
		: m_FOV(fov), m_AspecRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip)
	{
		UpdateView();
	}

	void EditorCamera::OnUpdate(Timestep ts)
	{
		if (Input::IsKeyPressed(Key::LeftAlt))
		{
			const glm::vec2& mouse{ Input::GetMouseX(), Input::GetMouseY() };
			glm::vec2 delta = (mouse - m_InitialMousePosition) * 0.003f;
			m_InitialMousePosition = mouse;

			if (Input::IsMouseButtonPressed(Mouse::ButtonMiddle))
				MousePan(delta);
			else if (Input::IsMouseButtonPressed(Mouse::ButtonLeft))
				MouseRotate(delta);
			else if (Input::IsMouseButtonPressed(Mouse::ButtonRight))
				MouseZoom(delta.y);
		}

		UpdateView();
	}

	void EditorCamera::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(FR_BIND_EVENT_FN(EditorCamera::OnMouseScrolled));
	}

	glm::vec3 EditorCamera::GetUpDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glm::vec3 EditorCamera::GetRightDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	glm::vec3 EditorCamera::GetForwardDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
	}

	glm::quat EditorCamera::GetOrientation() const
	{
		return glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f));
	}

	void EditorCamera::UpdateProjection()
	{
		m_AspecRatio = m_ViewportWidth / m_ViewportHeight;
		m_Projection = glm::perspective(m_FOV, m_AspecRatio, m_NearClip, m_FarClip);
	}

	void EditorCamera::UpdateView()
	{
		m_Position = CalculatePosition();

		glm::quat orientation = GetOrientation();
		m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(orientation);
		m_ViewMatrix = glm::inverse(m_ViewMatrix);
	}

	bool EditorCamera::OnMouseScrolled(MouseScrolledEvent& e)
	{
		float delta = e.GetYOffset() * 0.1f;
		MouseZoom(delta);
		UpdateView();
		return false;
	}

	void EditorCamera::MousePan(const glm::vec2& delta)
	{
		auto [xSpeed, ySpeed] = PanSpeed();
		m_FocalPoint += -GetRightDirection() * delta.x * xSpeed * m_Distance;
		m_FocalPoint += GetUpDirection() * delta.y * ySpeed * m_Distance;
	}

	void EditorCamera::MouseRotate(const glm::vec2& delta)
	{
		float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
		m_Yaw += yawSign * delta.x * RotationSpeed();
		m_Pitch += delta.y * RotationSpeed();
	}

	void EditorCamera::MouseZoom(float delta)
	{
		m_Distance -= delta * ZoomSpeed();
		if (m_Distance < 1.0f)
		{
			m_FocalPoint += GetForwardDirection();
			m_Distance = 1.0f;
		}
	}

	glm::vec3 EditorCamera::CalculatePosition() const
	{
		return m_FocalPoint - GetForwardDirection() * m_Distance;
	}

	std::pair<float, float> EditorCamera::PanSpeed() const
	{
		float x = std::min(m_ViewportWidth / 1000.0f, 2.4f);
		float xFactor = 0.366f * (x * x) - 1.778f * x + 0.3021f;

		float y = std::min(m_ViewportHeight / 1000.0f, 2.4f);
		float yFactor = 0.366f * (y * y) - 1.778f * y + 0.3021f;

		return { xFactor, yFactor };

	}

	float EditorCamera::RotationSpeed() const
	{
		return 0.8f;
	}

	float EditorCamera::ZoomSpeed() const
	{
		float distance = m_Distance * 0.2f;
		distance = std::max(distance, 0.0f);
		float speed = distance * distance;
		speed = std::min(speed, 100.0f);
		return speed;
	}
#endif

namespace Frostic {

	EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip)
		: m_FOV(fov), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip)
	{
		RefreshView();
	}

	void EditorCamera::OnUpdate(Timestep ts)
	{
		glm::vec2 mousePos = { Input::GetMouseX(), Input::GetMouseY() };
		glm::vec2 delta = InitialMousePosition - mousePos;
		InitialMousePosition = mousePos;

		if (Input::IsMouseButtonPressed(Mouse::ButtonRight))
		{
			// Rotation
			m_Rotation.x -= delta.y * 25.0f * ts;
			m_Rotation.x = std::max(m_Rotation.x, -90.0f); // Minimum is -90
			m_Rotation.x = std::min(m_Rotation.x, 90.0f);  // Maximum is  90
			m_Rotation.y -= delta.x * 25.0f * ts;
			m_Rotation.z = 0.0f;

			// Translation
			if (Input::IsKeyPressed(Key::W))
				m_Translation += GetForwardDirection() * 0.03f;
			else if (Input::IsKeyPressed(Key::S))
				m_Translation -= GetForwardDirection() * 0.03f;
			if (Input::IsKeyPressed(Key::A))
				m_Translation += GetRightDirection() * 0.03f;
			else if (Input::IsKeyPressed(Key::D))
				m_Translation -= GetRightDirection() * 0.03f;

			Input::SetHiddenCursor(true);
		}
		else if (Input::IsMouseButtonPressed(Mouse::ButtonMiddle))
		{
			m_Translation -= GetRightDirection() * delta.x * ts.GetSeconds() * 0.5f;
			m_Translation.y += delta.y * ts * 0.5f;

			Input::SetHiddenCursor(true);
		}
		else
		{
			Input::SetHiddenCursor(false);
		}

		RefreshView();
		// FR_CORE_WARN("x: {0}, y: {1}, z: {2}", m_Translation.x, m_Translation.y, m_Translation.z);
		// FR_CORE_WARN("rx: {0}, ry: {1}, rz: {2}", m_Rotation.x, m_Rotation.y, m_Rotation.z);
	}

	void EditorCamera::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(FR_BIND_EVENT_FN(EditorCamera::OnMouseScrolled));
	}

	bool EditorCamera::OnMouseScrolled(MouseScrolledEvent& e)
	{

		m_Translation += GetForwardDirection() * e.GetYOffset() * 0.1f;
		RefreshView();

		return false;
	}

	void EditorCamera::RefreshProjection()
	{
		m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
		m_Projection = glm::perspective(m_FOV, m_AspectRatio, m_NearClip, m_FarClip);
	}

	void EditorCamera::RefreshView()
	{
		m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Translation) * glm::toMat4(glm::quat(glm::radians(m_Rotation)));
		m_ViewMatrix = glm::inverse(m_ViewMatrix);
	}

	glm::vec3 EditorCamera::GetForwardDirection() const
	{
		return glm::rotate(glm::quat(glm::radians(m_Rotation)), glm::vec3(0.0f, 0.0f, -1.0f));
	}

	glm::vec3 EditorCamera::GetRightDirection() const
	{
		return glm::rotate(glm::quat(glm::radians(m_Rotation)), glm::vec3(1.0f, 0.0f, 0.0f));
	}

}