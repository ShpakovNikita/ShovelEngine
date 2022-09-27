#pragma once

#include <memory>

#include "Renderer.hpp"
#include "WindowConfig.hpp"

namespace SHV {

class Window;

/**
 * This instance manages render instances based on selected API option
 */
class RenderContext {
   public:
    RenderContext(const WindowConfig& windowConfig,
                  const eRenderApi& renderApi);
    ~RenderContext();

    void SetUp();
    void TearDown();

    Renderer& GetRenderer() const;
    Window& GetWindow() const;

   private:
    void CreateMetalApiContext();
    void CreateOpenGlContext();

    std::unique_ptr<Renderer> renderer;
    std::unique_ptr<Window> window;

    const WindowConfig windowConfig;
    const eRenderApi renderApi;
};
}  // namespace SHV