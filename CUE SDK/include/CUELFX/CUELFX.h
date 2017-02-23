#pragma once

#include "CUELFXGlobal.h"
#include "Shared/LFX.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/// Contains list of effect speeds.
	enum CorsairLightingEffectSpeed 
	{
		CLES_Slow,
		CLES_Medium,
		CLES_Fast
	};

	/// Contains list of circular effect directions.
	enum CorsairLightingEffectCircularDirection 
	{
		CLECD_Clockwise,
		CLECD_CounterClockwise
	};

	/// Contains list of linear effect directions.
	enum CorsairLightingEffectLinearDirection
	{
		CLELD_Left,
		CLELD_Right,
		CLELD_Up,
		CLELD_Down,
	};

	/// Contains list of color modes.
	enum CorsairLightingEffectColorMode
	{
		CLECM_Random,
		CLECM_Alternating
	};

	/// Color options to be used in the effect.
	struct CorsairLightingEffectColorOptions 
	{
		enum CorsairLightingEffectColorMode mode;  /**< Enum describing color mode for the effect */
		struct CorsairColor color1;                /**< First color is only used if mode is set to CLECM_Alternating */
		struct CorsairColor color2;                /**< Second color is only used if mode is set to CLECM_Alternating */

	};

	/**
	 * @brief Gets the frame for specified effect id and offset.
	 *
	 * @warning Client of this function is responsible for freeing memory used by CorsairFrame structure
	 * and it's fields using CUELFXFreeFrame() function.
	 *
	 * @param effectId Unique identifier of the effect. Can be obtained from CorsairEffect struct.
	 * @param offset   Offset in milliseconds.
	 * @return Pointer to CorsairFrame structure or NULL if effect is finished at specified offset.
	 */
	CUELFX_EXPORT struct CorsairFrame *CUELFXGetFrame(Guid effectId, int offset);

	/**
	 * @brief Frees the memory allocated by CorsairFrame structure and its fields.
	 *       
	 * @attention The frame data cannot be accessed after call to this function.
	 *
	 * @param pFrame Pointer to the frame structure that has to be freed.
	 */
	CUELFX_EXPORT void CUELFXFreeFrame(struct CorsairFrame *pFrame);

	/**
	 * @brief Provides information about keyboard geometry to library.
	 *
	 * @warning This function has to be called before any calls to CUELFXGetFrame().
	 *
	 * @param ledPositions Pointer to structure with positions information retrieved using CUESDKGetLedPositions().
	 */
	CUELFX_EXPORT void CUELFXSetLedPositions(struct CorsairLedPositions *ledPositions);

	/**
	 * @brief Creates spiral rainbow effect.
	 * @param size      Number of leds in pLeds array.
	 * @param pLeds     Array of CorsairLedId to be used in the effect.
	 * @param speed     Speed of the effect.
	 * @param direction Circular direction of the effect.
	 * @return Pointer to CorsairEffect structure.
	 */
	CUELFX_EXPORT struct CorsairEffect *CUELFXCreateSpiralRainbowEffect(int size, enum CorsairLedId *pLeds, enum CorsairLightingEffectSpeed speed, enum CorsairLightingEffectCircularDirection direction);

	/**
	 * @brief Creates rainbow wave effect.
	 * @param size      Number of leds in pLeds array.
	 * @param pLeds     Array of CorsairLedId to be used in the effect.
	 * @param speed     Speed of the effect.
	 * @param direction Linear direction of the effect.
	 * @return Pointer to CorsairEffect structure.
	 */
	CUELFX_EXPORT struct CorsairEffect *CUELFXCreateRainbowWaveEffect(int size, enum CorsairLedId *pLeds, enum CorsairLightingEffectSpeed speed, enum CorsairLightingEffectLinearDirection direction);

	/**
	 * @brief Creates visor effect.
	 * @param size         Number of leds in pLeds array.
	 * @param pLeds        Array of CorsairLedId to be used in the effect.
	 * @param speed        Speed of the effect.
	 * @param colorOptions Color option to be used in the effect.
	 * @return Pointer to CorsairEffect structure.
	 */
	CUELFX_EXPORT struct CorsairEffect *CUELFXCreateVisorEffect(int size, enum CorsairLedId *pLeds, enum CorsairLightingEffectSpeed speed, struct CorsairLightingEffectColorOptions colorOptions);

	/**
	 * @brief Creates rain effect.
	 * @param size         Number of leds in pLeds array.
	 * @param pLeds        Array of CorsairLedId to be used in the effect.
	 * @param speed        Speed of the effect.
	 * @param colorOptions Color option to be used in the effect.
	 * @return Pointer to CorsairEffect structure.
	 */
	CUELFX_EXPORT struct CorsairEffect *CUELFXCreateRainEffect(int size, enum CorsairLedId *pLeds, enum CorsairLightingEffectSpeed speed, struct CorsairLightingEffectColorOptions colorOptions);

	/**
	 * @brief Creates color shift effect.
	 * @param size         Number of leds in pLeds array.
	 * @param pLeds        Array of CorsairLedId to be used in the effect.
	 * @param speed        Speed of the effect.
	 * @param colorOptions Color option to be used in the effect.
	 * @return Pointer to CorsairEffect structure.
	 */
	CUELFX_EXPORT struct CorsairEffect *CUELFXCreateColorShiftEffect(int size, enum CorsairLedId *pLeds, enum CorsairLightingEffectSpeed speed, struct CorsairLightingEffectColorOptions colorOptions);

	/**
	 * @brief Creates color pulse effect.
	 * @param size         Number of leds in pLeds array.
	 * @param pLeds        Array of CorsairLedId to be used in the effect.
	 * @param speed        Speed of the effect.
	 * @param colorOptions Color option to be used in the effect.
	 * @return Pointer to CorsairEffect structure.
	 */
	CUELFX_EXPORT struct CorsairEffect *CUELFXCreateColorPulseEffect(int size, enum CorsairLedId *pLeds, enum CorsairLightingEffectSpeed speed, struct CorsairLightingEffectColorOptions colorOptions);

	/**
	 * @brief Creates color wave effect.
	 * @param size         Number of leds in pLeds array.
	 * @param pLeds        Array of CorsairLedId to be used in the effect.
	 * @param speed        Speed of the effect.
	 * @param direction    Linear direction of the effect.
	 * @param colorOptions Color option to be used in the effect.
	 * @return Pointer to CorsairEffect structure.
	 */
	CUELFX_EXPORT struct CorsairEffect *CUELFXCreateColorWaveEffect(int size, enum CorsairLedId *pLeds, enum CorsairLightingEffectSpeed speed, enum CorsairLightingEffectLinearDirection direction, struct CorsairLightingEffectColorOptions colorOptions);

	/**
	 * @brief Creates ripple effect.
	 *
	 * @note In order to construct effect properly, intensity points has to be added using CUELFXAddPointToEffect().
	 *
	 * @param size        Number of leds in pLeds array.
	 * @param pLeds       Array of CorsairLedId to be used in the effect.
	 * @param tail        Tail size (in LED units).
	 * @param velocity    Velocity (in LED/second).
	 * @param duration    Duration in milliseconds.
	 * @param repeatCount Number of times to repeat this effect. Specify 0 to run effect continuously.
	 * @return Pointer to CorsairEffect structure.
	 */
	CUELFX_EXPORT struct CorsairEffect *CUELFXCreateRippleEffect(int size, enum CorsairLedId *pLeds, double tail, double velocity, int duration, int repeatCount);

	/**
	 * @brief Creates wave effect.
	 *
	 * @note In order to construct effect properly, intensity points has to be added using CUELFXAddPointToEffect().
	 *
	 * @param size         Number of leds in pLeds array.
	 * @param pLeds        Array of CorsairLedId to be used in the effect.
	 * @param tail         Tail size (in LED units).
	 * @param velocity     Velocity (in LED/second).
	 * @param duration     Duration in milliseconds.
	 * @param orientation  Orientation in degrees within a range [0..359].
	 * @param boolTwoSided Specifies if the effect is one or two sided.
	 * @param repeatCount  Number of times to repeat this effect. Specify 0 to run effect continuously.
	 * @return Pointer to CorsairEffect structure.
	 */
	CUELFX_EXPORT struct CorsairEffect *CUELFXCreateWaveEffect(int size, enum CorsairLedId *pLeds, double tail, double velocity, int duration, int orientation, int boolTwoSided, int repeatCount);

	/**
	 * @brief Creates solid color effect.
	 * @param size  Number of leds in pLeds array.
	 * @param pLeds Array of CorsairLedId to be used in the effect.
	 * @param color Color of the effect.
	 * @return Pointer to CorsairEffect structure.
	 */
	CUELFX_EXPORT struct CorsairEffect *CUELFXCreateSolidColorEffect(int size, enum CorsairLedId *pLeds, struct CorsairColor color);

	/**
	 * @brief Creates rainbow pulse effect.
	 * @param size  Number of leds in pLeds array.
	 * @param pLeds Array of CorsairLedId to be used in the effect.
	 * @param speed Speed of the effect.
	 * @return Pointer to CorsairEffect structure.
	 */
	CUELFX_EXPORT struct CorsairEffect *CUELFXCreateRainbowPulseEffect(int size, enum CorsairLedId *pLeds, enum CorsairLightingEffectSpeed speed);

	/**
	 * @brief Creates single blink effect.
	 * @param size  Number of leds in pLeds array.
	 * @param pLeds Array of CorsairLedId to be used in the effect.
	 * @param color Color of the effect.
	 * @return Pointer to CorsairEffect structure.
	 */
	CUELFX_EXPORT struct CorsairEffect *CUELFXCreateSingleBlinkEffect(int size, enum CorsairLedId *pLeds, struct CorsairColor color);

	/**
	 * @brief Creates double blink effect.
	 * @param size  Number of leds in pLeds array.
	 * @param pLeds Array of CorsairLedId to be used in the effect.
	 * @param color Color of the effect.
	 * @return Pointer to CorsairEffect structure.
	 */
	CUELFX_EXPORT struct CorsairEffect *CUELFXCreateDoubleBlinkEffect(int size, enum CorsairLedId *pLeds, struct CorsairColor color);

	/**
	 * @brief Creates rapid blink effect.
	 * @param size  Number of leds in pLeds array.
	 * @param pLeds Array of CorsairLedId to be used in the effect.
	 * @param color Color of the effect.
	 * @return Pointer to CorsairEffect structure.
	 */
	CUELFX_EXPORT struct CorsairEffect *CUELFXCreateRapidBlinkEffect(int size, enum CorsairLedId *pLeds, struct CorsairColor color);

	/**
	 * @brief Creates alternating rapid blink effect.
	 * @param size        Number of leds in pLeds array.
	 * @param pLeds       Array of CorsairLedId to be used in the effect.
	 * @param firstColor  First color of the effect.
	 * @param secondColor Second color of the effect.
	 * @return Pointer to CorsairEffect structure.
	 */
	CUELFX_EXPORT struct CorsairEffect *CUELFXCreateAlternatingRapidBlinkEffect(int size, enum CorsairLedId *pLeds, struct CorsairColor firstColor, struct CorsairColor secondColor);

	/**
	 * @brief Creates heart beat effect.
	 * @param size  Number of leds in pLeds array.
	 * @param pLeds Array of CorsairLedId to be used in the effect.
	 * @param color Color of the effect.
	 * @return Pointer to CorsairEffect structure.
	 */
	CUELFX_EXPORT struct CorsairEffect *CUELFXCreateHeartbeatEffect(int size, enum CorsairLedId *pLeds, struct CorsairColor color);
	
	/**
	 * @brief Creates off beat effect.
	 * @param size  Number of leds in pLeds array.
	 * @param pLeds Array of CorsairLedId to be used in the effect.
	 * @param color Color of the effect.
	 * @return Pointer to CorsairEffect structure.
	 */
	CUELFX_EXPORT struct CorsairEffect *CUELFXCreateOffBeatEffect(int size, enum CorsairLedId *pLeds, struct CorsairColor color);
	
	/**
	 * @brief Creates breathe effect.
	 * @param size  Number of leds in pLeds array.
	 * @param pLeds Array of CorsairLedId to be used in the effect.
	 * @param color Color of the effect.
	 * @return Pointer to CorsairEffect structure.
	 */
	CUELFX_EXPORT struct CorsairEffect *CUELFXCreateBreatheEffect(int size, enum CorsairLedId *pLeds, struct CorsairColor color);

	/**
	 * @brief Creates slow breathe effect.
	 * @param size  Number of leds in pLeds array.
	 * @param pLeds Array of CorsairLedId to be used in the effect.
	 * @param color Color of the effect.
	 * @return Pointer to CorsairEffect structure.
	 */
	CUELFX_EXPORT struct CorsairEffect *CUELFXCreateSlowBreatheEffect(int size, enum CorsairLedId *pLeds, struct CorsairColor color);

	/**
	 * @brief Creates slow long breathe effect.
	 * @param size  Number of leds in pLeds array.
	 * @param pLeds Array of CorsairLedId to be used in the effect.
	 * @param color Color of the effect.
	 * @return Pointer to CorsairEffect structure.
	 */
	CUELFX_EXPORT struct CorsairEffect *CUELFXCreateSlowLongBreatheEffect(int size, enum CorsairLedId *pLeds, struct CorsairColor color);

	/**
	 * @brief Adds point to specified wave or ripple effect intensity chart.
	 * @param effectId Unique identifier of the effect. Can be obtained from CorsairEffect struct.
	 * @param position Position to insert the point within a range [0..1].
	 * @param color    The color of the point to insert.
	 */
	CUELFX_EXPORT void CUELFXAddPointToEffect(Guid effectId, double position, CorsairColor color);

#ifdef __cplusplus
} //exten "C"
#endif
