#include "Engine/Core/Engine.hpp"
#include "Engine/Core/ImmutableConfig.hpp"

constexpr int kWidth = 1200, kHeight = 800;

int main(int argc, char *argv[]) {
    SHV::ImmutableConfig immutableConfig = {kWidth, kHeight, {}};
    for (size_t i = 0; i < static_cast<size_t>(argc); i++) {
        immutableConfig.args.push_back(argv[i]);
    };

    SHV::MutableConfig mutableConfig = {120.0f, SHV::eRenderApi::kMetal};

    SHV::Engine engine = {immutableConfig, mutableConfig};

    return engine.Run();
}
