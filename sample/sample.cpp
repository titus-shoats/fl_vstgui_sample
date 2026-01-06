#include "sample_editor.h"
#include "sample.h"
#include <cmath>

//---------------------
// Constants
//---------------------
namespace {
	constexpr float PITCH_SCALE = 100.0f;     // FL Studio pitch scaling factor
	constexpr float MIDDLE_C = 60.0f;         // MIDI note for middle C
	constexpr float DEFAULT_SAMPLE_RATE = 44100.0f; // TODO: Query from host
}

//---------------------
// Plug-in information
//---------------------
TFruityPlugInfo PlugInfo =
{
	CurrentSDKVersion,
	"Trap RnB Rompler",
	"TrapRompler",
	FPF_Type_HybridGen,
	NumParams // the amount of parameters
};

//----------------
// DLL entry
//----------------
extern "C" BOOL WINAPI DllMain(HINSTANCE hInst, DWORD dwReason, LPVOID lpvReserved)
{
	VSTGUI::initPlatform(hInst);
	return TRUE;
}

//----------------
// 
//----------------
extern "C" TFruityPlug* _stdcall CreatePlugInstance(TFruityPlugHost *Host, int Tag)
{
	return new sample(Tag, Host);
};

//----------------
// constructor
//----------------
sample::sample(int Tag, TFruityPlugHost *Host)
{
	Info = &PlugInfo;
	HostTag = Tag;
	EditorHandle = 0;
	_host = Host;
	_editor = nullptr;

	// parameter initialize
	_gain = 1.0f;
	_attack = 0.01f;   // 10ms default attack
	_decay = 0.1f;     // 100ms default decay
	_sustain = 0.7f;   // 70% sustain level
	_release = 0.2f;   // 200ms default release
	
	// Initialize parameter values (FL Studio uses 16-bit fixed point)
	_params[0] = (1 << 16);      // Gain (100%)
	_params[1] = (int)(0.01f * (1 << 16));  // Attack
	_params[2] = (int)(0.1f * (1 << 16));   // Decay
	_params[3] = (int)(0.7f * (1 << 16));   // Sustain
	_params[4] = (int)(0.2f * (1 << 16));   // Release
	
	// Initialize sample data with a simple waveform
	initializeSampleData();
}

//----------------
// destructor
//----------------
sample::~sample()
{
	// Clean up all active voices
	for (auto voice : _voices)
	{
		delete voice;
	}
	_voices.clear();
	
	delete _editor;
}

//-------------------------
// save or load parameter
//-------------------------
void _stdcall sample::SaveRestoreState(IStream *Stream, BOOL Save)
{
	if( Save )
	{
		// save paremeters
		unsigned long length = 0;
		Stream->Write(_params, sizeof(_params), &length);
	}
	else
	{
		// load paremeters
		unsigned long length = 0;
		Stream->Read(_params, sizeof(_params), &length);
		for( int ii = 0; ii < NumParams; ii++ )
		{
			float normalizedValue = static_cast<float>(_params[ii]) / (1<<16);
			
			// Update internal parameters
			switch(ii)
			{
				case 0: _gain = normalizedValue; break;
				case 1: _attack = normalizedValue * 2.0f; break;
				case 2: _decay = normalizedValue * 2.0f; break;
				case 3: _sustain = normalizedValue; break;
				case 4: _release = normalizedValue * 4.0f; break;
			}

			if( _editor != nullptr )
			{
				// send message to editor
				_editor->setParameter(ii, static_cast<float>(_params[ii]));
			}
		}
	}
}

//----------------
// 
//----------------
intptr_t _stdcall sample::Dispatcher(intptr_t ID, intptr_t Index, intptr_t Value)
{
	if( ID == FPD_ShowEditor )
	{
		if (Value == 0)
		{
			// close editor
			delete _editor;
			_editor = nullptr;
			EditorHandle = 0;
		}
		else if( EditorHandle == 0 )
		{
			if (_editor == nullptr)
			{
				// first
				_editor = new sample_editor(this, reinterpret_cast<HWND>(Value));
			}

			// open editor
			EditorHandle = reinterpret_cast<HWND>(_editor->getHWND());
		}
		else
		{
			// change parent window ?
			::SetParent(EditorHandle, reinterpret_cast<HWND>(Value));
		}
	}
	return 0;
}

//----------------
// 
//----------------
void _stdcall sample::GetName(int Section, int Index, int Value, char *Name)
{
	if(Section == FPN_Param)
	{
		switch(Index)
		{
			case 0: strcpy_s(Name, 256, "Gain"); break;
			case 1: strcpy_s(Name, 256, "Attack"); break;
			case 2: strcpy_s(Name, 256, "Decay"); break;
			case 3: strcpy_s(Name, 256, "Sustain"); break;
			case 4: strcpy_s(Name, 256, "Release"); break;
			default: strcpy_s(Name, 256, "Unknown"); break;
		}
	}
}

int _stdcall sample::ProcessEvent(int EventID, int EventValue, int Flags)
{
	return 0;
}

