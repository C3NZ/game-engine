/// @file Renderer.h
/// @brief The rendering API.
#ifndef LAMBDA_SRC_CORE_RENDERER_RENDERER_H_
#define LAMBDA_SRC_CORE_RENDERER_RENDERER_H_

#include "core/renderer/RendererAPI.h"

#include "core/renderer/OrthographicCamera.h"
#include "core/renderer/Shader.h"

namespace lambda {
namespace core {
namespace renderer {

/// @brief The primary interface used for managing
class Renderer {
 public:
  /// @brief Initialize the Renderer.
  static void Init();

  /// @brief Begin a scene to be renderer.
  ///
  /// Uses a single orthograhpic camera to determine where things should be
  /// renderered on screen.
  static void BeginScene(const OrthographicCamera& camera);

  /// @begin Ends the rendering scene
  static void EndScene();

  /// @brief Updates the renderer with the new screen width and height.
  static void OnWindowResize(uint32_t width, uint32_t height);

  /// @brief Submit a vertex array, shader, and transform matrix for rendering.
  ///
  /// Must be associated with a specific scene. (used in between BeginScene and
  /// EndScene calls)
  /// TODO(C3NZ): update this to use engine memory allocators as opposed to
  // generic smart pointers.
  static void Submit(
      const std::shared_ptr<VertexArray>& vertex_array,
      const std::shared_ptr<Shader>& shader,
      const glm::mat4& transform = glm::mat4(1.0f));

  /// @brief Get the API being used by the current renderer.
  static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

 private:
  struct SceneData {
    glm::mat4 ViewProjectionMatrix;
  };

  static SceneData* scene_data_;
};

}  // namespace renderer
}  // namespace core
}  // namespace lambda

#endif  // LAMBDA_SRC_CORE_RENDERER_RENDERER_H_
