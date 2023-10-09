#pragma once

#include <GL/glew.h>

#include <memory>
#include <optional>
#include <string>

#include "Engine/Render/Renderer.hpp"

namespace SHV::OpenGl {

class WindowContext;
class RenderBatch;

class Renderer : public SHV::Renderer {
   public:
    Renderer(WindowContext& window);
    virtual ~Renderer();

    virtual void SetUp();
    virtual void TearDown();

    virtual void Draw(const Scene& scene);
    virtual std::shared_ptr<Texture> Draw(const Scene& scene, const Texture& renderTargetPrototype);

    virtual void SetUpScene(Scene& scene) const;
    virtual void TearDownScene(Scene& scene) const;

    virtual void BeginFrame();
    virtual void EndFrame();

    virtual void WaitForFrameExecutionFinish();

   private:
    std::optional<std::string> ValidateExtensions();

    WindowContext& windowContext;
};

}  // namespace SHV::OpenGl