#include "Engine/Core/Engine.hpp"
#include "Engine/Core/ImmutableConfig.hpp"
#include "Engine/Core/MutableConfig.hpp"

constexpr int kWidth = 1200, kHeight = 800;

int main(int argc, char *argv[]) {
    SHV::ImmutableConfig immutableConfig = {
        kWidth, kHeight, SHV::eRenderApi::kMetal, {}};
    for (size_t i = 0; i < static_cast<size_t>(argc); i++) {
        immutableConfig.args.push_back(argv[i]);
    };

    SHV::MutableConfig mutableConfig = {120.0f};

    SHV::Engine engine = {immutableConfig, mutableConfig};

    return engine.Run();
}
