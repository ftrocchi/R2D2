#include "MagicPanel.h"

MagicPanel::MagicPanel(I2C_DeviceAddress::Value address, LedControl *led)
{
	randomSeed(analogRead(A3)); 

	i2cAddress = address;
	ledControl = led;

	ResetModeVariables();

	feedGridLeftComplete = true;
	letterIndex = 0;

	englishString = "HELLO R2 BUILDERS!   ";
}

void MagicPanel::Update()
{
	if (randomActive)
	{
		if (millis() - lastRandomTimeCheck > RANDOMPLAY && modeActive)
		{
			lastRandomTimeCheck = millis();
			modeActive = false;
			Off();
		}
		else if (millis() - lastRandomTimeCheck > RANDOMSLEEP && !modeActive)
		{
			SetMode((I2C_MagicPanel_Mode::Value)random(14));
			lastRandomTimeCheck = millis();
			modeActive = true;
		}
	}

	if (modeActive)
	{
		switch (currentmode)
		{
			case I2C_MagicPanel_Mode::Alert:
				AnimateAlert();
				break;

			case I2C_MagicPanel_Mode::ToggleTopBottom:
				AnimateToggleTopBottom();
				break;

			case I2C_MagicPanel_Mode::ToggleLeftRight:
				AnimateToggleLeftRight();
				break;

			case I2C_MagicPanel_Mode::ToggleQuad:
				AnimateToggleQuad();
				break;

			case I2C_MagicPanel_Mode::QuadCycleCounterClockwise:
				AnimateQuadCycleCounterClockwise();
				break;

			case I2C_MagicPanel_Mode::QuadCycleClockwise:
				AnimateQuadCycleClockwise();
				break;

			case I2C_MagicPanel_Mode::TraceUp:
				AnimateTraceUp();
				break;

			case I2C_MagicPanel_Mode::TraceDown:
				AnimateTraceDown();
				break;

			case I2C_MagicPanel_Mode::TraceUpDown:
				AnimateTraceUpDown();
				break;

			case I2C_MagicPanel_Mode::TraceLeft:
				AnimateTraceLeft();
				break;

			case I2C_MagicPanel_Mode::TraceRight:
				AnimateTraceRight();
				break;

			case I2C_MagicPanel_Mode::TraceLeftRight:
				AnimateTraceLeftRight();
				break;

			case I2C_MagicPanel_Mode::SingleLEDTest:
				AnimateSingleLEDTest();
				break;

			case I2C_MagicPanel_Mode::DoubleLEDTest:
				AnimateDoubleLEDTest();
				break;

			case I2C_MagicPanel_Mode::RandomPixel:
				AnimateRandomPixel();
				break;

			case I2C_MagicPanel_Mode::String:
				AnimateString();
				break;
		};
	}
}

void MagicPanel::ProcessCommand()
{
	randomActive = false;

	I2C_MagicPanel_Command::Value command = (I2C_MagicPanel_Command::Value)Wire.read();

	switch (command)
	{
		case I2C_MagicPanel_Command::On:
			modeActive = false;
			On();
			break;

		case I2C_MagicPanel_Command::Off:
			modeActive = false;
			Off();
			break;

		case I2C_MagicPanel_Command::Brightness:
			SetBrightness(Wire.read());
			break;

		case I2C_MagicPanel_Command::Mode:
			SetMode((I2C_MagicPanel_Mode::Value)Wire.read());
			break;

		case I2C_MagicPanel_Command::Random:
			Random();
			break;
	};
}

// ----------------------------------------------------------------------------
// COMMANDS
// ----------------------------------------------------------------------------
void MagicPanel::SetBrightness(int brightness)
{
	ledControl->setIntensity(0, brightness);
	ledControl->setIntensity(1, brightness);
}

void MagicPanel::Off()
{
	ClearButDoNotShow(false);
	ResetModeVariables();
	MapAndPrint();
}

void MagicPanel::On()
{
	ClearButDoNotShow(true);
	ResetModeVariables();
	MapAndPrint();
}

void MagicPanel::SetMode(I2C_MagicPanel_Mode::Value mode)
{
	currentmode = mode;
	ResetModeVariables();
	Off();
	modeActive = true;
}

void MagicPanel::Random()
{
	ResetModeVariables();
	randomActive = true;
	lastRandomTimeCheck = millis() + RANDOMSLEEP;
	modeActive = true;
}

void MagicPanel::ResetModeVariables()
{
	state = 0;
	direction = 1;
	state2 = 0;
}

