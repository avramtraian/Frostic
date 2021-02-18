#pragma once

#include "Camera.h"

#include "Frostic/Core/Timestep.h"
#include "Frostic/Events/MouseEvent.h"

#include <glm/glm.hpp>

#if 0
	class EditorCamera : public Camera
	{
	public:
		EditorCamera() = default;
		EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		inline float GetDistance() const { return m_Distance; }
		inline void SetDistance(float distance) { m_Distance = distance; }

		inline void SetViewportSize(float width, float height) { m_ViewportWidth = width; m_ViewportHeight = height; UpdateProjection(); }

		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		glm::mat4 GetViewProjection() const { return m_Projection * m_ViewMatrix; }

		glm::vec3 GetUpDirection() const;
		glm::vec3 GetRightDirection() const;
		glm::vec3 GetForwardDirection() const;
		const glm::vec3& GetPosition() const { return m_Position; }
		glm::quat GetOrientation() const;

		float GetPitch() const { return m_Pitch; }
		float GetYaw() const { return m_Yaw; }
	private:
		void UpdateProjection();
		void UpdateView();

		bool OnMouseScrolled(MouseScrolledEvent& e);

		void MousePan(const glm::vec2& delta);
		void MouseRotate(const glm::vec2& delta);
		void MouseZoom(float delta);

		glm::vec3 CalculatePosition() const;

		std::pair<float, float> PanSpeed() const;
		float RotationSpeed() const;
		float ZoomSpeed() const;
	private:
		float m_FOV = 45.0f, m_AspecRatio = 1.778f, m_NearClip = 0.01f, m_FarClip = 1000.0f;

		glm::mat4 m_ViewMatrix;
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_FocalPoint = { 0.0f, 0.0f, 0.0f };

		glm::vec2 m_InitialMousePosition = { 0.0f, 0.0f };

		float m_Distance = 10.0f;
		float m_Pitch = 0.0f, m_Yaw = 0.0f;

		float m_ViewportWidth = 1600.0f, m_ViewportHeight = 900.0f;};
#endif

namespace Frostic {

	class EditorCamera : public Camera
	{
	public:
		enum class ProjectionType { Perspective = 0, Orthographic = 1 };

		EditorCamera() = default;
		EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);
		bool OnMouseScrolled(MouseScrolledEvent& e);

		void SetViewportSize(float width, float height) { m_ViewportWidth = width; m_ViewportHeight = height; RefreshProjection(); }

		ProjectionType GetProjectionType() const { return m_Type; }
		void SetProjectionType(ProjectionType type) { m_Type = type; }

		glm::mat4 GetViewProjection() const { return m_Projection * m_ViewMatrix; }
		glm::mat4 GetViewMatrix() const { return m_ViewMatrix; }

		glm::vec3 GetTranslation() const { return m_Translation; }
		void SetTranslation(const glm::vec3& translation) { m_Translation = translation; }

		glm::vec3 GetRotation() const { return m_Rotation; }
		void SetRotation(const glm::vec3& rotation) { m_Rotation = rotation; }

		float GetPerspectiveFOV() const { return m_PerspectiveFOV; }
		void SetPerspectiveFOV(float fov) { m_PerspectiveFOV = fov; }

		float GetNearClip() const { return m_PerspectiveNearClip; }
		void SetNearClip(float nearClip) { m_PerspectiveNearClip = nearClip; }

		float GetPerspectiveFarClip() const { return m_PerspectiveFarClip; }
		void SetPerspectiveFarClip(float farClip) { m_PerspectiveFarClip = farClip; }
	private:
		void RefreshProjection();
		void RefreshView();

		glm::vec3 GetForwardDirection() const;
		glm::vec3 GetRightDirection() const;
	private:
		ProjectionType m_Type = ProjectionType::Orthographic;

		float m_ViewportWidth, m_ViewportHeight;

		glm::mat4 m_ViewMatrix;
		glm::vec3 m_Translation{ 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Rotation{ 0.0f, 0.0f, 0.0f }; // In degrees

		float m_PerspectiveFOV;
		float m_AspectRatio;
		float m_PerspectiveNearClip, m_PerspectiveFarClip;

		float m_OrthographicSize = 10.0f;
		float m_OrthographicNearClip = -1.0f, m_OrthographicFarClip = 1.0f;

		glm::vec2 InitialMousePosition = { 0.0f, 0.0f };
	};
}