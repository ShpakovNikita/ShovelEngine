#pragma once

typedef union SDL_Event SDL_Event;

namespace SHV {
class RenderContext;

class ImGui {
   public:
    ImGui(RenderContext& renderContext);

    void SetUp();
    void TearDown();

    void BeginFrame();
    void EndFrame();

    void PollEvents(SDL_Event* event);

   private:
    RenderContext& renderContext;
};
}  // namespace SHV