// ----------------------------------------------------------------------------
// MODES
// ----------------------------------------------------------------------------
void MagicPanel::AnimateAlert()
{
	if (!IsTimeForStateChange(250))
		return;

	int row;

	if (state == 0) 
		for (row = 0; row<8; row++)
			SetRow(row, B11111111);
	else
		for (row = 0; row<8; row++)
			SetRow(row, B00000000);

	MapAndPrint();

	state++;
	if (state == 2)
		state = 0;
}

void MagicPanel::AnimateToggleTopBottom()
{
	if (!IsTimeForStateChange(250))
		return;

	if (state == 0)
	{
		SetRow(0, B11111111);
		SetRow(1, B11111111);
		SetRow(2, B11111111);
		SetRow(3, B11111111);
		SetRow(4, B00000000);
		SetRow(5, B00000000);
		SetRow(6, B00000000);
		SetRow(7, B00000000);
	}
	else
	{
		SetRow(0, B00000000);
		SetRow(1, B00000000);
		SetRow(2, B00000000);
		SetRow(3, B00000000);
		SetRow(4, B11111111);
		SetRow(5, B11111111);
		SetRow(6, B11111111);
		SetRow(7, B11111111);
	}

	MapAndPrint();

	state++;
	if (state == 2)
		state = 0;
}

void MagicPanel::AnimateToggleLeftRight()
{
	if (!IsTimeForStateChange(250))
		return;

	if (state == 0)
	{
		SetRow(0, B11110000);
		SetRow(1, B11110000);
		SetRow(2, B11110000);
		SetRow(3, B11110000);
		SetRow(4, B11110000);
		SetRow(5, B11110000);
		SetRow(6, B11110000);
		SetRow(7, B11110000);
	}
	else
	{
		SetRow(0, B00001111);
		SetRow(1, B00001111);
		SetRow(2, B00001111);
		SetRow(3, B00001111);
		SetRow(4, B00001111);
		SetRow(5, B00001111);
		SetRow(6, B00001111);
		SetRow(7, B00001111);
	}

	MapAndPrint();

	state++;
	if (state == 2)
		state = 0;
}

void MagicPanel::AnimateToggleQuad()
{
	if (!IsTimeForStateChange(250))
		return;

	if (state == 0)
	{
		SetRow(0, B11110000);
		SetRow(1, B11110000);
		SetRow(2, B11110000);
		SetRow(3, B11110000);
		SetRow(4, B00001111);
		SetRow(5, B00001111);
		SetRow(6, B00001111);
		SetRow(7, B00001111);
	}
	else
	{
		SetRow(0, B00001111);
		SetRow(1, B00001111);
		SetRow(2, B00001111);
		SetRow(3, B00001111);
		SetRow(4, B11110000);
		SetRow(5, B11110000);
		SetRow(6, B11110000);
		SetRow(7, B11110000);
	}

	MapAndPrint();

	state++;
	if (state == 2)
		state = 0;
}

void MagicPanel::AnimateQuadCycleCounterClockwise()
{
	if (!IsTimeForStateChange(250, true))
		return;

	switch (state)
	{
		case 0:
			SetRow(0, B00001111);
			SetRow(1, B00001111);
			SetRow(2, B00001111);
			SetRow(3, B00001111);
			break;

		case 1:
			SetRow(0, B11110000);
			SetRow(1, B11110000);
			SetRow(2, B11110000);
			SetRow(3, B11110000);
			break;

		case 2:
			SetRow(4, B11110000);
			SetRow(5, B11110000);
			SetRow(6, B11110000);
			SetRow(7, B11110000);
			break;

		case 3:
			SetRow(4, B00001111);
			SetRow(5, B00001111);
			SetRow(6, B00001111);
			SetRow(7, B00001111);
			break;
	};

	MapAndPrint();

	state++;
	if (state == 4)
		state = 0;
}

void MagicPanel::AnimateQuadCycleClockwise()
{
	if (!IsTimeForStateChange(250, true))
		return;

	switch (state)
	{
		case 0:
			SetRow(0, B00001111);
			SetRow(1, B00001111);
			SetRow(2, B00001111);
			SetRow(3, B00001111);
			break;

		case 1:
			SetRow(0, B11110000);
			SetRow(1, B11110000);
			SetRow(2, B11110000);
			SetRow(3, B11110000);
			break;

		case 2:
			SetRow(4, B11110000);
			SetRow(5, B11110000);
			SetRow(6, B11110000);
			SetRow(7, B11110000);
			break;

		case 3:
			SetRow(4, B00001111);
			SetRow(5, B00001111);
			SetRow(6, B00001111);
			SetRow(7, B00001111);
			break;
	};

	MapAndPrint();


	state--;
	if (state < 0)
		state = 3;
}

