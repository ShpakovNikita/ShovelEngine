#pragma once

#include <memory>

namespace SHV {
class Scene;
class Engine;
class HierarchyViewer;
class CameraDataViewer;
class ProfilersWindow;

class Toolbar {
   public:
    Toolbar(Scene& scene, Engine& engine);
    ~Toolbar();

    void Draw();

   private:
    void DrawFileMenu();
    void DrawToolsMenu();
    void DrawEngineMenu();

    Scene& scene;
    Engine& engine;

    bool showHierarchyViewer = false;
    std::unique_ptr<HierarchyViewer> hierarchyViewer;

    bool showCameraDataViewer = false;
    std::unique_ptr<CameraDataViewer> cameraDataViewer;

    bool showProfilersDataViewer = false;
    std::unique_ptr<ProfilersWindow> profilersDataViewer;
};
}  // namespace SHV