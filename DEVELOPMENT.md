# Trap R&B Rompler Development Guide

## Overview
This project converts the original FL Studio VSTGUI sample plugin into a **Trap R&B Sample-Based Rompler** instrument plugin.

## What Has Been Implemented

### Phase 1: Foundation ✓
- **Plugin Type**: Converted from `FPF_Type_Effect` to `FPF_Type_HybridGen` (generator/instrument)
- **Plugin Name**: "Trap RnB Rompler" (short name: "TrapRompler")
- **Architecture**: Voice-based polyphonic sample playback system

### Phase 2: Core Audio Engine ✓
- **Voice Management**: 
  - `Voice` structure for tracking polyphonic playback state
  - Dynamic voice allocation/deallocation
  - Per-voice envelope, phase tracking, and velocity sensitivity
  
- **Sample Playback**:
  - Linear interpolation for smooth pitch shifting
  - MIDI note-to-frequency conversion
  - Placeholder waveform (sine with harmonics) - ready for real samples
  
- **Voice Functions**:
  - `TriggerVoice`: Creates new voice on MIDI note-on
  - `Voice_Release`: Handles MIDI note-off
  - `Voice_Kill`: Cleans up voice resources
  - `Voice_Render`: Generates audio for each voice

- **ADSR Envelope**:
  - Attack: 0-2 seconds
  - Decay: 0-2 seconds
  - Sustain: 0-100% level
  - Release: 0-4 seconds
  - Full envelope calculation per-sample

### Phase 3: Parameters ✓
- **5 Parameters**:
  1. **Gain**: Volume control with dB display
  2. **Attack**: Envelope attack time
  3. **Decay**: Envelope decay time
  4. **Sustain**: Envelope sustain level
  5. **Release**: Envelope release time

- **Parameter Handling**:
  - Save/load state support
  - Real-time parameter updates
  - Hint bar display with units

### Phase 4: User Interface ✓
- **Window Size**: 280x100 pixels
- **Layout**: 5 knobs in a row with labels
- **Aesthetic**: 
  - Dark background (RGB: 30, 30, 35)
  - Orange/red knobs (RGB: 255, 100, 50) for trap/modern look
- **Controls**:
  - 5 rotary knobs with smooth response
  - Text labels: Gain, Attack, Decay, Sustain, Release
  - Mouse wheel support

## Building the Plugin

### Requirements
- **Windows** (FL Studio is Windows/Mac only, this build is for Windows)
- **Visual Studio 2019 or later** with C++ development tools
- **FL Studio Plugin SDK** (place in `fpsdk/source/c/` directory)
- **VSTGUI** (included as git submodule)

### Build Steps

1. **Initialize Git Submodules**:
   ```bash
   git submodule update --init --recursive
   ```

2. **Install FL Studio SDK**:
   - Download from: http://forum.image-line.com/viewtopic.php?t=12092
   - Extract to `fpsdk/source/c/` directory
   - Ensure `fp_plugclass.h` and other headers are present

3. **Open Solution**:
   - Open `sample/sample.sln` in Visual Studio
   - Select configuration: `Release | x64` or `Release | Win32`

4. **Build**:
   - Build → Build Solution (Ctrl+Shift+B)
   - DLL will be output to: `sample/x64/Release/sample.dll` or `sample/Win32/Release/sample.dll`

5. **Install Plugin**:
   - Copy the built DLL to FL Studio's plugin directory:
     - Usually: `C:\Program Files\Image-Line\FL Studio\Plugins\Fruity\Generators\`
   - Rename to something like `TrapRompler.dll`
   - Restart FL Studio

## Testing the Plugin

### Basic Testing
1. Open FL Studio
2. Add the "Trap RnB Rompler" to a channel
3. Open the plugin GUI
4. Play MIDI notes - you should hear a synthesized tone
5. Adjust ADSR knobs and hear the envelope changes
6. Test polyphony by playing chords

### Parameter Testing
- **Gain**: Adjust volume, should show dB values
- **Attack**: Short attack (left) for plucky sounds, long (right) for pads
- **Decay**: Controls how fast envelope decays to sustain level
- **Sustain**: Level held while key is down
- **Release**: Tail after key release

## Next Steps for Development

### Phase 5: Real Sample Integration
The current implementation uses a placeholder waveform. To add real samples:

1. **Prepare Samples**:
   - Collect trap R&B samples (808s, kicks, snares, hi-hats, etc.)
   - Format: WAV, mono or stereo, 44.1kHz recommended
   - Normalize and trim samples

2. **Load Samples**:
   - Modify `initializeSampleData()` to load from WAV files
   - Use a library like `dr_wav` (header-only) or Windows Multimedia API
   - Store multiple samples in an array/vector

3. **Sample Selection**:
   - Add parameter for sample selection (dropdown or knob)
   - Map MIDI notes to different samples
   - Implement velocity layers if needed

### Phase 6: Enhanced Features
- **Filter**: Add low-pass/high-pass filter for sound shaping
- **Pitch Envelope**: For 808-style pitch drops
- **Pitch Glide**: Portamento/glide between notes
- **Sample Looping**: Optional looping modes
- **Multi-sample Support**: Multiple samples per preset
- **Effects**: Built-in reverb, distortion, saturation

### Phase 7: UI Enhancements
- **Background Image**: Custom trap-themed graphics
- **Sample Display**: Waveform visualization
- **Preset Browser**: Save/load presets
- **More Controls**: Filter cutoff, resonance, pitch controls
- **Meter**: VU meter for output level

## Code Structure

### Key Files
- `sample/sample.h` - Main plugin class and Voice structure
- `sample/sample.cpp` - Plugin implementation, voice rendering
- `sample/sample_editor.h` - UI editor class
- `sample/sample_editor.cpp` - UI implementation
- `sample/FruityPlug.def` - DLL exports
- `sample/sample.vcxproj` - Visual Studio project

### Important Functions
- `TriggerVoice()` - Called on MIDI note-on
- `Voice_Render()` - Generates audio for one voice
- `Voice_Release()` - Called on MIDI note-off
- `Voice_Kill()` - Cleanup when voice ends
- `calculateEnvelope()` - ADSR envelope calculation
- `noteToFrequency()` - MIDI note to Hz conversion
- `initializeSampleData()` - Sample loading (currently placeholder)

## Known Limitations
1. **Sample Rate**: Currently hardcoded to 44.1kHz (should query from host)
2. **Placeholder Audio**: Uses synthetic waveform instead of real samples
3. **No Filter**: No built-in filtering yet
4. **Basic Looping**: Simple wraparound, no loop points
5. **No Multi-sampling**: One waveform across all notes

## Resources
- FL Studio SDK Docs: http://forum.image-line.com/viewtopic.php?t=12092
- VSTGUI Documentation: https://steinbergmedia.github.io/vst3_doc/vstgui/html/
- Trap Production Tips: Focus on 808 bass, crisp hi-hats, snappy snares

## License
Follow the licensing requirements of FL Studio SDK and VSTGUI.
