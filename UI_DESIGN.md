# Trap R&B Rompler - UI Mockup

## Current UI Design (280x100 pixels)

```
╔═══════════════════════════════════════════════════════════════╗
║                    Trap RnB Rompler                           ║
╠═══════════════════════════════════════════════════════════════╣
║                                                               ║
║       ┌─────┐      ┌─────┐      ┌─────┐      ┌─────┐        ║
║       │  ○  │      │  ○  │      │  ○  │      │  ○  │        ║
║   ○   │ ╱ ╲ │      │ ╱ ╲ │      │ ╱ ╲ │      │ ╱ ╲ │   ○    ║
║  ╱ ╲  │╱   ╲│      │╱   ╲│      │╱   ╲│      │╱   ╲│  ╱ ╲   ║
║ ╱   ╲ └─────┘      └─────┘      └─────┘      └─────┘ ╱   ╲  ║
║╱     ╲                                                ╱     ╲ ║
║                                                               ║
║  Gain    Attack     Decay     Sustain    Release             ║
║                                                               ║
╚═══════════════════════════════════════════════════════════════╝
```

## Color Scheme

**Background**: `RGB(30, 30, 35)` - Dark charcoal gray
**Knobs**: `RGB(255, 100, 50)` - Orange/red gradient
**Labels**: `RGB(200, 200, 200)` - Light gray text
**Border**: None (frameless window)

## Knob Specifications

