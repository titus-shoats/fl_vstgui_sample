# Build & Test Checklist

This checklist guides you through building and testing the Trap R&B Rompler plugin on Windows.

## Prerequisites Checklist

### Software Requirements
- [ ] Windows 10 or 11 (64-bit recommended)
- [ ] Visual Studio 2019 or 2022
  - [ ] "Desktop development with C++" workload installed
  - [ ] Windows 10 SDK installed
- [ ] FL Studio 20.8 or later
- [ ] Git for Windows

### Download Required Components
- [ ] Clone this repository: `git clone https://github.com/titus-shoats/fl_vstgui_sample.git`
- [ ] Initialize submodules: `git submodule update --init --recursive`
- [ ] Download FL Studio SDK from http://forum.image-line.com/viewtopic.php?t=12092
  - [ ] Extract to: `fpsdk/source/c/` directory
  - [ ] Verify `fp_plugclass.h` exists at `fpsdk/source/c/fp_plugclass.h`

## Build Process

### Step 1: Verify Project Structure
```
Your folder structure should look like:
fl_vstgui_sample/
├── sample/
│   ├── sample.sln          ✓ Solution file
│   ├── sample.vcxproj      ✓ Project file
│   ├── sample.h            ✓ Header
│   ├── sample.cpp          ✓ Implementation
│   ├── sample_editor.h     ✓ UI header
│   ├── sample_editor.cpp   ✓ UI implementation
│   ├── FruityPlug.def      ✓ Exports
│   └── background.png      (optional)
├── fpsdk/
│   └── source/
│       └── c/
│           ├── fp_plugclass.h   ✓ Must have!
│           ├── fp_def.h         ✓ Must have!
│           └── ... (other SDK files)
├── vstgui/
│   └── vstgui/              ✓ Git submodule
└── QUICKSTART.md
```

- [ ] All files present
- [ ] SDK headers in correct location

### Step 2: Open in Visual Studio
1. [ ] Navigate to `sample/` folder
2. [ ] Double-click `sample.sln`
3. [ ] Visual Studio opens the solution

### Step 3: Configure Build
1. [ ] Select configuration dropdown (top toolbar)
2. [ ] Choose: **Release | x64** (recommended)
   - Alternatives: Release | Win32, Debug | x64, Debug | Win32
3. [ ] Verify platform matches your FL Studio (64-bit → x64, 32-bit → Win32)

### Step 4: Check Include Paths
1. [ ] Right-click project → Properties
2. [ ] C/C++ → General → Additional Include Directories should include:
   - `../fpsdk/source/c/`
   - `../vstgui/vstgui/`
3. [ ] If missing, add them manually

### Step 5: Build
1. [ ] Build → Build Solution (or press `Ctrl+Shift+B`)
2. [ ] Watch Output window for progress
3. [ ] Wait for "Build succeeded" message

**Expected output location:**
- x64 Release: `sample/x64/Release/sample.dll`
- Win32 Release: `sample/Win32/Release/sample.dll`

### Step 6: Verify Build
- [ ] DLL file created (check file size ~100-500 KB)
- [ ] No error messages in Output window
- [ ] Warnings are OK (but fewer is better)

## Installation

### Step 7: Locate FL Studio Plugin Folder
Common locations:
```
C:\Program Files\Image-Line\FL Studio 20\Plugins\Fruity\Generators\
C:\Program Files\Image-Line\FL Studio 21\Plugins\Fruity\Generators\
```

Or check in FL Studio:
- Options → File Settings → "Browse plugin databases..."
- Look for "Generators" folder

- [ ] Found FL Studio Generators folder

### Step 8: Copy Plugin
1. [ ] Copy `sample.dll` to Generators folder
2. [ ] Rename to `TrapRompler.dll` (optional but recommended)
3. [ ] Keep original name if preferred

### Step 9: Restart FL Studio
- [ ] Close FL Studio completely
- [ ] Launch FL Studio

## Testing

### Test 1: Plugin Loads
1. [ ] In FL Studio, click channel rack "+"
2. [ ] Navigate to: More plugins → Generators
3. [ ] Look for "Trap RnB Rompler" or "TrapRompler"
4. [ ] Click to load

**Expected:** Plugin loads without errors

### Test 2: GUI Opens
1. [ ] Click gear icon on channel
2. [ ] Select plugin name to open GUI

**Expected:**
- [ ] Window opens (280x100 pixels)
- [ ] Dark gray background
- [ ] 5 orange/red knobs visible
- [ ] Labels: Gain, Attack, Decay, Sustain, Release

### Test 3: Basic Sound
1. [ ] Draw a C5 note in piano roll (middle C)
2. [ ] Play pattern

**Expected:**
- [ ] Sound plays (simple tone with harmonics)
- [ ] No crackling or distortion
- [ ] Sound stops when pattern ends

### Test 4: Polyphony
1. [ ] Draw a C major chord (C, E, G)
2. [ ] Play pattern

