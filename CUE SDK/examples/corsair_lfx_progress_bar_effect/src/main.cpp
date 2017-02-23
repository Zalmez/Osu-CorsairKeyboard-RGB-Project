#include "CUESDK.h"
#include "CorsairLFX/CorsairLFX.h"
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
	for (int i = CLK_Escape; i <= CLK_RightArrow; ++i) {
		leds.push_back(static_cast<CorsairLedId>(i));
	}
	leds.push_back(CLK_Fn);

	return leds;
}

void playEffect(CorsairEffect *effect)
{
	auto startPoint = Clock::now();
	int progressValue = 0;

	while (true) {
		
		auto offset = std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - startPoint).count();
		auto frame = CorsairLFXGetFrame(effect->effectId, static_cast<int>(offset));
		
		if (frame && frame->ledsColors) {
			auto res = CorsairSetLedsColors(frame->size, frame->ledsColors);
			CorsairLFXFreeFrame(frame);

			if ((GetAsyncKeyState(VK_OEM_PLUS) & 0x1) && progressValue < 100) {
				progressValue += 5;
				CorsairLFXSetProgress(effect->effectId, progressValue);
				std::cout << "Set progress to: " << progressValue << '\n';
			}
			if ((GetAsyncKeyState(VK_OEM_MINUS) & 0x1) && progressValue > 0) {
				progressValue -= 5;
				CorsairLFXSetProgress(effect->effectId, progressValue);
				std::cout << "Set progress to: " << progressValue << '\n';
			}
			if (GetAsyncKeyState(VK_ESCAPE)) {
				CorsairLFXHideProgressBar(effect->effectId);
			}
			
			if (!res) {
				std::cerr << "Failed to set led colors: " << errorString(CorsairGetLastError()) << std::endl;
				return;
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

	CorsairColor foregroundColor{ 255, 0, 0 };
	CorsairColor backgroundColor{ 0, 0, 255 };

	auto effect = CorsairLFXCreateProgressBarEffect(static_cast<int>(leds.size()), leds.data(), foregroundColor, backgroundColor);

	if (!effect) {
		std::cerr << "Failed to create ProgressBar Effect" << std::endl;
		system("pause");
		return -1;
	}
	
	std::cout << "Playing effect...\nUse \"+\" or \"-\" to increase or decrease progress value.\nPress Escape to hide progress bar\n";
	playEffect(effect);
	system("pause");
	return 0;
}