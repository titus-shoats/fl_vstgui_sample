# Trap R&B Rompler - Architecture Overview

## System Architecture

```
┌─────────────────────────────────────────────────────────────────┐
│                         FL Studio Host                          │
│  - MIDI Event Processing                                        │
│  - Audio Buffer Management                                      │
│  - Parameter Automation                                         │
└────────────────────┬───────────────────────┬────────────────────┘
                     │                       │
                     ▼                       ▼
        ┌────────────────────┐    ┌──────────────────────┐
        │  TFruityPlugHost   │    │   VSTGUI Framework   │
        │  - Voice_GetEvent  │    │   - CFrame           │
        │  - Voice_Kill      │    │   - CKnob            │
        │  - OnParamChanged  │    │   - CTextLabel       │
        └──────────┬─────────┘    └──────────┬───────────┘
                   │                         │
                   │                         │
        ┌──────────▼─────────────────────────▼───────────┐
        │         sample (Main Plugin Class)             │
        │  Type: FPF_Type_HybridGen                      │
        │                                                 │
        │  Parameters:                                    │
        │    - Gain (0-100%)                             │
        │    - Attack (0-2s)                             │
        │    - Decay (0-2s)                              │
        │    - Sustain (0-100%)                          │
        │    - Release (0-4s)                            │
        │                                                 │
        │  Voice Management:                             │
        │    - std::vector<Voice*> _voices               │
        │    - Dynamic allocation/deallocation           │
        │                                                 │
        │  Sample Data:                                  │
        │    - float _sampleData[44100]                  │
        │    - Waveform with harmonics (placeholder)     │
        └──────────┬──────────────────────────────────────┘
                   │
                   │
        ┌──────────▼─────────────────────────────────────┐
        │          sample_editor (UI Class)              │
        │  Size: 280x100 pixels                          │
        │  Background: RGB(30, 30, 35) - Dark Gray       │
        │                                                 │
        │  Controls (5 CKnob instances):                 │
        │    ◉ Gain      - Tag 0                         │
        │    ◉ Attack    - Tag 1                         │
        │    ◉ Decay     - Tag 2                         │
        │    ◉ Sustain   - Tag 3                         │
        │    ◉ Release   - Tag 4                         │
        │                                                 │
        │  Color: RGB(255, 100, 50) - Orange/Red         │
        └────────────────────────────────────────────────┘
```

## Voice Lifecycle

```
MIDI Note On
     │
     ▼
┌─────────────────────────────────┐
│  TriggerVoice()                 │
│  - Allocate new Voice struct    │
│  - Get note, velocity           │
│  - Calculate frequency          │
│  - Set initial envelope state   │
│  - Return voice handle          │
└────────────┬────────────────────┘
             │
             ▼
┌─────────────────────────────────┐
│  Voice_Render() [loop]          │◄─────┐
│  - Get sample with interpolation│      │
│  - Calculate ADSR envelope      │      │
│  - Apply velocity * gain        │      │
│  - Output stereo samples        │      │
│  - Increment phase              │      │
└────────────┬────────────────────┘      │
             │                           │
             ├───────────────────────────┘
             │  (continues until note off or sample end)
             │
             ▼
MIDI Note Off
     │
     ▼
┌─────────────────────────────────┐
│  Voice_Release()                │
│  - Set released flag            │
│  - Store envelope level         │
│  - Start release counter        │
└────────────┬────────────────────┘
             │
             ▼
┌─────────────────────────────────┐
│  Voice_Render() [release phase] │◄─────┐
│  - Fade envelope to 0           │      │
│  - Continue rendering           │      │
└────────────┬────────────────────┘      │
             │                           │
             ├───────────────────────────┘
             │  (continues until envelope = 0)
             │
             ▼
┌─────────────────────────────────┐
│  Voice_Kill()                   │
│  - Remove from voice list       │
│  - Free memory                  │
│  - End playback                 │
└─────────────────────────────────┘
```

## ADSR Envelope State Machine

```
Time ─────────────────────────────────────────────────►

Amplitude
    │
1.0 ├────╱╲
    │   ╱  ╲____
    │  ╱        ╲___________
0.7 ├─╱               ─────╲
    │╱                      ╲
    │                        ╲____
0.0 ├──────────────────────────────╲___________
    │                               
    │ A   D     Sustain        R
    │ │   │                    │
    └─┴───┴────────────────────┴──────────────►
      │   │                    │
      │   │                    └─ Release (note off)
      │   └─ Decay to sustain level
      └─ Attack from 0 to peak

States:
1. Attack:   env = time / attackTime
2. Decay:    env = 1.0 - (1.0 - sustain) * (time / decayTime)
3. Sustain:  env = sustainLevel (constant)
4. Release:  env = sustainLevel * (1.0 - time / releaseTime)
```

