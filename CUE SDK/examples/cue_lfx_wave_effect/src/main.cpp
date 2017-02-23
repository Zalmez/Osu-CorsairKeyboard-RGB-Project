#include "CUESDK.h"
#include "CUELFX/CUELFX.h"
#include "Shared/LFX.h"

#include <Windows.h>

#include <iostream>
#include <vector>
#include <thread>

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
	for (int i = CLI_Invalid; i <= CLI_Last; ++i) {
		leds.push_back(static_cast<CorsairLedId>(i));
	}
	return leds;
}

void playEffect(CorsairEffect *effect)
{
	auto startPoint = Clock::now();

	while (!GetAsyncKeyState(VK_ESCAPE)) {
		
		auto offset = std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - startPoint).count();
		auto frame = CUELFXGetFrame(effect->effectId, static_cast<int>(offset));
		
		if (frame && frame->ledsColors) {
			auto res = CorsairSetLedsColors(frame->size, frame->ledsColors);
			CUELFXFreeFrame(frame);

			if (!res) {
				std::cerr << "Failed to set led colors: " << errorString(CorsairGetLastError()) << std::endl;
				return;
			}
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

	CUELFXSetLedPositions(ledPositions);
	
	auto leds = getLeds();

	double tailLength = 5.;
	double velocity = 10.;
	int duration = 7;
	int angle = -30;
	bool isDoubleSided = false;
	int repeatCount = 0;

	std::cout << "Create Wave effect with parameters:\n";
	std::cout << "    tail length: " << tailLength << '\n';
	std::cout << "       velocity: " << velocity << '\n';
	std::cout << "       duration: " << duration << '\n';
	std::cout << "          angle: " << angle << '\n';
	std::cout << "   double sided: " << (isDoubleSided ? "true" : "false") << '\n';
	std::cout << "   repeat count: " << repeatCount << '\n';

	auto effect = CUELFXCreateWaveEffect(static_cast<int>(leds.size()), leds.data(), tailLength, velocity, duration, angle, isDoubleSided, repeatCount);

	if (!effect) {
		std::cerr << "Failed to create Wave Effect" << std::endl;
		system("pause");
		return -1;
	}
	
	CorsairColor firstColor{ 0, 255, 0 };
	CorsairColor secondColor{ 255, 0, 0 };

	std::cout << "Add color (" << firstColor.r << ", " << firstColor.g << ", " << firstColor.b << ") at position 0.0\n";
	std::cout << "Add color (" << secondColor.r << ", " << secondColor.g << ", " << secondColor.b << ") at position 1.0\n";

	CUELFXAddPointToEffect(effect->effectId, 0., firstColor);
	CUELFXAddPointToEffect(effect->effectId, 1., secondColor);
	
	std::cout << "Playing effect...\nPress Escape to stop playback\n";
	playEffect(effect);
	system("pause");
	return 0;
}