**Expected:**
- [ ] All three notes play simultaneously
- [ ] Clean sound, no voice stealing issues

### Test 5: ADSR Envelope
1. [ ] Set Attack to minimum (knob left)
2. [ ] Set Release to minimum (knob left)
3. [ ] Draw short note, play

**Expected:** Quick pluck sound

4. [ ] Set Attack to middle
5. [ ] Set Release to maximum (knob right)
6. [ ] Draw short note, play

**Expected:** Slow fade-in and long fade-out

### Test 6: Parameter Automation
1. [ ] Right-click Gain knob
2. [ ] Select "Create automation clip"
3. [ ] Draw automation curve
4. [ ] Play pattern

**Expected:**
- [ ] Gain changes according to automation
- [ ] Smooth transitions

### Test 7: Velocity Sensitivity
1. [ ] Draw note with full velocity (127)
2. [ ] Draw note with low velocity (20)
3. [ ] Play both

**Expected:**
- [ ] Loud note is louder than soft note
- [ ] Clear volume difference

### Test 8: State Save/Load
1. [ ] Adjust all knobs to unique positions
2. [ ] Save project
3. [ ] Close FL Studio
4. [ ] Reopen project

**Expected:**
- [ ] Plugin loads with same knob positions
- [ ] Sound is identical

## Troubleshooting

### Plugin Doesn't Appear
- [ ] Check you copied to **Generators** folder (not Effects!)
- [ ] Verify FL Studio version matches DLL architecture (32/64-bit)
- [ ] Try: Options → Manage plugins → Refresh plugin list
- [ ] Check Windows Event Viewer for DLL load errors

### Plugin Crashes on Load
- [ ] Build in Debug configuration
- [ ] Attach Visual Studio debugger to FL Studio process
- [ ] Load plugin and check error messages
- [ ] Verify all SDK headers are correct version

### No Sound
- [ ] Check FL Studio mixer - is channel muted?
- [ ] Verify Gain knob isn't at zero
- [ ] Check Sustain level > 0
- [ ] Try different MIDI notes

### Crackling/Glitches
- [ ] Increase FL Studio buffer size (F10 → Audio settings)
- [ ] Close other programs
- [ ] Check CPU usage
- [ ] Try Debug build to identify issues

### Knobs Don't Respond
- [ ] Check mouse/trackpad is working
- [ ] Try mouse wheel on knob
- [ ] Verify no automation overriding manual control
- [ ] Reopen GUI window

### Wrong Notes
- [ ] Verify MIDI input/keyboard is sending correct notes
- [ ] Check `noteToFrequency()` calculation
- [ ] Ensure sample rate matches FL Studio settings

## Performance Testing

### CPU Usage Test
1. [ ] Create 16 instances of plugin
2. [ ] Play complex pattern with many notes
3. [ ] Check FL Studio CPU meter (F9)

**Expected:** < 20% CPU usage on modern PC

### Polyphony Test
1. [ ] Draw pattern with 50+ simultaneous notes
2. [ ] Play pattern

**Expected:**
- [ ] All notes play
- [ ] No voice stealing (FL Studio manages this)
- [ ] No crashes or glitches

### Automation Test
1. [ ] Create automation for all 5 parameters
2. [ ] Draw complex curves
3. [ ] Play pattern

**Expected:**
- [ ] Smooth parameter changes
- [ ] No audio dropouts
- [ ] Parameters update in real-time

## Next Development Steps

Once basic testing passes:

### Phase 5: Real Samples
- [ ] Prepare trap/R&B WAV samples
- [ ] Implement WAV file loading (use dr_wav.h)
- [ ] Replace `initializeSampleData()` with file loader
- [ ] Test with real samples

### Phase 6: Advanced Features
- [ ] Add low-pass filter
- [ ] Implement sample selection UI
- [ ] Add pitch envelope for 808-style sounds
- [ ] Create sample browser

### Phase 7: Polish
- [ ] Custom background graphics
- [ ] Preset system
- [ ] Better waveform (or multi-sample)
- [ ] User manual

## Bug Reporting

If you encounter issues:

1. **Note the exact steps** to reproduce
2. **Check Output/Debug console** for errors
3. **Capture screenshot** if UI-related
4. **Note system specs**: Windows version, FL Studio version, CPU
5. **Report on GitHub** with all above information

## Success Criteria

Plugin is working correctly when:
- [x] Loads in FL Studio without errors
- [x] GUI displays properly
- [x] Plays sound on MIDI input
- [x] Supports polyphony
- [x] ADSR envelope works
- [x] Parameters automate
- [x] State saves/loads
- [x] No crashes or glitches

## Completion

- [ ] All tests passed
- [ ] Performance is acceptable
- [ ] Ready for real sample integration

**Congratulations!** You have a working FL Studio rompler plugin.

Next: Add real samples and advanced features per DEVELOPMENT.md.
