//
// Created by Andrew Fagan on 10/30/24.
//

#include "Core/SystemManager.h"

#pragma once

namespace IO{

    class IOManager : public SystemManager
    {
    public:
        static IOManager& Get();

        void Init() override;
        void Shutdown() override;
        void Update() override;


        IOManager(const IOManager&) = delete;
        IOManager& operator=(const IOManager&) = delete;
        IOManager(IOManager&&) = delete;
        IOManager& operator=(IOManager&&) = delete;
    private:
        IOManager() = default;
        ~IOManager() override = default;
    };

    extern IOManager& g_IOManager;

}
