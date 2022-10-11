#pragma once

namespace SHV {
class Window;

class WindowContext {
   public:
    WindowContext() = default;
    virtual ~WindowContext() = default;

    virtual void SetUp() = 0;
    virtual void TearDown() = 0;

    virtual const Window& GetWindow() = 0;
};
}  // namespace SHV