//----------------
// 
//----------------
int _stdcall sample::ProcessParam(int Index, int Value, int RECFlags)
{
	int ret = 0;
	if( Index < NumParams )
	{
		if( RECFlags & REC_UpdateValue )
		{
			_params[Index] = Value;

			char hinttext[256] = { 0 };
			float normalizedValue = static_cast<float>(Value) / (1<<16);
			
			switch(Index)
			{
				case 0: // Gain
					_gain = normalizedValue;
					if( _gain < 1.0e-8)
					{
						sprintf_s(hinttext, "Gain: -oo dB");
					}
					else
					{
						sprintf_s(hinttext, "Gain: %.3f dB", 20.0 * log10(_gain));
					}
					break;
					
				case 1: // Attack
					_attack = normalizedValue * 2.0f; // 0-2 seconds
					sprintf_s(hinttext, "Attack: %.0f ms", _attack * 1000.0f);
					break;
					
				case 2: // Decay
					_decay = normalizedValue * 2.0f; // 0-2 seconds
					sprintf_s(hinttext, "Decay: %.0f ms", _decay * 1000.0f);
					break;
					
				case 3: // Sustain
					_sustain = normalizedValue;
					sprintf_s(hinttext, "Sustain: %.0f%%", _sustain * 100.0f);
					break;
					
				case 4: // Release
					_release = normalizedValue * 4.0f; // 0-4 seconds
					sprintf_s(hinttext, "Release: %.0f ms", _release * 1000.0f);
					break;
			}

			// display text to hint bar
			_host->OnHint(Index, hinttext);

			if( RECFlags & REC_UpdateControl )
			{
				// send message to editor
				if (_editor != nullptr)
				{
					_editor->setParameter(Index, static_cast<float>(Value));
				}
			}
			else
			{
				// send message to host
				_host->OnParamChanged(this->HostTag, Index, Value);
			}
		}
		else if( RECFlags & REC_GetValue )
		{
			// get parameter
			ret = _params[Index];
		}
	}
	return ret;
}

//----------------
// idle
//----------------
void _stdcall sample::Idle_Public()
{
	if (_editor) _editor->doIdleStuff();
}

//----------------
// effect
//----------------
void _stdcall sample::Eff_Render(PWAV32FS SourceBuffer, PWAV32FS DestBuffer, int Length)
{
	float gain = _gain;
	for (int ii = 0; ii < Length; ii++)
	{
		(*DestBuffer)[ii][0] = (*SourceBuffer)[ii][0] * gain;
		(*DestBuffer)[ii][1] = (*SourceBuffer)[ii][1] * gain;
	}
}

void _stdcall sample::Gen_Render(PWAV32FS DestBuffer, int& Length)
{
}

TVoiceHandle _stdcall sample::TriggerVoice(PVoiceParams VoiceParams, intptr_t SetTag)
{
	// Create a new voice
	Voice* voice = new Voice();
	voice->voiceParams = VoiceParams;
	voice->setTag = SetTag;
	voice->active = true;
	voice->released = false;
	
	// Get MIDI note from voice params
	voice->note = (int)(VoiceParams->InitLevels.Pitch / PITCH_SCALE + MIDDLE_C);
	
	// Get velocity (0-127 from FL Studio, convert to 0.0-1.0)
	int velocityValue = _host->Voice_GetEventValue(HostTag, SetTag, 0, FPV_GetVelocity);
	voice->velocity = (velocityValue / 16384.0f); // FL Studio velocity is 0-16384
	
	// Calculate phase increment for pitch
	float freq = noteToFrequency(voice->note);
	float sampleRate = DEFAULT_SAMPLE_RATE; // TODO: Query from host using PlugHost API
	voice->phaseIncrement = (freq * SAMPLE_LENGTH) / sampleRate;
	
	voice->phase = 0.0f;
	voice->envelope = 0.0f;
	voice->releaseCounter = 0;
	
	// Add to voice list
	_voices.push_back(voice);
	
	// Return voice pointer as handle
	return (TVoiceHandle)voice;
}

void _stdcall sample::Voice_Release(TVoiceHandle Handle)
{
	Voice* voice = (Voice*)Handle;
	if (voice && voice->active)
	{
		voice->released = true;
		voice->releaseStartEnv = voice->envelope;
		voice->releaseCounter = 0;
	}
}

void _stdcall sample::Voice_Kill(TVoiceHandle Handle)
{
	Voice* voice = (Voice*)Handle;
	if (voice)
	{
		// Remove from voice list
		for (auto it = _voices.begin(); it != _voices.end(); ++it)
		{
			if (*it == voice)
			{
				_voices.erase(it);
				break;
			}
		}
		delete voice;
	}
}

int _stdcall sample::Voice_ProcessEvent(TVoiceHandle Handle, int EventID, int EventValue, int Flags)
{
	return 0;
}

