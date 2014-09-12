// Copyright 2014 afuzzyllama. All Rights Reserved.

#pragma once

#include "ModuleManager.h"
#include "VoronoiDiagram.h"

/**
 * The public interface to this module.  In most cases, this interface is only public to sibling modules
 * within this plugin.
 */
class FVoronoiDiagramModule : public IModuleInterface, public TSharedFromThis<FVoronoiDiagramModule>
{
    
public:
	/**
	 * Singleton-like access to this module's interface.  This is just for convenience!
	 * Beware of calling this during the shutdown phase, though.  Your module might have been unloaded already.
	 *
	 * @return Returns singleton instance, loading the module on demand if needed
	 */
	static inline FVoronoiDiagramModule& Get()
	{
		return FModuleManager::LoadModuleChecked< FVoronoiDiagramModule >( "VoronoiDiagram" );
	}
    
	/**
	 * Checks to see if this module is loaded and ready.  It is only valid to call Get() if IsAvailable() returns true.
	 *
	 * @return True if the module is loaded and ready to use
	 */
	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded( "VoronoiDiagram" );
	}

    // Begin IModuleInterface implementation
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
    // End IModuleInterface implementation

//    FVoronoiDiagram Generate(int32 DimensionLength, TArray<FVector2D>& Points);
};