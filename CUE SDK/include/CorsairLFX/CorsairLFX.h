#pragma once

#include "CorsairLFXGlobal.h"
#include "Shared/LFX.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	 * @brief Gets the frame for specified effect id and offset.
	 * 
	 * @warning Client of this function is responsible for freeing memory used by CorsairFrame structure 
	 * and it's fields using CorsairLFXFreeFrame() function.
	 *
	 * @param effectId Unique identifier of the effect. Can be obtained from CorsairEffect struct.
	 * @param offset   Offset in milliseconds.
	 * @return Pointer to CorsairFrame structure or NULL if effect is finished at specified offset.
	 */
	CORSAIRLFX_EXPORT struct CorsairFrame *CorsairLFXGetFrame(Guid effectId, int offset);

	/**
	 * @brief Frees the memory allocated by CorsairFrame structure and its fields.
	 *        
	 * @attention The frame data cannot be accessed after call to this function.
	 *
	 * @param pFrame Pointer to the frame structure that has to be freed.
	 */
	CORSAIRLFX_EXPORT void CorsairLFXFreeFrame(struct CorsairFrame* pFrame);

	/**
	 * @brief Provides information about keyboard geometry to library.
	 *
	 * @warning This function has to be called before any calls to CorsairLFXGetFrame().
	 *
	 * @param ledPositions Pointer to structure with positions information retrieved using CUESDKGetLedPositions().
	 */
	CORSAIRLFX_EXPORT void CorsairLFXSetLedPositions(struct CorsairLedPositions *ledPositions);
	
	/**
	 * @brief Creates gradient effect.
	 *  
	 * @note In order to construct effect properly, Ramp has to be added using CorsairLFXAddRampToGradientEffect().
	 *
	 * @param size       Number of leds in pLeds array.
	 * @param pLeds      Array of CorsairLedId to be used in the effect.
	 * @param startColor Starting color of the gradient effect.
	 * @return Pointer to CorsairEffect structure.
	 */
	CORSAIRLFX_EXPORT struct CorsairEffect* CorsairLFXCreateGradientEffect(int size, enum CorsairLedId* pLeds, struct CorsairColor startColor);

	/**
	 * @brief Adds a ramp to gradient effect.
	 *
	 * The ramp is added to the tail of effect. Multiple ramps could be added to the same
	 * gradient effect. When effect is played color of keys is gradually changed from
	 * ramp start color (which is same as effect startColor for first ramp and the same as
	 * endColor of previous ramp for next ramps) using curve specified by power parameter.
	 *
	 * @param effectId Unique identifier of the effect. Can be obtained from CorsairEffect struct.
	 * @param duration Duration of the ramp in milliseconds.
	 * @param endColor Color for the end of the ramp.
	 * @param power    Shape of curve that will be used for ramp.
	 */
	CORSAIRLFX_EXPORT void CorsairLFXAddRampToGradientEffect(Guid effectId, int duration, struct CorsairColor endColor, double power);

	/**
	 * @brief Ceates a progress bar on the keyboard.
	 *
	 * After creation the level of the progress is 0 by default, call CorsairLFXSetProgress()
	 * to change its value.
	 *
	 * @param size            Number of leds in pLeds array.
	 * @param pLeds           Array of CorsairLedId to be used in the effect.
	 * @param foregroundColor Color for keys which represent active part of progress.
	 * @param backgroundColor Color for the rest of keys of the progress bar.
	 * @return Pointer to CorsairEffect structure.
	 */
	CORSAIRLFX_EXPORT struct CorsairEffect* CorsairLFXCreateProgressBarEffect(int size, enum CorsairLedId* pLeds, struct CorsairColor foregroundColor, struct CorsairColor backgroundColor);

	/**
	 * @brief Updates the level of progress.
	 * @param effectId      Unique identifier of the effect. Can be obtained from CorsairEffect struct.
	 * @param progressValue The level of the progress within a range [0..100].
	 */
	CORSAIRLFX_EXPORT void CorsairLFXSetProgress(Guid effectId, int progressValue);

	/**
	 * @brief Hides progress bar from the keyboard.
	 * 
	 * After calling this function CorsairLFXGetFrame() will return NULL if you call it
	 * with progress bar effectId.
	 *
	 * @param effectId Unique identifier of the effect. Can be obtained from CorsairEffect struct.
	 */
	CORSAIRLFX_EXPORT void CorsairLFXHideProgressBar(Guid effectId);
#ifdef __cplusplus
} //exten "C"
#endif