void MagicPanel::AnimateTraceUp()
{
	if (!IsTimeForStateChange(100, true))
		return;

	SetRow(state, B11111111);
	MapAndPrint();

	state--;
	if (state < 0)
		state = 7;
}

void MagicPanel::AnimateTraceDown()
{
	if (!IsTimeForStateChange(100, true))
		return;

	SetRow(state, B11111111);
	MapAndPrint();

	state++;
	if (state > 7)
		state = 0;
}

void MagicPanel::AnimateTraceUpDown()
{
	if (!IsTimeForStateChange(100, true))
		return;

	SetRow(state, B11111111);
	MapAndPrint();

	state += direction;
	if (state <0)
	{
		state = 1;
		direction = 1;
	}
	else if (state > 7)
	{
		state = 6;
		direction = -1;
	}
}

void MagicPanel::AnimateTraceLeft()
{
	if (!IsTimeForStateChange(100, true))
		return;

	SetCol(state, B11111111);
	MapAndPrint();

	state++;
	if (state > 7)
		state = 0;
}

void MagicPanel::AnimateTraceRight()
{
	if (!IsTimeForStateChange(100, true))
		return;

	SetCol(state, B11111111);
	MapAndPrint();

	state--;
	if (state < 0)
		state = 7;
}

void MagicPanel::AnimateTraceLeftRight()
{
	if (!IsTimeForStateChange(100, true))
		return;

	SetCol(state, B11111111);
	MapAndPrint();

	state += direction;
	if (state <0)
	{
		state = 1;
		direction = 1;
	}
	else if (state > 7)
	{
		state = 6;
		direction = -1;
	}
}

void MagicPanel::AnimateSingleLEDTest()
{
	if (!IsTimeForStateChange(50, true))
		return;

	vMagicPanel[state][state2] = true;
	MapAndPrint();

	state2++;

	if (state2 > 7)
	{
		state2 = 0;
		state++;
		if (state > 7)
			state = 0;
	}
}

void MagicPanel::AnimateDoubleLEDTest()
{
	if (!IsTimeForStateChange(50, false))
		return;

	vMagicPanel[state][state2] = !vMagicPanel[state][state2];
	MapAndPrint();

	state2++;

	if (state2 > 7)
	{
		state2 = 0;
		state++;
		if (state > 7)
			state = 0;
	}
}

void MagicPanel::AnimateRandomPixel()
{
	if (!IsTimeForStateChange(50, true))
		return;

	int row = random(8);
	int col = random(8);

	vMagicPanel[row][col] = true;

	MapAndPrint();
}

void MagicPanel::AnimateString()
{
	if (!feedGridLeftComplete)
		return;

	feedGridLeftComplete = false;

	if (letterIndex > englishString.length())
		letterIndex = 0;

	DrawLetter(englishString[letterIndex]);

	letterIndex++;
}

// ----------------------------------------------------------------------------
// COMMON METHODS
// ----------------------------------------------------------------------------
void MagicPanel::MapAndPrint() 
{
	MapBoolGrid();
	PrintGrid();
}

void MagicPanel::MapBoolGrid()
{
	for(int Row=0; Row<8; Row++)
	{
		panel[2 * Row]     = 128 * vMagicPanel[Row][7] + 
							  64 * vMagicPanel[Row][6] + 
							  32 * vMagicPanel[Row][5] + 
							  16 * vMagicPanel[Row][4];       // 0, 2, 4, 6, 8, 10, 12, 14

		panel[2 * Row + 1] =   8 * vMagicPanel[Row][3] + 
							   4 * vMagicPanel[Row][2] +
							   2 * vMagicPanel[Row][1] + 
								   vMagicPanel[Row][0];            // 1, 3, 5, 7, 9, 11, 13, 15
	}
}

void MagicPanel::PrintGrid(){
	for(int i=0; i<16; i++)
	{
		if(i<8)
		{
			ledControl->setRow(0, i, panel[i]);
		}
		else
		{
			ledControl->setRow(1, i-8, panel[i]);
		}
	}
}

bool MagicPanel::IsTimeForStateChange(int delay, bool clear)
{
	unsigned long timeNow = millis();
  
	// early exit if we don't need to do anything
	if (timeNow - lastTimeCheck < delay)
		return false;

	// set the time  
	lastTimeCheck = timeNow;

	// clear the device
	if (clear)
		ClearButDoNotShow(false);

	return true;
}

void MagicPanel::SetRow(int ledRow, unsigned char rowState)
{
	for (int col = 0; col < 8; col++)
		vMagicPanel[ledRow][col] = ((rowState >> col) & 1);
}

void MagicPanel::SetCol(int ledCol, unsigned char colState)
{
	for (int row = 0; row < 8; row++)
		vMagicPanel[row][ledCol] = ((colState >> row) & 1);
}

