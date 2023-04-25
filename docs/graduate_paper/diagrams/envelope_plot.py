import numpy as np
import matplotlib.pyplot as plt
import math

def adsr_envelope(attack_time, decay_time, sustain_level, release_time, duration, sample_rate):
    t = np.linspace(0, duration, int(duration * sample_rate))
    envelope = np.zeros_like(t)
    
    # Attack
    attack_samples = int(attack_time * sample_rate)
    if attack_samples > 0:
        envelope[:attack_samples] = np.linspace(0, 1, attack_samples)
    
    # Decay
    decay_samples = int(decay_time * sample_rate)
    if decay_samples > 0:
        decay_values = np.linspace(1, sustain_level, decay_samples)
        envelope[attack_samples:attack_samples+decay_samples] = decay_values
    
    # Sustain
    sustain_samples = int((duration - attack_time - decay_time - release_time) * sample_rate)
    if sustain_samples > 0:
        envelope[attack_samples+decay_samples:attack_samples+decay_samples+sustain_samples] = sustain_level
    
    # Release
    release_samples = int(release_time * sample_rate)
    if release_samples > 0:
        release_values = np.linspace(sustain_level, 0, release_samples)
        envelope[attack_samples+decay_samples+sustain_samples:attack_samples+decay_samples+sustain_samples+release_samples] = release_values
    
    return envelope

attack_time = 0.1
decay_time = 0.2
sustain_level = 0.6
release_time = 0.3
duration = 1.0
sample_rate = 44100

envelope = adsr_envelope(attack_time, decay_time, sustain_level, release_time, duration, sample_rate)

x_line = np.arange(len(envelope)) / sample_rate

plt.plot(x_line, envelope, label="Envelope")
plt.plot(x_line, np.sin(x_line*200)*envelope, label="Oscillator")

plt.xlabel('Time (s)')
plt.ylabel('Amplitude')
plt.title('Amp Envelope')
plt.legend()
plt.show()
