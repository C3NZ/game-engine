#include "core/Application.h"

#include <functional>

#include "core/Input.h"
#include "core/Window.h"
#include "core/events/ApplicationEvent.h"
#include "core/events/Event.h"
#include "core/layers/Layer.h"
#include "core/memory/Pointers.h"
#include "core/util/Assert.h"
#include "core/util/Log.h"
#include "core/util/Reverse.h"
#include "core/util/Time.h"

namespace engine {

using engine::memory::Shared;

memory::Unique<Application> Application::kApplication_ = nullptr;

Application::Application() {
  ENGINE_CORE_ASSERT(!kApplication_, "Application already exists.");
  kApplication_.reset(this);

  window_ = Window::Create();
  window_->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

  imgui_layer_ = memory::CreateShared<imgui::ImGuiLayer>();
  PushLayer(imgui_layer_);
}

Application::~Application() {}

void Application::Run() {
  while (running_) {
    util::Time current_frame_time;
    util::TimeStep time_step(last_frame_time_, current_frame_time);
    last_frame_time_ = current_frame_time;

    for (Shared<layers::Layer> layer : layer_stack_) {
      layer->OnUpdate(time_step);
    }

    imgui_layer_->Begin();
    for (memory::Shared<layers::Layer> layer : layer_stack_) {
      layer->OnImGuiRender();
    }
    imgui_layer_->End();

    window_->OnUpdate();
  }
}

void Application::OnEvent(memory::Shared<Event> event) {
  events::EventDispatcher dispatcher(event);
  dispatcher.Dispatch<events::WindowCloseEvent>
      (BIND_EVENT_FN(Application::OnWindowClosed));

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
  return true;
}

}  // namespace engine
