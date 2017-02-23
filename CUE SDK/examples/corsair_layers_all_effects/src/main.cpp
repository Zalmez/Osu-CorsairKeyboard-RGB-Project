#include "CUELFX/CUELFX.h"
#include "CorsairLFX/CorsairLFX.h"
#include "CorsairLayers/CorsairLayers.h"

#include <Windows.h>
#include <iostream>
#include <vector>
#include <thread>
#include <conio.h>

#pragma warning(disable: 4996)

using CorsairLedList = std::vector<CorsairLedId>;

const char* errorString(CorsairError error)
{
	switch (error) {
	case CE_Success:
		return "Success";
	case CE_ServerNotFound:
		return "Server not found";
	case CE_NoControl:
		return "No control";
	case CE_ProtocolHandshakeMissing:
		return "Protocol handshake missing";
	case CE_IncompatibleProtocol:
		return "Incompatible protocol";
	case CE_InvalidArguments:
		return "Invalid Arguments";
	default:
		return "Unknown error";
	}
}

CorsairLedList getColorShiftLeds()
{
	CorsairLedList leds;
	for (int i = CLK_Escape; i <= CLK_Application; ++i) {
		leds.push_back(static_cast<CorsairLedId>(i));
	}
	for (int i = CLK_F12; i <= CLK_RightArrow; ++i) {
		leds.push_back(static_cast<CorsairLedId>(i));
	}
	leds.push_back(CLK_Fn);
	return leds;
}

CorsairLedList getSolidColorLeds()
{
	CorsairLedList leds;
	for (int i = CLK_4; i <= CLK_9; ++i) {
		leds.push_back(static_cast<CorsairLedId>(i));
	}
	for (int i = CLK_E; i <= CLK_O; ++i) {
		leds.push_back(static_cast<CorsairLedId>(i));
	}
	for (int i = CLK_D; i <= CLK_K; ++i) {
		leds.push_back(static_cast<CorsairLedId>(i));
	}
	for (int i = CLK_C; i <= CLK_M; ++i) {
		leds.push_back(static_cast<CorsairLedId>(i));
	}
	leds.push_back(CLK_Space);
	return leds;
}

CorsairLedList getGradientLeds()
{
	CorsairLedList leds;
	for (int i = CLK_PrintScreen; i <= CLK_PageUp; ++i) {
		leds.push_back(static_cast<CorsairLedId>(i));
	}
	for (int i = CLK_Delete; i <= CLK_PageDown; ++i) {
		leds.push_back(static_cast<CorsairLedId>(i));
	}
	for (int i = CLK_UpArrow; i <= CLK_RightArrow; ++i) {
		leds.push_back(static_cast<CorsairLedId>(i));
	}
	return leds;
}

CorsairLedList getProgressBarLeds()
{
	CorsairLedList leds;
	for (int i = CLK_Tab; i <= CLK_BracketLeft; ++i) {
		leds.push_back(static_cast<CorsairLedId>(i));
	}
	for (int i = CLK_CapsLock; i <= CLK_ApostropheAndDoubleQuote; ++i) {
		leds.push_back(static_cast<CorsairLedId>(i));
	}
	return leds;
}

int main(int argc, char* argv[]) 
{
	CorsairPerformProtocolHandshake();
	if (const auto error = CorsairGetLastError()) {
		std::cerr << "Protocol Handshake failed: " << errorString(error) << std::endl;
		system("pause");
		return -1;
	}

	if (!CorsairGetDeviceCount()) {
		std::cerr << "No devices found" << std::endl;
		system("pause");
		return -1;
	}

	auto ledPositions = CorsairGetLedPositions();
	if (!ledPositions) {
		std::cerr << "No led posiotns available" << std::endl;
		system("pause");
		return -1;
	}

	CUELFXSetLedPositions(ledPositions);
	CorsairLFXSetLedPositions(ledPositions);

	CorsairLayersInitialize(&CorsairSetLedsColorsAsync);

	auto solidColorLeds = getSolidColorLeds();
	auto solidColor = CUELFXCreateSolidColorEffect(static_cast<int>(solidColorLeds.size()), solidColorLeds.data(), { 50, 150, 200 });

	std::cout << "Play SolidColor effect on layer 5\nPress any key to play next step...\n";
	auto solidColorId = CorsairLayersPlayEffect(solidColor, 5);
	getch();

	auto colorShiftLeds = getColorShiftLeds();
	CorsairLightingEffectColorOptions colorShiftColorOptions{ CLECM_Random, { 255, 0, 0 }, { 0, 255, 0 } };
	auto colorShift = CUELFXCreateColorShiftEffect(static_cast<int>(colorShiftLeds.size()), colorShiftLeds.data(), CLES_Medium, colorShiftColorOptions);

	std::cout << "\nMix ColorShift effect with SolidColor on layer 5\nPress any key to play next step...\n";
	auto colorShiftId = CorsairLayersPlayEffect(colorShift, 5);
	getch();

	auto gradientLeds = getGradientLeds();
	auto gradient = CorsairLFXCreateGradientEffect(static_cast<int>(gradientLeds.size()), gradientLeds.data(), { 0, 0, 255 });
	CorsairLFXAddRampToGradientEffect(gradient->effectId, 2500, { 255, 255, 255 }, 3.);
	CorsairLFXAddRampToGradientEffect(gradient->effectId, 2000, { 125, 125, 0 }, 0.2);
	CorsairLFXAddRampToGradientEffect(gradient->effectId, 1000, { 0, 0, 0 }, -1.);
	CorsairLFXAddRampToGradientEffect(gradient->effectId, 3000, { 0, 0, 255 }, 1.);

	std::cout << "\nPlay Gradient effect with 4 Ramps on layer 10\nPress any key to play next step...\n";
	auto gradientId = CorsairLayersPlayEffect(gradient, 10);
	getch();

	auto progressBarLeds = getProgressBarLeds();
	auto progressBar = CorsairLFXCreateProgressBarEffect(static_cast<int>(progressBarLeds.size()), progressBarLeds.data(), { 255, 0, 0 }, { 255, 255, 255 });
	
	std::cout << "\nMix ProgressBar with effects on layer 5\nUse \"+\" or \"-\" to increase or decrease progress value\nPress Escape to play next step...\n";
	auto progressBarId = CorsairLayersPlayEffect(progressBar, 5);
	
	int progressValue = 0;
	while (!GetAsyncKeyState(VK_ESCAPE)) {
		if ((GetAsyncKeyState(VK_OEM_PLUS) & 0x1) && progressValue < 100) {
			progressValue += 5;
			CorsairLFXSetProgress(progressBar->effectId, progressValue);
		}
		if ((GetAsyncKeyState(VK_OEM_MINUS) & 0x1) && progressValue > 0) {
			progressValue -= 5;
			CorsairLFXSetProgress(progressBar->effectId, progressValue);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	std::cout << "\nStop all effects\n";
	CorsairLayersStopEffect(colorShiftId);
	CorsairLayersStopEffect(solidColorId);
	CorsairLayersStopEffect(gradientId);
	CorsairLayersStopEffect(progressBarId);
	system("pause");
	return 0;
}
