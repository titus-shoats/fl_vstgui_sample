# 🎵 Trap R&B Rompler - FL Studio Native Plugin

A polyphonic sample-based rompler instrument plugin for FL Studio, designed for trap and R&B music production.

## ✨ Features

- ✅ **Polyphonic Voice Management** - Unlimited simultaneous voices
- ✅ **Full ADSR Envelope** - Attack, Decay, Sustain, Release controls
- ✅ **MIDI Velocity Sensitive** - Dynamic response to playing style
- ✅ **Modern UI** - Dark theme with 5 orange/red knobs (280x100px)
- ✅ **FL Studio Integration** - Native plugin with automation support
- ✅ **State Persistence** - Save/load with project files

## 🚀 Quick Start

**Want to build and use this plugin? Start here:**
- 📘 **[QUICKSTART.md](QUICKSTART.md)** - Build instructions and testing guide
- ✅ **[BUILD_CHECKLIST.md](BUILD_CHECKLIST.md)** - Step-by-step checklist for building

## 📚 Documentation

### For Developers
- 🏗️ **[ARCHITECTURE.md](ARCHITECTURE.md)** - System architecture, diagrams, data flow
- 📖 **[DEVELOPMENT.md](DEVELOPMENT.md)** - Implementation guide, roadmap, features
- 🎨 **[UI_DESIGN.md](UI_DESIGN.md)** - UI mockups, color scheme, layout specs

### Summary
- 📋 **[PROJECT_SUMMARY.md](PROJECT_SUMMARY.md)** - Complete project overview

## 🎹 Current Status

**Phase 1-4 Complete** ✅ - Core engine fully functional!

The plugin currently has:
- Polyphonic sample playback engine
- Complete ADSR envelope generator
- 5 parameter controls (Gain, Attack, Decay, Sustain, Release)
- Modern dark UI with orange/red knobs
- Placeholder waveform (sine + harmonics)

**Next**: Replace placeholder with real trap/R&B samples, add filters

## 🛠️ Building

### Requirements
- Windows 10/11
- Visual Studio 2019 or later
- FL Studio Plugin SDK ([download](http://forum.image-line.com/viewtopic.php?t=12092))
- FL Studio 20.8+

### Quick Build
```bash
# 1. Clone and setup
git clone https://github.com/titus-shoats/fl_vstgui_sample.git
cd fl_vstgui_sample
git submodule update --init --recursive

# 2. Download FL SDK and extract to: fpsdk/source/c/

# 3. Open sample/sample.sln in Visual Studio

# 4. Build: Release | x64

# 5. Copy sample.dll to FL Studio's Generators folder
```

See **[BUILD_CHECKLIST.md](BUILD_CHECKLIST.md)** for detailed instructions.

## 🎛️ Parameters

| Parameter | Range      | Description                    |
|-----------|------------|--------------------------------|
| Gain      | 0-100%     | Master volume control          |
| Attack    | 0-2000ms   | Envelope attack time           |
| Decay     | 0-2000ms   | Envelope decay time            |
| Sustain   | 0-100%     | Envelope sustain level         |
| Release   | 0-4000ms   | Envelope release time          |

## 🎨 UI Preview

```
┌───────────────────────────────────────────────┐
│        Trap RnB Rompler (280x100)            │
├───────────────────────────────────────────────┤
│                                               │
│   ○      ○      ○      ○      ○              │
│  Gain  Attack Decay Sustain Release          │
│                                               │
└───────────────────────────────────────────────┘
```

Dark background (RGB: 30,30,35) with orange/red knobs (RGB: 255,100,50)

## 🗺️ Development Roadmap

### ✅ Completed
- [x] **Phase 1**: Plugin architecture conversion (Effect → Generator)
- [x] **Phase 2**: Voice management & sample playback engine
- [x] **Phase 3**: ADSR envelope & velocity sensitivity
- [x] **Phase 4**: UI with 5 parameter knobs

### 📋 Upcoming
- [ ] **Phase 5**: Real sample integration (WAV loading)
- [ ] **Phase 6**: Filters and advanced features
- [ ] **Phase 7**: Sample browser UI
- [ ] **Phase 8**: Polish and optimization

## 🎼 Technical Details

- **Plugin Type**: FPF_Type_HybridGen (Voice-based generator)
- **Audio Format**: 32-bit float, stereo
- **Sample Rate**: 44.1 kHz
- **Polyphony**: Unlimited (FL Studio managed)
- **UI Framework**: VSTGUI 4.x
- **Language**: C++ (Windows native)

## 📖 Learning Resources

This plugin demonstrates:
- FL Studio hybrid generator development
- Polyphonic voice management
- ADSR envelope implementation
- Real-time audio processing
- VSTGUI interface design

## 🤝 Contributing

This project is built step-by-step for learning. See [DEVELOPMENT.md](DEVELOPMENT.md) for the complete roadmap.

## 📄 License

Follow the licensing terms of:
- FL Studio Plugin SDK (Image-Line)
- VSTGUI (Steinberg Media Technologies)

## 🎉 Credits

- **Original Template**: [fukuroder/fl_vstgui_sample](https://github.com/fukuroder/fl_vstgui_sample)
- **FL Studio SDK**: Image-Line Software
- **VSTGUI**: Steinberg Media Technologies

---

**Status**: ✅ Core engine complete, ready for building and testing!

For detailed build instructions, see **[BUILD_CHECKLIST.md](BUILD_CHECKLIST.md)**

