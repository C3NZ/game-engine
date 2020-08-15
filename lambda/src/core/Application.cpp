#include "core/Application.h"

#include <functional>

#include "core/input/Input.h"
#include "core/Window.h"
#include "core/events/ApplicationEvent.h"
#include "core/events/Event.h"
#include "core/layers/Layer.h"
#include "core/memory/Pointers.h"
#include "core/util/Assert.h"
#include "core/util/Log.h"
#include "core/util/Reverse.h"
#include "core/util/Time.h"
#include "core/renderer/Renderer.h"

namespace lambda {
namespace core {

memory::Unique<Application> Application::kApplication_ = nullptr;

Application::Application() {
  LAMBDA_CORE_ASSERT(!kApplication_, "Application already exists.");
  kApplication_.reset(this);

  window_ = Window::Create();
  window_->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

  imgui_layer_ = memory::CreateShared<imgui::ImGuiLayer>();
  PushLayer(imgui_layer_);
}

/// The application must tell the single to release itself once it's being
/// destroyed, so that it's destructor is not called again.
Application::~Application() {
  kApplication_.release();
}

void Application::Run() {
  while (running_) {
    util::Time current_frame_time;
    util::TimeStep time_step(last_frame_time_, current_frame_time);
    last_frame_time_ = current_frame_time;

    if (!minimized_) {
      for (memory::Shared<layers::Layer> layer : layer_stack_) {
        layer->OnUpdate(time_step);
      }
    }

    imgui_layer_->Begin();
    for (memory::Shared<layers::Layer> layer : layer_stack_) {
      layer->OnImGuiRender();
    }
    imgui_layer_->End();

    window_->OnUpdate();
  }
}

void Application::OnEvent(memory::Shared<events::Event> event) {
  events::EventDispatcher dispatcher(event);
  dispatcher.Dispatch<events::WindowCloseEvent>(
      BIND_EVENT_FN(Application::OnWindowClosed));

  dispatcher.Dispatch<events::WindowResizeEvent>(
      BIND_EVENT_FN(Application::OnWindowResize));

  for (memory::Shared<layers::Layer> layer : util::Reverse(layer_stack_)) {
    layer->OnEvent(event);
    if (event->HasBeenHandled()) {
      break;
    }
  }
}

void Application::PushLayer(memory::Shared<layers::Layer> layer) {
  layer_stack_.PushLayer(layer);
  layer->OnAttach();
}

void Application::PushOverlay(memory::Shared<layers::Layer> layer) {
  layer_stack_.PushOverlay(layer);
  layer->OnAttach();
}

bool Application::OnWindowClosed(const events::WindowCloseEvent& event) {
  running_ = false;
  return false;
}

/// Doesn't update when the window is resized.
bool Application::OnWindowResize(const events::WindowResizeEvent& event) {
  if (event.GetWidth() == 0 || event.GetHeight() == 0) {
    minimized_ = true;
    return false;
  }

  // Send the resize to the renderer.
  minimized_ = false;
  renderer::Renderer::OnWindowResize(event.GetWidth(), event.GetHeight());

  return false;
}


}  // namespace core
}  // namespace lambda