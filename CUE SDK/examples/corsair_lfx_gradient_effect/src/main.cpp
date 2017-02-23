#include "CUESDK.h"
#include "CorsairLFX/CorsairLFX.h"
#include "Shared/LFX.h"

#include <Windows.h>

#include <iostream>
#include <vector>
#include <thread>

const int cDefaultDuration = 1000;
const double cDefaultPower = 1.0;

using Clock = std::chrono::steady_clock;

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

std::vector<CorsairLedId> getLeds()
{
	std::vector<CorsairLedId> leds;
	for (int i = CLK_Q; i <= CLK_BracketLeft; ++i) {
		leds.push_back(static_cast<CorsairLedId>(i));
	}
	for (int i = CLK_A; i <= CLK_ApostropheAndDoubleQuote; ++i) {
		leds.push_back(static_cast<CorsairLedId>(i));
	}
	for (int i = CLK_Z; i <= CLK_SlashAndQuestionMark; ++i) {
		leds.push_back(static_cast<CorsairLedId>(i));
	}

	return leds;
}

void generateRamp(CorsairEffect *effect)
{
	CorsairColor color;
	color.r = std::rand() % 255;
	color.g = std::rand() % 255;
	color.b = std::rand() % 255;

	std::cout << "Create new Ramp for gradient\n";
	std::cout << "Generated color: (" << color.r << ", " << color.g << ", " << color.b << ")\n";

	int duration = 0;
	std::cout << "Input duration: ";
	std::cin >> duration;

	if (std::cin.fail()) {
		std::cin.clear();
		std::cin.ignore();
		duration = cDefaultDuration;
		std::cerr << "Invalid input value, default is selected: " << duration << '\n';
	}

	double power = 0;
	std::cout << "Input power: ";
	std::cin >> power;

	if (std::cin.fail()) {
		std::cin.clear();
		std::cin.ignore();
		power = cDefaultPower;
		std::cerr << "Invalid input value, default is selected: " << power << '\n';
	}

	CorsairLFXAddRampToGradientEffect(effect->effectId, duration, color, power);
}

void playEffect(CorsairEffect *effect)
{
	auto startPoint = Clock::now();

	while (!GetAsyncKeyState(VK_ESCAPE)) {
		auto offset = std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - startPoint).count();
		auto frame = CorsairLFXGetFrame(effect->effectId, static_cast<int>(offset));
		
		if (frame && frame->ledsColors) {
			auto res = CorsairSetLedsColors(frame->size, frame->ledsColors);
			CorsairLFXFreeFrame(frame);

			if (!res) {
				std::cerr << "Failed to set led colors: " << errorString(CorsairGetLastError()) << std::endl;
				return;
			}

			if (GetAsyncKeyState(VK_OEM_PLUS) & 0x1) {
				generateRamp(effect);
				startPoint = Clock::now();
			}
		} else {
			break;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(25));
	}
}

int main(int argc, char *argv[])
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

	CorsairLFXSetLedPositions(ledPositions);
	
	auto leds = getLeds();

	CorsairColor startColor{ 255, 255, 255 };

	auto effect = CorsairLFXCreateGradientEffect(static_cast<int>(leds.size()), leds.data(), startColor);

	if (!effect) {
		std::cerr << "Failed to create Gradient Effect" << std::endl;
		system("pause");
		return -1;
	}

	std::srand(1000);
	generateRamp(effect);
	
	std::cout << "\nPlaying effect...\nUse \"+\" to interrupt playback and add new Ramp\nPress Escape to stop playback\n\n";
	playEffect(effect);
	system("pause");
	return 0;
}