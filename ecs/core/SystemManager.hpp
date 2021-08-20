#pragma once

#include<unordered_map>
#include<memory>
#include<cassert>

#include "ecs/core/System.hpp"

class SystemManager
{
public:
    SystemManager();
    template<typename DrSys>
        std::shared_ptr<DrSys> registerSystem();
    template<typename DrSys>
        void setSignature(Signature signature);

    void entityDestroyed(Entity entity);
    void entitySignatureChanged(Entity entity, Signature newSignature);
private:
    std::unordered_map<const char*, Signature> mSignatures;
    std::unordered_map<const char*,
            std::shared_ptr<System>> mSystems;
};

template<typename DrSys>
std::shared_ptr<DrSys> SystemManager::registerSystem()
{
    const char* typeName = typeid(DrSys).name();
    assert(mSystems.find(typeName) == mSystems.end()
           && "Same system type register twice!");

    std::shared_ptr<DrSys> ptr = std::make_shared<DrSys>();
    mSystems[typeName] = ptr;
    return ptr;
}

template<typename DrSys>
void SystemManager::setSignature(Signature signature)
{
    const char* typeName = typeid(DrSys).name();
    assert(mSystems.find(typeName) != mSystems.end()
           && "System has not been registered");

    mSignatures[typeName] = signature;
}