unsigned char MagicPanel::GetCol(int ledCol)
{
	unsigned char result = 0;

	for (int row = 7; row >= 0; row--)
	{
		result = result << 1;
		result += vMagicPanel[row][ledCol] == true ? 1 : 0;
	}

	return result;
}

void MagicPanel::ClearButDoNotShow(bool isOn)
{
	for(int row=0;row<8;row++) 
		for(int col=0;col<8;col++) 
			vMagicPanel[row][col]=isOn;
}

void MagicPanel::DrawLetter(char letter)
{
	switch (letter)
	{
		case ' ' : FeedGridLeft(B00000000); FeedGridLeft(B00000000); FeedGridLeft(B00000000); FeedGridLeft(B00000000); FeedGridLeft(B00000000); FeedGridLeft(B00000000); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case '!' : FeedGridLeft(B00000000); FeedGridLeft(B00000000); FeedGridLeft(B00000000); FeedGridLeft(B00000000); FeedGridLeft(B01011111); FeedGridLeft(B00000000); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case '"' : FeedGridLeft(B00000000); FeedGridLeft(B00000000); FeedGridLeft(B00000000); FeedGridLeft(B00000011); FeedGridLeft(B00000000); FeedGridLeft(B00000011); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case '#' : FeedGridLeft(B00000000); FeedGridLeft(B00100100); FeedGridLeft(B01111110); FeedGridLeft(B00100100); FeedGridLeft(B00100100); FeedGridLeft(B01111110); FeedGridLeft(B00100100); FeedGridLeft(B00000000); break;
		case '$' : FeedGridLeft(B00000000); FeedGridLeft(B00101110); FeedGridLeft(B00101010); FeedGridLeft(B01111111); FeedGridLeft(B00101010); FeedGridLeft(B00111010); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case '%' : FeedGridLeft(B00000000); FeedGridLeft(B01000110); FeedGridLeft(B00100110); FeedGridLeft(B00010000); FeedGridLeft(B00001000); FeedGridLeft(B01100100); FeedGridLeft(B01100010); FeedGridLeft(B00000000); break;
		case '&' : FeedGridLeft(B00000000); FeedGridLeft(B00100000); FeedGridLeft(B01010100); FeedGridLeft(B01001010); FeedGridLeft(B01010100); FeedGridLeft(B00100000); FeedGridLeft(B01010000); FeedGridLeft(B00000000); break;
		case '\'' : FeedGridLeft(B00000000); FeedGridLeft(B00000000); FeedGridLeft(B00000000); FeedGridLeft(B00000100); FeedGridLeft(B00000010); FeedGridLeft(B00000000); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case '(' : FeedGridLeft(B00000000); FeedGridLeft(B00000000); FeedGridLeft(B00000000); FeedGridLeft(B00111100); FeedGridLeft(B01000010); FeedGridLeft(B00000000); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case ')' : FeedGridLeft(B00000000); FeedGridLeft(B00000000); FeedGridLeft(B00000000); FeedGridLeft(B01000010); FeedGridLeft(B00111100); FeedGridLeft(B00000000); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case '*' : FeedGridLeft(B00000000); FeedGridLeft(B00010000); FeedGridLeft(B01010100); FeedGridLeft(B00111000); FeedGridLeft(B01010100); FeedGridLeft(B00010000); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case '+' : FeedGridLeft(B00000000); FeedGridLeft(B00010000); FeedGridLeft(B00010000); FeedGridLeft(B01111100); FeedGridLeft(B00010000); FeedGridLeft(B00010000); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case '-' : FeedGridLeft(B00000000); FeedGridLeft(B00010000); FeedGridLeft(B00010000); FeedGridLeft(B00010000); FeedGridLeft(B00010000); FeedGridLeft(B00010000); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case '.' : FeedGridLeft(B00000000); FeedGridLeft(B00000000); FeedGridLeft(B00000000); FeedGridLeft(B01100000); FeedGridLeft(B01100000); FeedGridLeft(B00000000); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case '/' : FeedGridLeft(B00000000); FeedGridLeft(B01000000); FeedGridLeft(B00100000); FeedGridLeft(B00010000); FeedGridLeft(B00001000); FeedGridLeft(B00000100); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case '0' : FeedGridLeft(B00111100); FeedGridLeft(B01100010); FeedGridLeft(B01010010); FeedGridLeft(B01001010); FeedGridLeft(B01000110); FeedGridLeft(B00111100); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case '1' : FeedGridLeft(B01000100); FeedGridLeft(B01000010); FeedGridLeft(B01111110); FeedGridLeft(B01000000); FeedGridLeft(B01000000); FeedGridLeft(B00000000); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case '2' : FeedGridLeft(B01100100); FeedGridLeft(B01010010); FeedGridLeft(B01010010); FeedGridLeft(B01010010); FeedGridLeft(B01010010); FeedGridLeft(B01001100); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case '3' : FeedGridLeft(B00100100); FeedGridLeft(B01000010); FeedGridLeft(B01000010); FeedGridLeft(B01001010); FeedGridLeft(B01001010); FeedGridLeft(B00110100); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case '4' : FeedGridLeft(B00110000); FeedGridLeft(B00101000); FeedGridLeft(B00100100); FeedGridLeft(B01111110); FeedGridLeft(B00100000); FeedGridLeft(B00100000); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case '5' : FeedGridLeft(B00101110); FeedGridLeft(B01001010); FeedGridLeft(B01001010); FeedGridLeft(B01001010); FeedGridLeft(B01001010); FeedGridLeft(B00110010); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case '6' : FeedGridLeft(B00111100); FeedGridLeft(B01001010); FeedGridLeft(B01001010); FeedGridLeft(B01001010); FeedGridLeft(B01001010); FeedGridLeft(B00110000); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case '7' : FeedGridLeft(B00000010); FeedGridLeft(B00000010); FeedGridLeft(B01100010); FeedGridLeft(B00010010); FeedGridLeft(B00001010); FeedGridLeft(B00000110); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case '8' : FeedGridLeft(B00110100); FeedGridLeft(B01001010); FeedGridLeft(B01001010); FeedGridLeft(B01001010); FeedGridLeft(B01001010); FeedGridLeft(B00110100); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case '9' : FeedGridLeft(B00001100); FeedGridLeft(B01010010); FeedGridLeft(B01010010); FeedGridLeft(B01010010); FeedGridLeft(B01010010); FeedGridLeft(B00111100); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case ':' : FeedGridLeft(B00000000); FeedGridLeft(B00000000); FeedGridLeft(B00000000); FeedGridLeft(B01001000); FeedGridLeft(B00000000); FeedGridLeft(B00000000); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case ';' : FeedGridLeft(B00000000); FeedGridLeft(B00000000); FeedGridLeft(B10000000); FeedGridLeft(B01100100); FeedGridLeft(B00000000); FeedGridLeft(B00000000); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case '<' : FeedGridLeft(B00000000); FeedGridLeft(B00000000); FeedGridLeft(B00010000); FeedGridLeft(B00101000); FeedGridLeft(B01000100); FeedGridLeft(B00000000); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case '=' : FeedGridLeft(B00000000); FeedGridLeft(B00101000); FeedGridLeft(B00101000); FeedGridLeft(B00101000); FeedGridLeft(B00101000); FeedGridLeft(B00101000); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case '>' : FeedGridLeft(B00000000); FeedGridLeft(B00000000); FeedGridLeft(B01000100); FeedGridLeft(B00101000); FeedGridLeft(B00010000); FeedGridLeft(B00000000); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case '?' : FeedGridLeft(B00000000); FeedGridLeft(B00000100); FeedGridLeft(B00000010); FeedGridLeft(B00000010); FeedGridLeft(B01010010); FeedGridLeft(B00001010); FeedGridLeft(B00000100); FeedGridLeft(B00000000); break;
		case '@' : FeedGridLeft(B00000000); FeedGridLeft(B00111100); FeedGridLeft(B01000010); FeedGridLeft(B01011010); FeedGridLeft(B01010110); FeedGridLeft(B01011010); FeedGridLeft(B00011100); FeedGridLeft(B00000000); break;
		case 'A' : FeedGridLeft(B01111100); FeedGridLeft(B00010010); FeedGridLeft(B00010010); FeedGridLeft(B00010010); FeedGridLeft(B00010010); FeedGridLeft(B01111100); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case 'B' : FeedGridLeft(B01111110); FeedGridLeft(B01001010); FeedGridLeft(B01001010); FeedGridLeft(B01001010); FeedGridLeft(B01001010); FeedGridLeft(B00110100); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case 'C' : FeedGridLeft(B00111100); FeedGridLeft(B01000010); FeedGridLeft(B01000010); FeedGridLeft(B01000010); FeedGridLeft(B01000010); FeedGridLeft(B00100100); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case 'D' : FeedGridLeft(B01111110); FeedGridLeft(B01000010); FeedGridLeft(B01000010); FeedGridLeft(B01000010); FeedGridLeft(B00100100); FeedGridLeft(B00011000); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case 'E' : FeedGridLeft(B01111110); FeedGridLeft(B01001010); FeedGridLeft(B01001010); FeedGridLeft(B01001010); FeedGridLeft(B01001010); FeedGridLeft(B01000010); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case 'F' : FeedGridLeft(B01111110); FeedGridLeft(B00001010); FeedGridLeft(B00001010); FeedGridLeft(B00001010); FeedGridLeft(B00001010); FeedGridLeft(B00000010); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case 'G' : FeedGridLeft(B00111100); FeedGridLeft(B01000010); FeedGridLeft(B01000010); FeedGridLeft(B01010010); FeedGridLeft(B01010010); FeedGridLeft(B00110100); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case 'H' : FeedGridLeft(B01111110); FeedGridLeft(B00001000); FeedGridLeft(B00001000); FeedGridLeft(B00001000); FeedGridLeft(B00001000); FeedGridLeft(B01111110); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case 'I' : FeedGridLeft(B00000000); FeedGridLeft(B01000010); FeedGridLeft(B01000010); FeedGridLeft(B01111110); FeedGridLeft(B01000010); FeedGridLeft(B01000010); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case 'J' : FeedGridLeft(B00110000); FeedGridLeft(B01000000); FeedGridLeft(B01000000); FeedGridLeft(B01000000); FeedGridLeft(B01000000); FeedGridLeft(B00111110); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case 'K' : FeedGridLeft(B01111110); FeedGridLeft(B00001000); FeedGridLeft(B00001000); FeedGridLeft(B00010100); FeedGridLeft(B00100010); FeedGridLeft(B01000000); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case 'L' : FeedGridLeft(B01111110); FeedGridLeft(B01000000); FeedGridLeft(B01000000); FeedGridLeft(B01000000); FeedGridLeft(B01000000); FeedGridLeft(B01000000); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case 'M' : FeedGridLeft(B01111110); FeedGridLeft(B00000100); FeedGridLeft(B00001000); FeedGridLeft(B00001000); FeedGridLeft(B00000100); FeedGridLeft(B01111110); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case 'N' : FeedGridLeft(B01111110); FeedGridLeft(B00000100); FeedGridLeft(B00001000); FeedGridLeft(B00010000); FeedGridLeft(B00100000); FeedGridLeft(B01111110); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case 'O' : FeedGridLeft(B00111100); FeedGridLeft(B01000010); FeedGridLeft(B01000010); FeedGridLeft(B01000010); FeedGridLeft(B01000010); FeedGridLeft(B00111100); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case 'P' : FeedGridLeft(B01111110); FeedGridLeft(B00010010); FeedGridLeft(B00010010); FeedGridLeft(B00010010); FeedGridLeft(B00010010); FeedGridLeft(B00001100); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case 'Q' : FeedGridLeft(B00111100); FeedGridLeft(B01000010); FeedGridLeft(B01010010); FeedGridLeft(B01100010); FeedGridLeft(B01000010); FeedGridLeft(B00111100); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case 'R' : FeedGridLeft(B01111110); FeedGridLeft(B00010010); FeedGridLeft(B00010010); FeedGridLeft(B00010010); FeedGridLeft(B00110010); FeedGridLeft(B01001100); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case 'S' : FeedGridLeft(B00100100); FeedGridLeft(B01001010); FeedGridLeft(B01001010); FeedGridLeft(B01001010); FeedGridLeft(B01001010); FeedGridLeft(B00110000); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case 'T' : FeedGridLeft(B00000010); FeedGridLeft(B00000010); FeedGridLeft(B00000010); FeedGridLeft(B01111110); FeedGridLeft(B00000010); FeedGridLeft(B00000010); FeedGridLeft(B00000010); FeedGridLeft(B00000000); break;
		case 'U' : FeedGridLeft(B00111110); FeedGridLeft(B01000000); FeedGridLeft(B01000000); FeedGridLeft(B01000000); FeedGridLeft(B01000000); FeedGridLeft(B00111110); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case 'V' : FeedGridLeft(B00011110); FeedGridLeft(B00100000); FeedGridLeft(B01000000); FeedGridLeft(B01000000); FeedGridLeft(B00100000); FeedGridLeft(B00011110); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case 'W' : FeedGridLeft(B00111110); FeedGridLeft(B01000000); FeedGridLeft(B00100000); FeedGridLeft(B00100000); FeedGridLeft(B01000000); FeedGridLeft(B00111110); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case 'X' : FeedGridLeft(B01000010); FeedGridLeft(B00100100); FeedGridLeft(B00011000); FeedGridLeft(B00011000); FeedGridLeft(B00100100); FeedGridLeft(B01000010); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case 'Y' : FeedGridLeft(B00000010); FeedGridLeft(B00000100); FeedGridLeft(B00001000); FeedGridLeft(B01110000); FeedGridLeft(B00001000); FeedGridLeft(B00000100); FeedGridLeft(B00000010); FeedGridLeft(B00000000); break;
		case 'Z' : FeedGridLeft(B01000010); FeedGridLeft(B01100010); FeedGridLeft(B01010010); FeedGridLeft(B01001010); FeedGridLeft(B01000110); FeedGridLeft(B01000010); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case '[' : FeedGridLeft(B00000000); FeedGridLeft(B00000000); FeedGridLeft(B01111110); FeedGridLeft(B01000010); FeedGridLeft(B01000010); FeedGridLeft(B00000000); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case '\\' : FeedGridLeft(B00000000); FeedGridLeft(B00000100); FeedGridLeft(B00001000); FeedGridLeft(B00010000); FeedGridLeft(B00100000); FeedGridLeft(B01000000); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case ']' : FeedGridLeft(B00000000); FeedGridLeft(B00000000); FeedGridLeft(B01000010); FeedGridLeft(B01000010); FeedGridLeft(B01111110); FeedGridLeft(B00000000); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case '^' : FeedGridLeft(B00000000); FeedGridLeft(B00001000); FeedGridLeft(B00000100); FeedGridLeft(B01111110); FeedGridLeft(B00000100); FeedGridLeft(B00001000); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case '_' : FeedGridLeft(B10000000); FeedGridLeft(B10000000); FeedGridLeft(B10000000); FeedGridLeft(B10000000); FeedGridLeft(B10000000); FeedGridLeft(B10000000); FeedGridLeft(B10000000); FeedGridLeft(B00000000); break;
		case '`' : FeedGridLeft(B00111100); FeedGridLeft(B01000010); FeedGridLeft(B10011001); FeedGridLeft(B10100101); FeedGridLeft(B10100101); FeedGridLeft(B10000001); FeedGridLeft(B01000010); FeedGridLeft(B00111100); break;
		case 'a' : FeedGridLeft(B00000000); FeedGridLeft(B00100000); FeedGridLeft(B01010100); FeedGridLeft(B01010100); FeedGridLeft(B01010100); FeedGridLeft(B01111000); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case 'b' : FeedGridLeft(B00000000); FeedGridLeft(B01111110); FeedGridLeft(B01001000); FeedGridLeft(B01001000); FeedGridLeft(B01001000); FeedGridLeft(B00110000); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case 'c' : FeedGridLeft(B00000000); FeedGridLeft(B00000000); FeedGridLeft(B00111000); FeedGridLeft(B01000100); FeedGridLeft(B01000100); FeedGridLeft(B01000100); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case 'd' : FeedGridLeft(B00000000); FeedGridLeft(B00110000); FeedGridLeft(B01001000); FeedGridLeft(B01001000); FeedGridLeft(B01001000); FeedGridLeft(B01111110); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case 'e' : FeedGridLeft(B00000000); FeedGridLeft(B00111000); FeedGridLeft(B01010100); FeedGridLeft(B01010100); FeedGridLeft(B01010100); FeedGridLeft(B01001000); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case 'f' : FeedGridLeft(B00000000); FeedGridLeft(B00000000); FeedGridLeft(B00000000); FeedGridLeft(B01111100); FeedGridLeft(B00001010); FeedGridLeft(B00000010); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case 'g' : FeedGridLeft(B00000000); FeedGridLeft(B00011000); FeedGridLeft(B10100100); FeedGridLeft(B10100100); FeedGridLeft(B10100100); FeedGridLeft(B10100100); FeedGridLeft(B01111100); FeedGridLeft(B00000000); break;
		case 'h' : FeedGridLeft(B00000000); FeedGridLeft(B01111110); FeedGridLeft(B00001000); FeedGridLeft(B00001000); FeedGridLeft(B00001000); FeedGridLeft(B01110000); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case 'i' : FeedGridLeft(B00000000); FeedGridLeft(B00000000); FeedGridLeft(B00000000); FeedGridLeft(B01001000); FeedGridLeft(B01111010); FeedGridLeft(B01000000); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case 'j' : FeedGridLeft(B00000000); FeedGridLeft(B00000000); FeedGridLeft(B01000000); FeedGridLeft(B10000000); FeedGridLeft(B10000000); FeedGridLeft(B01111010); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case 'k' : FeedGridLeft(B00000000); FeedGridLeft(B01111110); FeedGridLeft(B00011000); FeedGridLeft(B00100100); FeedGridLeft(B01000000); FeedGridLeft(B00000000); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case 'l' : FeedGridLeft(B00000000); FeedGridLeft(B00000000); FeedGridLeft(B00000000); FeedGridLeft(B00111110); FeedGridLeft(B01000000); FeedGridLeft(B01000000); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case 'm' : FeedGridLeft(B00000000); FeedGridLeft(B01111100); FeedGridLeft(B00000100); FeedGridLeft(B01111000); FeedGridLeft(B00000100); FeedGridLeft(B01111000); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case 'n' : FeedGridLeft(B00000000); FeedGridLeft(B01111100); FeedGridLeft(B00000100); FeedGridLeft(B00000100); FeedGridLeft(B00000100); FeedGridLeft(B01111000); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case 'o' : FeedGridLeft(B00000000); FeedGridLeft(B00111000); FeedGridLeft(B01000100); FeedGridLeft(B01000100); FeedGridLeft(B01000100); FeedGridLeft(B00111000); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case 'p' : FeedGridLeft(B00000000); FeedGridLeft(B11111100); FeedGridLeft(B00100100); FeedGridLeft(B00100100); FeedGridLeft(B00100100); FeedGridLeft(B00011000); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case 'q' : FeedGridLeft(B00000000); FeedGridLeft(B00011000); FeedGridLeft(B00100100); FeedGridLeft(B00100100); FeedGridLeft(B00100100); FeedGridLeft(B11111100); FeedGridLeft(B10000000); FeedGridLeft(B00000000); break;
		case 'r' : FeedGridLeft(B00000000); FeedGridLeft(B00000000); FeedGridLeft(B01111000); FeedGridLeft(B00000100); FeedGridLeft(B00000100); FeedGridLeft(B00000100); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case 's' : FeedGridLeft(B00000000); FeedGridLeft(B01001000); FeedGridLeft(B01010100); FeedGridLeft(B01010100); FeedGridLeft(B01010100); FeedGridLeft(B00100000); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case 't' : FeedGridLeft(B00000000); FeedGridLeft(B00000000); FeedGridLeft(B00000100); FeedGridLeft(B00111110); FeedGridLeft(B01000100); FeedGridLeft(B01000000); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case 'u' : FeedGridLeft(B00000000); FeedGridLeft(B00111100); FeedGridLeft(B01000000); FeedGridLeft(B01000000); FeedGridLeft(B01000000); FeedGridLeft(B00111100); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case 'v' : FeedGridLeft(B00000000); FeedGridLeft(B00001100); FeedGridLeft(B00110000); FeedGridLeft(B01000000); FeedGridLeft(B00110000); FeedGridLeft(B00001100); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case 'w' : FeedGridLeft(B00000000); FeedGridLeft(B00111100); FeedGridLeft(B01000000); FeedGridLeft(B00111000); FeedGridLeft(B01000000); FeedGridLeft(B00111100); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case 'x' : FeedGridLeft(B00000000); FeedGridLeft(B01000100); FeedGridLeft(B00101000); FeedGridLeft(B00010000); FeedGridLeft(B00101000); FeedGridLeft(B01000100); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case 'y' : FeedGridLeft(B00000000); FeedGridLeft(B00011100); FeedGridLeft(B10100000); FeedGridLeft(B10100000); FeedGridLeft(B10100000); FeedGridLeft(B01111100); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case 'z' : FeedGridLeft(B00000000); FeedGridLeft(B01000100); FeedGridLeft(B01100100); FeedGridLeft(B01010100); FeedGridLeft(B01001100); FeedGridLeft(B01000100); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case '{' : FeedGridLeft(B00000000); FeedGridLeft(B00001000); FeedGridLeft(B00001000); FeedGridLeft(B01110110); FeedGridLeft(B01000010); FeedGridLeft(B01000010); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case '|' : FeedGridLeft(B00000000); FeedGridLeft(B00000000); FeedGridLeft(B00000000); FeedGridLeft(B01111110); FeedGridLeft(B00000000); FeedGridLeft(B00000000); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case '}' : FeedGridLeft(B00000000); FeedGridLeft(B01000010); FeedGridLeft(B01000010); FeedGridLeft(B01110110); FeedGridLeft(B00001000); FeedGridLeft(B00001000); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
		case '~' : FeedGridLeft(B00000000); FeedGridLeft(B00000000); FeedGridLeft(B00000100); FeedGridLeft(B00000010); FeedGridLeft(B00000100); FeedGridLeft(B00000010); FeedGridLeft(B00000000); FeedGridLeft(B00000000); break;
	}

	feedGridLeftComplete = true;
}

void MagicPanel::FeedGridLeft(unsigned char newColumn)
{
	for (int col = 7; col > 0; col--)
		SetCol(col, GetCol(col - 1));

	SetCol(0, newColumn);
	MapAndPrint();

	delay(60);
}
