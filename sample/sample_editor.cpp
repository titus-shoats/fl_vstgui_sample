#include "lib/platform/platform_win32.h"
#include "sample_editor.h"
#include "fp_plugclass.h"
#include "lib/controls/ctextlabel.h"

//----------------
// constructor
//----------------
sample_editor::sample_editor(TFruityPlug* effect, void* ptr)
	:plugin(effect), _controls{ nullptr }
{
	// Create a larger frame for 5 knobs: Gain, Attack, Decay, Sustain, Release
	// Each knob: 40x40, spacing: 10px, left/right margin: 20px
	// Total width: 20 + (40*5) + (10*4) + 20 = 280px
	// Height: 100px (20 top margin + 40 knob + 20 label + 20 bottom)
	VSTGUI::CRect frameSize(0, 0, 280, 100);
	this->frame = new VSTGUI::CFrame(frameSize, this);
	this->frame->open(ptr, VSTGUI::PlatformType::kHWND);

	// Set background color (dark gray for trap/modern look)
	this->frame->setBackgroundColor(VSTGUI::CColor(30, 30, 35, 255));

	// Create 5 knobs with labels
	const char* labels[] = { "Gain", "Attack", "Decay", "Sustain", "Release" };
	int xPos = 20; // Starting X position
	
	for (int i = 0; i < NumControls; i++)
	{
		// Create knob
		VSTGUI::CRect r(0, 0, 40, 40);
		r.offset(xPos, 20);
		VSTGUI::CKnob* knob = new VSTGUI::CKnob(r, this, i, nullptr, nullptr);
		knob->setColorHandle(VSTGUI::CColor(255, 100, 50, 255));  // Orange/red color for trap aesthetic
		knob->setColorShadowHandle(VSTGUI::kTransparentCColor);
		knob->setRangeAngle(static_cast<float>((M_PI*2) * 5 / 6));
		knob->setStartAngle(static_cast<float>((M_PI*2) / 3));
		knob->setWheelInc(0.01f);
		knob->setHandleLineWidth(3.0f);
		knob->setInsetValue(7.0f);
		knob->setMax((1 << 16));
		
		// Set default values
		if (i == 0 || i == 3) // Gain and Sustain default to full
			knob->setDefaultValue((1 << 16));
		else if (i == 1) // Attack - 10ms
			knob->setDefaultValue((int)(0.005f * (1 << 16)));
		else if (i == 2) // Decay - 100ms
			knob->setDefaultValue((int)(0.05f * (1 << 16)));
		else if (i == 4) // Release - 200ms
			knob->setDefaultValue((int)(0.05f * (1 << 16)));
		
		this->_controls[i] = knob;
		this->frame->addView(knob);
		
		// Create text label below knob
		VSTGUI::CRect labelRect(xPos - 10, 65, xPos + 50, 85);
		VSTGUI::CTextLabel* label = new VSTGUI::CTextLabel(labelRect, labels[i]);
		label->setFontColor(VSTGUI::CColor(200, 200, 200, 255));
		label->setBackColor(VSTGUI::kTransparentCColor);
		label->setFrameColor(VSTGUI::kTransparentCColor);
		label->setFont(VSTGUI::kNormalFontSmall);
		label->setHoriAlign(VSTGUI::CHoriTxtAlign::kCenterText);
		this->frame->addView(label);
		
		xPos += 50; // Move to next knob position (40px knob + 10px spacing)
	}

	// synchronize host parameters
	for (int i = 0; i < NumControls; i++)
	{
		setParameter(i, static_cast<float>(this->plugin->ProcessParam(i, 0, REC_GetValue)));
	}
}

//----------------
// destructor
//----------------
sample_editor::~sample_editor()
{
	if (frame != nullptr)
	{
		frame->forget();
	}
}

//-----------------------
// send mesasge to host
//-----------------------
void sample_editor::valueChanged(VSTGUI::CControl* pControl)
{
	int index = pControl->getTag();
	if(index < NumControls)
	{
		plugin->ProcessParam(index, static_cast<int>(pControl->getValue() + 0.5), REC_UpdateValue);
	}
}

void sample_editor::doIdleStuff()
{
	if (frame != nullptr)
	{
		frame->idle();
	}
}

//---------------------------
// recieve message from host
//---------------------------
void sample_editor::setParameter(int32_t index, float value)
{
	if (frame != nullptr && index < NumControls && _controls[index] != nullptr)
	{
		_controls[index]->setValue(value);
	}
}

void* sample_editor::getHWND()
{
	return frame->getPlatformFrame()->getPlatformRepresentation();
}
