#pragma once

#include "Modules/ModuleManager.h"

/** Editor module for STAT schema validation. */
class FSTATEditorModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
