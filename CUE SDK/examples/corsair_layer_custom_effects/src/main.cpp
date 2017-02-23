#include "CorsairLayers/CorsairLayers.h"

#include <iostream>
#include <memory>
#include <vector>
#include <conio.h>

#pragma warning(disable: 4996)

namespace 
{
	using CorsairColorList = std::vector<CorsairLedColor>;

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

	CorsairFrame* getFrameFunc(Guid effectId, int offset);
	void freeFrameFunc(CorsairFrame *frame);

	class Effect
	{
	public:

		explicit Effect(const std::vector<CorsairLedColor>& colors)
			: effectId(0), mColors(colors), stopEffect(false)
		{
		}

		virtual ~Effect() {}

		CorsairEffect* effect() 
		{
			if (!mEffect) {
				mEffect = std::unique_ptr<CorsairEffect>(new CorsairEffect);
				mEffect->effectId = reinterpret_cast<Guid>(this);
				effectId = mEffect->effectId;
				mEffect->getFrameFunction = getFrameFunc;
				mEffect->freeFrameFunction = freeFrameFunc;
			}
			return mEffect.get();
		}

		virtual CorsairFrame* getFrame(Guid effectId, int offset) 
		{
			if (reinterpret_cast<Effect*>(effectId) != this || stopEffect) {
				return nullptr;
			} else {
				CorsairFrame *frame = new CorsairFrame;
				frame->size = static_cast<int>(mColors.size());
				frame->ledsColors = new CorsairLedColor[mColors.size()];
				std::copy(mColors.begin(), mColors.end(), frame->ledsColors);
				return frame;
			}
		}

		Guid effectId;
		bool stopEffect;

	private:
		std::vector<CorsairLedColor> mColors;
		std::unique_ptr<CorsairEffect> mEffect;
	};

	class BlinkEffect : public Effect 
	{
	public:

		explicit BlinkEffect(const std::vector<CorsairLedColor>& colors)
			: Effect(colors)
		{
		}

		CorsairFrame* getFrame(Guid effectId, int offset) override 
		{
			if ((offset / 500) % 2 ) {
				return Effect::getFrame(effectId, offset);
			} else {
				CorsairFrame *frame = new CorsairFrame;
				frame->size = 0;
				frame->ledsColors = nullptr;
				return frame;
			}
		}
	};

	CorsairFrame* getFrameFunc(Guid effectId, int offset) 
	{
		if (effectId) {
			return reinterpret_cast<Effect*>(effectId)->getFrame(effectId, offset);
		} else {
			return nullptr;
		}
	}

	void freeFrameFunc(CorsairFrame *frame)
	{
		if (frame) {
			delete[] frame->ledsColors;
			delete frame;
		}
	}

	std::vector<CorsairLedColor> getFirstColorList(const CorsairColor &baseColor)
	{
		std::vector<CorsairLedColor> colors;
		for (int i = CLK_GraveAccentAndTilde; i <= CLK_6; ++i) {
			colors.push_back({ static_cast<CorsairLedId>(i), baseColor.r, baseColor.g, baseColor.b });
		}
		for (int i = CLK_Tab; i <= CLK_E; ++i) {
			colors.push_back({ static_cast<CorsairLedId>(i), baseColor.r, baseColor.g, baseColor.b });
		}
		colors.push_back({ CLK_R, 0, 0, 0 });
		colors.push_back({ CLK_T, 0, 0, 0 });
		colors.push_back({ CLK_Y, baseColor.r, baseColor.g, baseColor.b });
		for (int i = CLK_CapsLock; i <= CLK_D; ++i) {
			colors.push_back({ static_cast<CorsairLedId>(i), baseColor.r, baseColor.g, baseColor.b });
		}
		colors.push_back({ CLK_F, 0, 0, 0 });
		colors.push_back({ CLK_G, 0, 0, 0 });
		colors.push_back({ CLK_H, baseColor.r, baseColor.g, baseColor.b });
		for (int i = CLK_LeftShift; i <= CLK_B; ++i) {
			colors.push_back({ static_cast<CorsairLedId>(i), baseColor.r, baseColor.g, baseColor.b });
		}
		return colors;
	}
	
