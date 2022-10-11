#pragma once

#include <memory>

namespace SHV {
class Scene;
class HierarchyViewer;

class Toolbar {
   public:
    Toolbar(Scene& scene);
    ~Toolbar();

    void Draw();

   private:
    void DrawFileMenu();
    void DrawToolsMenu();

    Scene& scene;

    bool showHierarchyViewer = false;
    std::unique_ptr<HierarchyViewer> hierarchyViewer;
};
}  // namespace SHV