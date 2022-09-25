//Using SDL and standard IO
#include <stdio.h>

#include "Engine/Utils/Logger.hpp"
#include "Engine/Core/Engine.hpp"
#include "Engine/Core/ImmutableConfig.hpp"

constexpr int kWidth = 640, kHeight = 420;

int main(int argc, char *argv[])
{
    SHV::ImmutableConfig engineConfig = { kWidth, kHeight, {} };
    for (size_t i = 0; i < static_cast<size_t>(argc); i++) {
        engineConfig.args.push_back(argv[i]);
    };
    
    SHV::Engine engine = { engineConfig };
    
    return engine.Run();
}
