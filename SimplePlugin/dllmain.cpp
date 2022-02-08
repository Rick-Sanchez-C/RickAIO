	#include "../plugin__sdk/plugin_sdk.hpp"

	PLUGIN_NAME("RickSanchez AIO");
	SUPPORTED_CHAMPIONS(champion_id::Tryndamere);
	//WIP : SUPPORTED_CHAMPIONS(champion_id::Tryndamere,champion_id::Varus);

	#include "tryndamere.h"
	//WIP
	//#include "Varus.h"

	PLUGIN_API bool on_sdk_load(plugin_sdk_core* plugin_sdk_good)
	{
	    DECLARE_GLOBALS(plugin_sdk_good);

	    switch (myhero->get_champion())
	    {
	    //case champion_id::Varus:
	    //    Varus::load();
	    //    break;
	    case champion_id::Tryndamere:
	        tryndamere::load();
	        break;
	    default:
	        console->print("Champion %s is not supported", myhero->get_model_cstr());
	        return false;
	    }

	    return true;

	}

	PLUGIN_API void on_sdk_unload()
	{
	    switch (myhero->get_champion())
	    {
	        //case champion_id::Varus:
	        //    Varus::unload();
	        //    break;
	    case champion_id::Tryndamere:
			tryndamere::unload();
			break;

	    default:
	        break;

	    }
	}