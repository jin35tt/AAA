#pragma once

#include "Modules/ModuleManager.h"

/**
 * Core module interface for the STAT system.
 */
class FSTATModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