- **Size**: 40x40 pixels each
- **Type**: Rotary (CKnob)
- **Range**: 270° rotation (5/6 of full circle)
- **Start Angle**: 240° (4 o'clock position)
- **Handle**: 3px wide line, orange/red color
- **Interaction**: 
  - Mouse drag (vertical)
  - Mouse wheel
  - Double-click to reset

## Layout Dimensions

```
┌─────────────────────────────────────────────────────────────┐
│ 0,0                                            280,0        │
│                                                             │
│  20px margin                                                │
│     │                                                       │
│     ▼                                                       │
│    ┌─┐  ┌─┐  ┌─┐  ┌─┐  ┌─┐                               │
│    │○│  │○│  │○│  │○│  │○│  ← 40px knobs                 │
│ 20 └─┘  └─┘  └─┘  └─┘  └─┘                               │
│ px  ↑    ↑    ↑    ↑    ↑                                 │
│ top │    │    │    │    │                                  │
│     └────┴────┴────┴────┘                                  │
│        50px spacing                                         │
│                                                             │
│    Text  Text  Text  Text  Text                            │
│ 65 ───────────────────────────── ← Label row              │
│ px                                                          │
│                                                             │
│                                                             │
│ 0,100                                        280,100       │
└─────────────────────────────────────────────────────────────┘
```

## Parameter Ranges & Display

| Parameter | Range      | Display Format      | Default |
|-----------|------------|---------------------|---------|
| Gain      | 0-100%     | "Gain: X.X dB"      | 100%    |
| Attack    | 0-2000ms   | "Attack: X ms"      | 10ms    |
| Decay     | 0-2000ms   | "Decay: X ms"       | 100ms   |
| Sustain   | 0-100%     | "Sustain: X%"       | 70%     |
| Release   | 0-4000ms   | "Release: X ms"     | 200ms   |

## Future UI Enhancements (Phase 7)

### Expanded Layout Concept (600x400 pixels)
```
╔═════════════════════════════════════════════════════════════════╗
║                     Trap RnB Rompler                            ║
╠═════════════════════════════════════════════════════════════════╣
║  ┌─────────────────────────────────────────────────────────┐   ║
║  │ Sample Browser                                          │   ║
║  │  • 808 Bass A    [▶]                                    │   ║
║  │  • 808 Bass B    [▶]                                    │   ║
║  │  • Kick Heavy    [▶]                                    │   ║
║  │  • Snare Trap    [▶]                                    │   ║
║  │  • Hi-Hat Closed [▶]                                    │   ║
║  └─────────────────────────────────────────────────────────┘   ║
║                                                                 ║
║  ┌─────────────────────────────────────────────────────────┐   ║
║  │ Waveform Display                                        │   ║
║  │  ╭─╮                                                    │   ║
║  │  │ ╰╮╭─╮                                                │   ║
║  │ ─┴──╰╯─╰─────────────────────────────────────────────  │   ║
║  └─────────────────────────────────────────────────────────┘   ║
║                                                                 ║
║  Envelope                Filter                Effects          ║
║  ┌───────────┐         ┌───────────┐        ┌───────────┐     ║
║  │ ○  ○  ○  ○│         │ ○  ○      │        │ ○  ○      │     ║
║  │A  D  S  R │         │Cut Res    │        │Rev Sat    │     ║
║  └───────────┘         └───────────┘        └───────────┘     ║
║                                                                 ║
║  Pitch                 Output                                   ║
║  ┌───────────┐         ┌───────────┐                           ║
║  │ ○  ○      │         │ ─────── ○ │ VU Meter                 ║
║  │Env Glide  │         │ Gain      │                           ║
║  └───────────┘         └───────────┘                           ║
╚═════════════════════════════════════════════════════════════════╝
```

### Planned Features for UI v2.0
- **Sample Browser** (left panel)
  - List of loaded samples
  - Preview playback
  - Drag & drop WAV import
  
- **Waveform Display** (top center)
  - Visual feedback of current sample
  - Loop points (if implemented)
  - Zoom controls
  
- **Filter Section** (center)
  - Cutoff frequency knob
  - Resonance knob
  - Filter type selector (LP/HP/BP)
  
- **Effects Section** (right)
  - Reverb send knob
  - Saturation/distortion knob
  - Bit crusher (optional)
  
- **Pitch Section** (bottom left)
  - Pitch envelope amount
  - Pitch glide time
  - Pitch bend range
  
- **Output Section** (bottom right)
  - Master gain with VU meter
  - Peak indicator
  - Limiter on/off

## Trap Aesthetic Design Guidelines

### Color Palette
```
Primary:   #FF6432  (Orange-red) - Knobs, highlights
Secondary: #1E1E23  (Dark gray)  - Background
Accent:    #00FFF5  (Cyan)       - Active states
Text:      #C8C8C8  (Light gray) - Labels
Inactive:  #505055  (Med gray)   - Disabled controls
```

### Typography
- **Title**: Bold, 14pt, Sans-serif
- **Labels**: Regular, 9pt, Sans-serif
- **Values**: Monospace, 8pt (for numerical displays)

### Visual Style
- Flat design (no gradients or shadows)
- High contrast for visibility
- Clean, minimalist interface
- Sharp corners (no rounding)
- Tight spacing for compact layout

## Interaction Design

### Knob Behavior
1. **Hover**: Slight brightness increase
2. **Active**: Border highlight
3. **Automated**: Pulsing outline
4. **Double-click**: Reset to default
5. **Shift+drag**: Fine adjustment (10x precision)
6. **Ctrl+click**: Enter numeric value

### Visual Feedback
- Parameter changes show in FL Studio hint bar
- Knob position visually updates in real-time
- No modals or popups (non-blocking UI)

## Accessibility
- High contrast ratios (WCAG AA compliant)
- Large enough controls (40px minimum)
- Clear labels on all controls
- Keyboard shortcuts (future)
- Screen reader support (future)

## Technical Implementation

### VSTGUI Components Used
```cpp
CFrame          // Main window container
CKnob           // Rotary knobs (x5)
CTextLabel      // Parameter labels (x5)
// Future:
CBitmap         // Background graphics
CWaveView       // Waveform display
COptionMenu     // Sample selector
CVuMeter        // Level meter
```

### Rendering Performance
- **Frame Rate**: 60 FPS (idle)
- **Draw Calls**: Minimal (static background, dirty-rect updates)
- **CPU Usage**: <1% on modern systems
- **GPU**: Software rendering (no GPU required)

## Platform Considerations

### Windows
- Native Win32 window handle
- DPI awareness (100%, 125%, 150%, 200%)
- Windows 10/11 theming compatible

### Future: macOS
- Would require Cocoa integration
- Retina display support
- Dark mode compatibility

## Responsive Design
Current implementation is fixed size (280x100).

Future: Support for window scaling:
- 100% - 280x100 (current)
- 150% - 420x150
- 200% - 560x200

All controls scale proportionally.
