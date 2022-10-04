#pragma once

namespace SHV {
class Scene;

class Toolbar {
   public:
    Toolbar(Scene& scene);

    void Draw();

   private:
    Scene& scene;
};
}  // namespace SHV