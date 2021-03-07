#pragma once

#include "OrthographicCamera.h"
#include "Camera.h"
#include "EditorCamera.h"
#include "Texture.h"
#include "SubTexture2D.h"

namespace Frostic {

	class Renderer2D
	{
	public:
		struct SpriteSpecifications
		{
			SpriteSpecifications() = default;

			uint32_t EntityID = -1;
			glm::mat4 Transform = glm::mat4(1.0f);
			Ref<Texture2D> Texture = nullptr;
			Ref<SubTexture2D> SubTexture = nullptr;
			float TilingFactor = 1.0f;
			glm::vec4 Color = glm::vec4(1.0f);

			void Reset()
			{
				EntityID = -1;
				Transform = glm::mat4(1.0f);
				Texture = nullptr;
				SubTexture = nullptr;
				TilingFactor = 1.0f;
				Color = glm::vec4(1.0f);
			}
		};
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void BeginScene(const EditorCamera& camera);
		static void BeginScene(const glm::mat4& projection, const glm::mat4& view);
		static void EndScene();
		static void Flush();

		// Primitives
		static void DrawQuad(uint32_t entityId, const glm::mat4& transform, const glm::vec4& color);
		static void DrawQuad(uint32_t entityId, const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawQuad(uint32_t entityId, const glm::mat4& transform, const Ref<SubTexture2D>& subTexture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

		static void DrawSprite(SpriteSpecifications& specs);

		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;
			uint32_t Textures = 0;

			uint32_t GetTotalVertexCount() const { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() const { return QuadCount * 6; }
		};

		static Statistics GetStats();
		static void ResetStats();

	private:
		static void StartBatch();
		static void NextBatch();
	};

}