int _stdcall sample::Voice_Render(TVoiceHandle Handle, PWAV32FS DestBuffer, int& Length)
{
	Voice* voice = (Voice*)Handle;
	if (!voice || !voice->active)
	{
		Length = 0;
		return FVR_Ok;
	}
	
	for (int i = 0; i < Length; i++)
	{
		// Get sample from our sample data with interpolation
		int sampleIndex = (int)voice->phase;
		float frac = voice->phase - sampleIndex;
		
		float sample1 = 0.0f;
		float sample2 = 0.0f;
		
		// Check bounds and get samples
		if (sampleIndex >= 0 && sampleIndex < SAMPLE_LENGTH)
		{
			sample1 = _sampleData[sampleIndex];
			if (sampleIndex + 1 < SAMPLE_LENGTH)
				sample2 = _sampleData[sampleIndex + 1];
		}
		
		// Linear interpolation
		float sampleValue = sample1 + (sample2 - sample1) * frac;
		
		// Apply envelope
		float env = calculateEnvelope(voice, 1);
		
		// Apply velocity and gain
		sampleValue *= env * voice->velocity * _gain;
		
		// Output to both channels (stereo)
		(*DestBuffer)[i][0] = sampleValue;
		(*DestBuffer)[i][1] = sampleValue;
		
		// Advance phase
		voice->phase += voice->phaseIncrement;
		
		// Check if sample has finished playing
		if (voice->phase >= SAMPLE_LENGTH)
		{
			// For now, loop the sample (can be changed for one-shot samples)
			voice->phase = fmodf(voice->phase, (float)SAMPLE_LENGTH);
		}
		
		// If envelope is done, tell FL Studio to kill this voice
		if (voice->released && env <= 0.0001f)
		{
			voice->active = false;
			_host->Voice_Kill(HostTag, voice->setTag, true);
			Length = i;
			return FVR_Ok;
		}
	}
	
	return FVR_Ok;
}

void _stdcall sample::NewTick()
{
}

void _stdcall sample::MIDITick()
{
}

void _stdcall sample::MIDIIn(int& Msg)
{
}

void _stdcall sample::MsgIn(intptr_t Msg)
{
}

int _stdcall sample::OutputVoice_ProcessEvent(TOutVoiceHandle Handle, int EventID, int EventValue, int Flags)
{
	return 0;
}

void _stdcall sample::OutputVoice_Kill(TVoiceHandle Handle)
{
}

//----------------
// Helper Functions
//----------------

// Initialize sample data with a simple waveform (placeholder for actual samples)
void sample::initializeSampleData()
{
	// Create a simple sine wave with harmonics for a richer sound
	const float PI = 3.14159265358979323846f;
	for (int i = 0; i < SAMPLE_LENGTH; i++)
	{
		float t = (float)i / (float)SAMPLE_LENGTH;
		// Fundamental + harmonics for a richer tone
		_sampleData[i] = 0.6f * sinf(2.0f * PI * t * 2.0f)   // Fundamental
		               + 0.3f * sinf(2.0f * PI * t * 4.0f)   // 2nd harmonic
		               + 0.1f * sinf(2.0f * PI * t * 6.0f);  // 3rd harmonic
		
		// Apply a simple envelope to avoid clicks
		float env = 1.0f;
		if (t < 0.01f) env = t / 0.01f; // fade in
		if (t > 0.95f) env = (1.0f - t) / 0.05f; // fade out
		_sampleData[i] *= env;
	}
}

// Convert MIDI note to frequency
float sample::noteToFrequency(int note)
{
	// A4 (note 69) = 440 Hz
	return 440.0f * powf(2.0f, (note - 69) / 12.0f);
}

// Calculate envelope value for a voice
float sample::calculateEnvelope(Voice* voice, int numSamples)
{
	if (!voice->active) return 0.0f;
	
	float sampleRate = DEFAULT_SAMPLE_RATE; // TODO: Query from host using PlugHost API
	
	if (voice->released)
	{
		// Release phase
		voice->releaseCounter += numSamples;
		float releaseTime = _release * sampleRate;
		if (releaseTime < 1.0f) releaseTime = 1.0f;
		
		float releaseProgress = (float)voice->releaseCounter / releaseTime;
		if (releaseProgress >= 1.0f)
		{
			voice->envelope = 0.0f;
			return 0.0f;
		}
		voice->envelope = voice->releaseStartEnv * (1.0f - releaseProgress);
	}
	else
	{
		// Attack-Decay-Sustain phase
		float attackTime = _attack * sampleRate;
		float decayTime = _decay * sampleRate;
		float totalTime = voice->phase / voice->phaseIncrement;
		
		if (totalTime < attackTime)
		{
			// Attack phase
			voice->envelope = totalTime / attackTime;
		}
		else if (totalTime < attackTime + decayTime)
		{
			// Decay phase
			float decayProgress = (totalTime - attackTime) / decayTime;
			voice->envelope = 1.0f - (1.0f - _sustain) * decayProgress;
		}
		else
		{
			// Sustain phase
			voice->envelope = _sustain;
		}
	}
	
	return voice->envelope;
}
