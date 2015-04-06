#ifndef _MAGICPANEL_h
#define _MAGICPANEL_h

#include <Wire.h>
#include <LedControl.h>
#include "I2C_Common.h"

const int RANDOMPLAY = 5000;
const int RANDOMSLEEP = 30000;

class MagicPanel
{
	private:
		LedControl *ledControl;
		I2C_DeviceAddress::Value i2cAddress;
		bool vMagicPanel[8][8];
		unsigned char panel[16];
		I2C_MagicPanel_Mode::Value currentmode;
		unsigned long lastTimeCheck;

		int state;
		int state2;
		int direction;
		bool modeActive;
		bool randomActive;
		unsigned long lastRandomTimeCheck;

		void AnimateAlert();
		void AnimateToggleTopBottom();
		void AnimateToggleLeftRight();
		void AnimateToggleQuad();
		void AnimateQuadCycleCounterClockwise();
		void AnimateQuadCycleClockwise();
		void AnimateTraceUp();
		void AnimateTraceDown();
		void AnimateTraceUpDown();
		void AnimateTraceLeft();
		void AnimateTraceRight();
		void AnimateTraceLeftRight();
		void AnimateSingleLEDTest();
		void AnimateDoubleLEDTest();
		void AnimateRandomPixel();
		void AnimateString();
		bool feedGridLeftComplete;
		int letterIndex;
		String englishString;

		void MapAndPrint();
		void MapBoolGrid();
		void PrintGrid();
		bool IsTimeForStateChange(int delay, bool clear = false);
		void SetRow(int ledRow, unsigned char rowState);
		void SetCol(int ledCol, unsigned char colState);
		unsigned char GetCol(int ledCol);
		void ClearButDoNotShow(bool isOn);
		void DrawLetter(char letter);
		void FeedGridLeft(unsigned char newColumn);
		void ResetModeVariables();

	public:
		MagicPanel(I2C_DeviceAddress::Value i2cAddress, LedControl *led);

		void ProcessCommand();
		void Update();

		void SetBrightness(int brightness);
		void Off();
		void On();
		void SetMode(I2C_MagicPanel_Mode::Value mode);
		void Random();
};

#endif
