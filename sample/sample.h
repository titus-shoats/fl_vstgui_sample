#pragma once

#include "fp_plugclass.h"
#include <vector>

const int NumParams = 5;	// the amount of parameters (Gain, Attack, Decay, Sustain, Release)

class sample_editor;

// Voice structure for polyphonic sample playback
struct Voice
{
	PVoiceParams voiceParams;  // FL Studio voice parameters
	intptr_t setTag;           // Voice tag from FL Studio
	bool active;               // Is this voice currently playing?
	bool released;             // Has note-off been received?
	int note;                  // MIDI note number
	float velocity;            // Note velocity (0.0 - 1.0)
	float phase;               // Current playback position
	float phaseIncrement;      // How much to increment phase per sample
	float envelope;            // Current envelope amplitude
	float releaseStartEnv;     // Envelope level when release started
	int releaseCounter;        // Samples since release
	
	Voice() : voiceParams(nullptr), setTag(0), active(false), released(false),
	          note(0), velocity(1.0f), phase(0.0f), phaseIncrement(0.0f),
	          envelope(0.0f), releaseStartEnv(0.0f), releaseCounter(0) {}
};

class sample : public TFruityPlug
{
public:
	sample(int Tag, TFruityPlugHost *Host);
	virtual ~sample();

	// from TFruityPlug
	virtual intptr_t _stdcall Dispatcher(intptr_t ID, intptr_t Index, intptr_t Value);
	virtual void _stdcall Idle_Public();
	virtual void _stdcall SaveRestoreState(IStream* Stream, BOOL Save);
	virtual void _stdcall GetName(int Section, int Index, int Value, char* Name);
	virtual int _stdcall ProcessEvent(int EventID, int EventValue, int Flags); 
	virtual int _stdcall ProcessParam(int Index, int Value, int RECFlags); 
	virtual void _stdcall Eff_Render(PWAV32FS SourceBuffer, PWAV32FS DestBuffer, int Length); 
	virtual void _stdcall Gen_Render(PWAV32FS DestBuffer, int& Length);  
	virtual TVoiceHandle _stdcall TriggerVoice(PVoiceParams VoiceParams, intptr_t SetTag); 
	virtual void _stdcall Voice_Release(TVoiceHandle Handle); 
	virtual void _stdcall Voice_Kill(TVoiceHandle Handle); 
	virtual int _stdcall Voice_ProcessEvent(TVoiceHandle Handle, int EventID, int EventValue, int Flags); 
	virtual int _stdcall Voice_Render(TVoiceHandle Handle, PWAV32FS DestBuffer, int& Length); 
	virtual void _stdcall NewTick();  
	virtual void _stdcall MIDITick();  
	virtual void _stdcall MIDIIn(int& Msg); 
	virtual void _stdcall MsgIn(intptr_t Msg); 
	virtual int _stdcall OutputVoice_ProcessEvent(TOutVoiceHandle Handle, int EventID, int EventValue, int Flags); 
	virtual void _stdcall OutputVoice_Kill(TVoiceHandle Handle); 

protected:
	// GUI
	sample_editor* _editor;

	// host
	TFruityPlugHost* _host;

	// parameter
	int _params[NumParams];

	// audio parameters
	float _gain;
	float _attack;   // Attack time in seconds
	float _decay;    // Decay time in seconds
	float _sustain;  // Sustain level (0.0 - 1.0)
	float _release;  // Release time in seconds
	
	// voice management
	std::vector<Voice*> _voices;
	
	// sample data (simple sine wave for now, will be replaced with actual samples)
	static const int SAMPLE_LENGTH = 44100; // 1 second at 44.1kHz
	float _sampleData[SAMPLE_LENGTH];
	
	// helper functions
	float calculateEnvelope(Voice* voice, int numSamples);
	float noteToFrequency(int note);
	void initializeSampleData();
};