	CorsairColorList getSecondColorList(const CorsairColor &baseColor)
	{
		CorsairColorList colors;
		for (int i = CLK_4; i <= CLK_MinusAndUnderscore; ++i) {
			colors.push_back({ static_cast<CorsairLedId>(i), baseColor.r, baseColor.g, baseColor.b });
		}
		for (int i = CLK_E; i <= CLK_BracketLeft; ++i) {
			colors.push_back({ static_cast<CorsairLedId>(i), baseColor.r, baseColor.g, baseColor.b });
		}
		for (int i = CLK_D; i <= CLK_ApostropheAndDoubleQuote; ++i) {
			colors.push_back({ static_cast<CorsairLedId>(i), baseColor.r, baseColor.g, baseColor.b });
		}
		for (int i = CLK_C; i <= CLK_SlashAndQuestionMark; ++i) {
			colors.push_back({ static_cast<CorsairLedId>(i), baseColor.r, baseColor.g, baseColor.b });
		}
		return colors;
	}

	CorsairColorList getThirdColorList(const CorsairColor &baseColor)
	{
		CorsairColorList colors;
		for (int i = CLK_F3; i <= CLK_F5; ++i) {
			colors.push_back({ static_cast<CorsairLedId>(i), baseColor.r, baseColor.g, baseColor.b });
		}
		for (int i = CLK_3; i <= CLK_8; ++i) {
			colors.push_back({ static_cast<CorsairLedId>(i), baseColor.r, baseColor.g, baseColor.b });
		}
		for (int i = CLK_W; i <= CLK_U; ++i) {
			colors.push_back({ static_cast<CorsairLedId>(i), baseColor.r, baseColor.g, baseColor.b });
		}
		for (int i = CLK_S; i <= CLK_J; ++i) {
			colors.push_back({ static_cast<CorsairLedId>(i), baseColor.r, baseColor.g, baseColor.b });
		}
		for (int i = CLK_X; i <= CLK_N; ++i) {
			colors.push_back({ static_cast<CorsairLedId>(i), baseColor.r, baseColor.g, baseColor.b });
		}
		colors.push_back({ CLK_Space, baseColor.r, baseColor.g, baseColor.b });
		return colors;
	}
}

int main(int argc, char* argv[]) 
{
	CorsairPerformProtocolHandshake();
	if (const auto error = CorsairGetLastError()) {
		std::cerr << "Protocol Handshake failed: " << ::errorString(error) << std::endl;
		system("pause");
		return -1;
	}

	if (!CorsairGetDeviceCount()) {
		std::cerr << "No devices found" << std::endl;
		system("pause");
		return -1;
	}

	CorsairLayersInitialize(&CorsairSetLedsColorsAsync);

	Effect effect1(getFirstColorList({ 255, 0, 0 }));
	
	std::cout << "Play first effect with black-color center\nPress any key to play next step...\n";
	auto effect1Id = CorsairLayersPlayEffect(effect1.effect(), 1);
	getch();
	
	Effect effect2(getSecondColorList({ 0, 0, 255 }));
	
	std::cout << "\nMix second effect with the first\nPress any key to play next step...\n";
	auto effect2Id = CorsairLayersPlayEffect(effect2.effect(), 1);
	getch();

	BlinkEffect effect3(getThirdColorList({ 0, 255, 0 }));

	std::cout << "\nPlay third color on the topmost layer\nPress any key to play next step...\n";
	auto effect3Id = CorsairLayersPlayEffect(effect3.effect(), 2);
	getch();

	std::cout << "\nStop third effect and then mix it with the first and the second\nPress any key to play next step...\n";
	CorsairLayersStopEffect(effect3Id);
	effect3Id = CorsairLayersPlayEffect(effect3.effect(), 1);
	getch();

	std::cout << "\nStop second and third effects and play third effect under the first\nPress any key to play next step...\n";
	CorsairLayersStopEffect(effect2Id);
	CorsairLayersStopEffect(effect3Id);
	effect3Id = CorsairLayersPlayEffect(effect3.effect(), 0);
	getch();

	std::cout << "\nStop all effects\nPress any key to finish...\n";
	CorsairLayersStopEffect(effect1Id);
	CorsairLayersStopEffect(effect3Id);
	getch();
	return 0;
}
