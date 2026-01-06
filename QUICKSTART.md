# Trap R&B Rompler - Quick Start

## What This Is
A native FL Studio generator plugin (rompler) designed for trap and R&B music production. Built step-by-step from the fl_vstgui_sample template.

## Current Status: Core Engine Complete ✓

### What Works
- ✅ Polyphonic sample playback (unlimited voices)
- ✅ MIDI note triggering with velocity sensitivity
- ✅ Full ADSR envelope (Attack, Decay, Sustain, Release)
- ✅ Gain control with dB metering
- ✅ Modern dark UI with 5 knobs
- ✅ Parameter automation support
- ✅ State save/load

### Plugin Parameters
1. **Gain** - Master volume (0-100%, displayed in dB)
2. **Attack** - Envelope attack time (0-2000ms)
3. **Decay** - Envelope decay time (0-2000ms)
4. **Sustain** - Envelope sustain level (0-100%)
5. **Release** - Envelope release time (0-4000ms)

## Building (Windows Only)

### Prerequisites
```
- Windows 10/11
- Visual Studio 2019+ (with C++ Desktop Development)
- FL Studio Plugin SDK (download separately)
- Git (for submodules)
```

### Quick Build
```bash
# 1. Clone and setup
git clone https://github.com/titus-shoats/fl_vstgui_sample.git
cd fl_vstgui_sample
git submodule update --init --recursive

# 2. Download FL SDK from http://forum.image-line.com/viewtopic.php?t=12092
#    Extract to: fpsdk/source/c/

# 3. Open in Visual Studio
sample/sample.sln

# 4. Build (Ctrl+Shift+B)
# Select: Release | x64

# 5. Install
# Copy: sample/x64/Release/sample.dll
# To: C:\Program Files\Image-Line\FL Studio\Plugins\Fruity\Generators\
# Rename: TrapRompler.dll
```

## UI Preview
```
┌─────────────────────────────────────────────────────────────┐
│  Trap RnB Rompler                                           │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│    ◉        ◉        ◉        ◉        ◉                   │
│   Gain    Attack   Decay   Sustain  Release                │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

## Next Development Phases

### Phase 5: Real Sample Integration (Next)
- [ ] Load WAV samples from disk
- [ ] Multi-sample support (kicks, snares, 808s, etc.)
- [ ] Sample selection parameter
- [ ] MIDI key mapping

### Phase 6: Trap Features
- [ ] Low-pass filter with resonance
- [ ] 808-style pitch envelope
- [ ] Pitch glide/portamento
- [ ] Saturation/distortion
- [ ] Sample velocity layers

### Phase 7: Professional Polish
- [ ] Custom background graphics
- [ ] Preset management
- [ ] Sample browser UI
- [ ] Waveform display
- [ ] VU meter

## Technical Details

### Plugin Type
`FPF_Type_HybridGen` - Voice-based generator with FL Studio sampler integration

### Architecture
- **Voice Management**: Dynamic voice allocation per MIDI note
- **Rendering**: Per-voice rendering with sample-accurate envelopes
- **Sample Playback**: Linear interpolation for pitch shifting
- **Format**: 32-bit floating point, stereo output

### Code Highlights
```cpp
// Voice structure - manages each playing note
struct Voice {
    PVoiceParams voiceParams;  // FL Studio parameters
    bool active, released;
    int note;                   // MIDI note number
    float velocity;            // 0.0 - 1.0
    float phase;               // Sample position
    float envelope;            // Current ADSR value
    // ... more fields
};

// Main rendering loop
int Voice_Render(TVoiceHandle Handle, PWAV32FS DestBuffer, int& Length) {
    // Get sample with interpolation
    // Apply ADSR envelope
    // Apply velocity and gain
    // Output stereo
}
```

## File Structure
```
sample/
├── sample.h              # Plugin class + Voice struct
├── sample.cpp            # Core logic + audio rendering  
├── sample_editor.h       # UI class
├── sample_editor.cpp     # UI implementation (5 knobs)
├── sample.vcxproj        # Visual Studio project
├── sample.sln            # Visual Studio solution
└── FruityPlug.def        # DLL exports

vstgui/                   # Git submodule (UI framework)
fpsdk/                    # FL Studio SDK (download separately)
```

## Testing in FL Studio
1. Load plugin as a generator (not effect!)
2. Open the GUI - you'll see 5 orange knobs
3. Draw notes in piano roll or play MIDI keyboard
4. Adjust ADSR for different envelope shapes:
   - Quick pluck: A=1ms, D=100ms, S=0%, R=50ms
   - Pad: A=500ms, D=200ms, S=70%, R=2000ms
   - 808: A=1ms, D=300ms, S=0%, R=400ms

## Troubleshooting

**Plugin doesn't appear in FL Studio**
- Ensure DLL is in correct folder (Generators, not Effects)
- Check FL Studio version compatibility (v20.8+)
- Restart FL Studio after installing

**No sound**
- Check gain knob isn't at zero
- Ensure sustain level > 0% if holding notes
- Verify MIDI input is triggering notes

**Crashes**
- Ensure FL SDK headers match your FL Studio version
- Check all submodules are initialized
- Try Debug build for better error messages

## Performance
- **CPU Usage**: Very low (simple waveform playback)
- **Voice Limit**: Unlimited (FL Studio manages voice stealing)
- **Latency**: Near-zero (direct rendering)
- **Memory**: ~1MB base + 4 bytes per sample

## Contributing
This is built step-by-step for learning. Current phase: Core engine complete.
Next: Real sample integration. See DEVELOPMENT.md for detailed roadmap.

## Credits
- Original template: fukuroder/fl_vstgui_sample
- FL Studio SDK: Image-Line
- VSTGUI: Steinberg Media Technologies

## License
Follow FL Studio SDK and VSTGUI licensing terms.
