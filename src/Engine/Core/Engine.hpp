#pragma once

#include <stdio.h>


namespace SHV
{
    struct ImmutableConfig;

    class Engine {
        public:
        Engine(const ImmutableConfig& config);
        ~Engine();
        
        int Run() noexcept;
    private:
        
        void Init();
        
        const ImmutableConfig& config;
    };
}