## Audio Rendering Pipeline

```
┌────────────────────────────────────────────────────────┐
│ FL Studio Audio Thread (per audio buffer)             │
└──────────────────────┬─────────────────────────────────┘
                       │
          ┌────────────▼──────────────┐
          │ For each active voice:    │
          └────────────┬──────────────┘
                       │
    ┌──────────────────▼───────────────────────┐
    │ 1. Calculate sample position (phase)     │
    │    - Integer part: sample index          │
    │    - Fractional part: interpolation      │
    └──────────────────┬───────────────────────┘
                       │
    ┌──────────────────▼───────────────────────┐
    │ 2. Get samples with linear interpolation │
    │    sample = s[i] + (s[i+1] - s[i]) * frac│
    └──────────────────┬───────────────────────┘
                       │
    ┌──────────────────▼───────────────────────┐
    │ 3. Calculate ADSR envelope               │
    │    - Check phase (A/D/S/R)               │
    │    - Compute envelope value (0.0-1.0)    │
    └──────────────────┬───────────────────────┘
                       │
    ┌──────────────────▼───────────────────────┐
    │ 4. Apply modulation                      │
    │    output = sample * envelope * velocity │
    │             * gain                       │
    └──────────────────┬───────────────────────┘
                       │
    ┌──────────────────▼───────────────────────┐
    │ 5. Write to stereo buffer                │
    │    buffer[i][0] = output  // left        │
    │    buffer[i][1] = output  // right       │
    └──────────────────┬───────────────────────┘
                       │
    ┌──────────────────▼───────────────────────┐
    │ 6. Advance phase                         │
    │    phase += phaseIncrement               │
    └──────────────────┬───────────────────────┘
                       │
          ┌────────────▼──────────────┐
          │ Check if voice finished:  │
          │ - Envelope reached 0?     │
          │ - Sample ended?           │
          │ → Call Voice_Kill if done │
          └───────────────────────────┘
```

## Parameter Flow

```
User adjusts knob in UI
         │
         ▼
┌────────────────────────┐
│ sample_editor::        │
│ valueChanged()         │
│ - Get knob value       │
│ - Get knob tag (0-4)   │
└──────────┬─────────────┘
           │
           ▼
┌────────────────────────┐
│ sample::ProcessParam() │
│ - Store param value    │
│ - Convert to float     │
│ - Update internal var  │
│ - Format hint text     │
└──────────┬─────────────┘
           │
           ├─────────────────────┐
           │                     │
           ▼                     ▼
┌──────────────────┐   ┌──────────────────┐
│ Update host      │   │ Display hint     │
│ automation       │   │ "Attack: 50ms"   │
└──────────────────┘   └──────────────────┘
```

## Memory Layout

```
sample instance (~1MB)
├── PlugInfo structure (80 bytes)
├── _params[5] (20 bytes)
├── _gain, _attack, _decay, _sustain, _release (20 bytes)
├── _sampleData[44100] (176 KB)
├── _voices vector
│   ├── Voice* 1 (64 bytes)
│   ├── Voice* 2 (64 bytes)
│   └── ... (up to max polyphony)
└── _editor (280x100 UI)
    ├── CFrame (platform window)
    ├── CKnob[5] controls
    └── CTextLabel[5] labels
```

## Thread Safety

```
┌─────────────────────────────────────────────────────┐
│ FL Studio manages thread safety                    │
│                                                     │
│ Audio Thread (RT):                                  │
│   - Voice_Render()       [lock-free reads]        │
│   - TriggerVoice()       [atomic allocation]      │
│   - Voice_Release()      [flag set]               │
│   - Voice_Kill()         [deferred cleanup]       │
│                                                     │
│ GUI Thread:                                         │
│   - ProcessParam()       [lock-free writes]       │
│   - setParameter()       [atomic updates]         │
│   - Idle_Public()        [UI refresh]             │
│                                                     │
│ Note: FL Studio ensures parameters are updated     │
│       safely between threads                       │
└─────────────────────────────────────────────────────┘
```

## Data Flow Summary

```
MIDI Input → TriggerVoice → Voice Creation
                              ↓
                         Voice_Render (loop)
                              ↓
                    Sample Read + Interpolate
                              ↓
                      ADSR Envelope Apply
                              ↓
                    Velocity + Gain Apply
                              ↓
                      Stereo Output Buffer
                              ↓
                         FL Studio Mixer

UI Interaction → valueChanged → ProcessParam → Parameter Update
                                                      ↓
                                              Affects Voice_Render
```
