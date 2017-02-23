#pragma once
#include "CUESDK.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/// Defines type for Guid returned by <lfxname>Create<effect_name>Effect functions.
	typedef struct EffectGuidTag *Guid;

	/// Contains r, g and b components of the color. 
	struct CorsairColor 
	{
		int r;    /**< Red component [0..255] */
		int g;    /**< Green component [0..255] */
		int b;    /**< Blue component [0..255] */
	};

	/// Contains information about the frame.
	struct CorsairFrame             
	{
		int size;                            /**< Number of leds in ledsColors array */
		struct CorsairLedColor* ledsColors;  /**< Array containing colors for each LED */
	};

	/// Contains information about effect.
	struct CorsairEffect
	{
		Guid effectId;                                                       /**< Unique identifier of the effect */
		struct CorsairFrame* (*getFrameFunction)(Guid effectId, int offset); /**< Pointer to <lfxname>GetFrame function of the same library that should be called to retrieve the frame by effect identifier and offset */
		void (*freeFrameFunction)(struct CorsairFrame* pFrame);              /**< Pointer to <lfxname>FreeFrame function of the same library that should be called to free memory allocated by frame */
	};

#ifdef __cplusplus
} //exten "C"
#endif