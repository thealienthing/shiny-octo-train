```mermaid
stateDiagram-v2
    Ready
    Attack
    Decay
    Sustain
    Release

    [*] --> Ready: Envelope instance created
    Ready --> Attack: Key pressed
    Attack --> Decay: Gain at max
    Decay --> Sustain: Gain at sustain level
    Sustain --> Release: Key released
    Release --> Ready: Gain at 0

Attack --> Release: Key released
Decay --> Release: Key release
```