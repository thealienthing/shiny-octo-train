# UVU MCS Graduate Paper
### Candidate: Benjamin Stoneking
### Advisor: Dr. Frank Jones

# Abstract
The project focuses on implementing a real-time digital audio synthesizer from the ground up on a microcontroller. The aim is to leverage all the strengths of the microcontroller such as interrupt service routines, timers, and hardware interrupts to produce a system capable of acting as a reliable musical instrument for the user. The topic is interesting as an exercise in implementing an audio data processing pipeline that involves many signal processing components such as oscillators, pitch control, gain mixing and attenuation, envelope processing, and filtering to produce a variety of different sounds. The project represents building a complex system capable of being embedded in a limited processing space with options to extend its capabilities. The final project yielded a synthesizer meeting the fundamental requirements to be called a subtractive synthesizer: It can produce sound based on input from a midi instrument and can manipulate the signal gain to create an instrument capable of volume dynamics. It can process the audio through high and lowpass filters to change the timbre to sound similar to real-world instruments like guitars, pianos, and string instruments and it has a hardware interface consisting of knobs, digital encoders, and a screen allowing the user to directly manipulate the sounds in real-time. A computer software accessory application can communicate with the system to be able to adjust parameters and save and load user configurations for later use. Although the project meets the minimum requirements for being a synthesizer, it has much work to be done. Many lessons were learned: the process of prototyping and implementing hardware peripherals can be fraught with frustrating bugs and failures. Embedded low-level software has a much more involved and time-consuming process for debugging, with regression tests being an absolute necessity. A lot of the functionality of electronic musical instruments that we expect is a harder system to implement than expected. The lines of code for this system ended up around 2250 which is much lower than what is typically required of the project. Much of the reason for the smaller count of lines of code was because 1) Much time getting the system working required many if not tens of hours writing drivers to interface with hardware without the system regressing 2) More lines of code are not a good idea in an embedded system as it causes the firmware builds to grow and the microprocessors flash memory cannot contain all the instructions and 3) 50-100 hours was taken away from software development for prototyping hardware on a breadboard and then designing and implementing permanent hardware fixtures with many setbacks and revisions required.

Keywords: audio, synthesis, embedded, microcontroller, real-time, midi, hardware, filters, envelopes


Begin with a proper thesis statement, which may be several paragraphs. A proper thesis statement answers the
following 4 questions:
(1) What problem are you trying to solve?
(2) why is it interesting?
(3) What are your major results?
(4) What are your conclusions?

# Introduction

The problem I wanted to solve was rather simple: The acquisition of musical gear is a common obsession that plagues millions of musicians around the world. The cost of gear for performance or music production quickly drains the wallets of musicians who already struggle to keep their bank accounts at a non-zero value and for many musicians, this thought crosses their mind at least once: "Can I just make my own X and save some money?". If you ask anyone who has gone down the road of making their own digital software and music hardware, they will tell you the answer is "Yes you can just make your own X. No you won't save any money. You will spend more". Over the years, I've researched this very topic and found this conclusion to be true but with a small caveat: You _can_ in fact save money by making your audio effect of the electronic instrument as a digital system rather than an electrical circuit of discreet analog components. This caveat has a caveat of its own: if time is money, then every hour spent in implementing your system is implicitly adding to the price tag of your system.

I've known these facts for a long time and have seen them in action as my education has had me walk through the process of making yet another database management system, yet another virtual machine. Unsurprisingly, my database hasn't replaced MySQL and my virtual machine hasn't replaced the JVM. Still, so much knowledge and understanding of how computers work was gained from these experiences. I applied this same principle to my experience as a musician with the goal of my master's project: Make yet another digital synthesizer on an embedded microcontroller. The purpose is to learn how to implement a complex real-time embedded system, develop a digital audio pipeline, make a hardware interface to control it and indulge my fantasy of making a piece of musical gear that is wholly mine. To maximize the learning potential of this project and focus on the process rather than the final destination, I decided to set a few restrictions:

1. The firmware must be implemented in a low-level language such as C/C++.
2. I can only use the standard C/C++ libraries and a Hardware Abstraction Layer library to minimize time wasted fussing with getting hardware peripherals to communicate with the microcontroller.
3. No libraries mean no DSP libraries. Every bit of audio data music is generated by instructions handcrafted by me. Although I can reference other digital audio codebases, I avoided it as much as possible to allow myself a chance to solve the problem on my own for better or for worse.


The resultant product of my project turned out to be satisfactory but fell miles short of where I hoped it would be. I believe my project currently is in a place where it can be considered something that meets the basic requirements to be called a subtractive synthesizer. It's relatively stable, but with a few tweaks stands to be a studio or stage performance-ready instrument for musicians.

# Concepts

## A quick word on audio synthesis and features required
Audio synthesis is a rather broad topic and the project goal of "make a synthesizer" is very vague. There are so many styles and methods of audio synthesis. Since this is my first jump into the world of real-time data processing, I decided to make my synthesizer as classic as possible. The system I designed is a __subtractive__ synthesizer: fundamental waveforms are combined and then passed through a series of filters to strip away certain frequencies to create new sounds. This method is a relatively primitive yet classic means mimicking real-world instruments and dates back nearly a century. Subtractive synthesis was made especially popular in the 60s and 70's for its ability to bring new and imaginative sounds to rock and jazz music. 

To make a digital subtractive synthesizer that meets the bare minimum requirements, I needed to implement the following features for my microcontroller:
1. Oscillators - produces an audio signal by repeating some pattern such as a sine wave at a set frequency
2. MIDI - __Music Instrument Data Interface__ or MIDI is the standard way that musicians communicate with electronic instruments and software. A common electronic piano keyboard usually has a MIDI out jack on it allowing the user to control other devices using the keys, knobs, and buttons on their keyboard. My system needs to be able to receive generic MIDI messages commanding it to turn notes on and off in real-time
3. In addition to MIDI capability, the system must have 8 voices, meaning it can play up to 8 notes at once. 8 is a rather arbitrary number but is generally an acceptable amount for most keyboard players. 
4. Filters - After oscillators, filters are the most critical component in a subtractive synthesizer as it allows the user to shape and mold their basic fundamental frequencies into new sounds.
5. Envelopes - After Oscillators and Filters, envelopes represent a critical component of a synthesizer. Envelopes are gates that may be triggered by arbitrary signal sources such as a key press on the keyboard and over time, the envelope gate will open and close over time. Most commonly, an envelope is applied to the output gain of a synthesizer allowing control of the volume which adds a great deal of strength to the ability of the synthesizer to mimic other instruments.

# Architecture

A big decision in implementing this project was choosing the right microcontroller platform. I considered several options but settled on a popular and relatively new audio development ecosystem by Electro-Smith known as Daisy. Daisy is an Arm Cortex-M7 based development board that provides all the necessary processing power and peripherals necessary for audio processing and then some. The Cortex M7 clocks at 480Mz peripheral support for an extremely high-resolution digital-to-analog converter (DAC). In addition to the DAC, it has support for expected devices such as I2C, UART, USB, Timers, etc. The biggest plus (at the time) to choosing Daisy as my platform was Electro-Smith's libDaisy library which acts as an abstraction on top of the standard HAL libraries for the Cortex M7. This was an attractive option as it would allow me to set up hardware quickly and allow me to focus primarily on the audio and signal processing aspects of the project. 

<img src="images/daisy_promo_pic.png" alt="Daisy" height=150 title=Daisy>

They also provide a DSP library which I chose to not use as an established requirement was not to use a DSP library and instead do all my audio processing from scratch. As I mentioned before, Daisy seemed to be a great choice at the time and freed up time in the initial hardware setup. What I was not aware of was the relative newness of the platform. Daisy originally was a Kickstarter-funded project which receive incredible amounts of success and support from customers. The platform was designed with musicians in mind, and they put a lot of work into supporting much more user-friendly development languages and frameworks like MaxMSP, Puredata, and Arduino to assist new developers in making their projects and forgetting about all the gory details. Because of this additional layer of abstraction, it meant that a lot of time put into customer accessibility, and full support for utilizing all hardware to its fullest capabilities was put off and is still in current development. This had the following consequences for my project:

Some HAL library functions did not work as they were supposed to. 
    a. One of the biggest disappointments was a lack of support for two-way USB communication outside of the bootloader. When I was implementing my serial interfacing GUI application for remote control of my project, I discovered that the USB software support extended to either a dedicated interface for the device to either receive or transmit data over USB but not both. My choices were to either hack a software solution involving handing off control between the host computer and device, adding the two USB communication support to the libDaisy library (something that I did not have time or sufficient knowledge of USB to tackle) or simply add a USB-UART converter chip and connect it to some UART pins on the devices. I opted for the last option as I was severely lacking in time.
    b. Another severely lacking feature was support for hardware interrupts. Some of the HAL libraries provided support for hardware interrupts for USB receive (this felt insulting as I could not talk back on the USB line). But also, there was no support for UART and GPIO interrupt service routines (ISR). This is the reason why MIDI data events are handled via periodic polling during timer ISRs. I believe this polling method yielded imperfect MIDI data consumption.
    c. A last gripe I have with the platform is the datasheet claiming the availability of certain I2C and UART lines on the board's pins, but they are not completely wired up in the MCU's current revision. This was not documented and instead made known to me via community forums. This lack of complete availability of hardware made adding all my peripherals rather difficult because of pin conflicts.
This whole experience soured me a bit about the Daisy platform in general. While I admire the ambition of the project to support programming in a variety of languages that opens up audio development to new developers, I feel it would have been best to fully flesh out all hardware support in basic C before moving on to supporting things like Object Oriented Programming, visual programming languages, and Arduino. 

My original vision for the project was, as the master's capstone prompt recommends, **very ambitious**. My vision was to implement a near clone of my first synthesizer, the Korg microKorg, but as a headless unit that could be used by plugging in any standard MIDI keyboard (In my original design drawing, you can even see an array of buttons that represented a very limited keyboard which would allow the user to play notes without connecting an external keyboard. I truly was a bright-eyed summer child when I first envisioned this project).

I'll start with the explanation of my design first with the things that were complete, and then address the absent hardware and software features.

## Hardware

This section covers the hardware that was successfully integrated into the project

### MIDI
There is a 5-pin MIDI input circuit that allows the user to connect a standard MIDI cable between the MIDI out of a standard keyboard and my synthesizer. This functionality is plug-and-play. By plugging in the MIDI cable and powering on the device, you can immediately start playing notes and making music which you can hear by plugging in headphones or a speaker into the 1/8" audio jack. 

<div style="text-align:center">
    <p>Midi Optocoupler Circuit</p>
    <img src="images/midi_optocoupler_circuit.png" alt="Daisy" height=300 title=Optocoupler>
</div>

The MIDI circuit implements a standard of device safety for receiving incoming signals. An optocoupler receives signals and converts the incoming electrical signal from the MIDI line into an optical signal which can be read and converted to an electrical signal that is safe for the device. This safety precaution is meant to handle cases where the connected keyboard is malfunctioning or outputting an electrical signal that is too high voltage for the pins on the MCU. It also acts as a much easier-to-replace component in the circuit. If the MIDI device is outputting voltage dangerously outside the expected voltage range, the optocoupler will fry and can be easily replaced instead of frying the MCU. 

### Analog Potentiometers and Screen

For adjusting parameters, I took a design philosophy of the Korg microKorg and set up an array of 5 analog potentiometers that had contextual control over various synth parameters which were determined by the internal state of the synthesizer. The internal state of the synthesizer is communicated to the user with a 20x4 character LCD screen that shows the user the current mutable context of the system. 

<div style="text-align:center">
    <p>Photo Of Screen Main Menu</p>
    <img src="images/placeholder_duck.png" alt="Daisy" height=100 title=Duck>
</div>

For example, if the menu is displaying the oscillator control context, you can use the potentiometers to change the fundamental waveforms generated by the oscillators and set them as you like to a Sin, Triangle, Square, Sawtooth, or Noise generator. If the menu was instead displaying the Amp Envelope control context, you can use those same knobs to adjust the Attack, Decay, Sustain or Release parameters of the envelope. The way the menu context is controlled is via a digital rotary encoder with a built-in push button. Turning the knob will take the user out of their current control context into a universal menu of contexts that the user may control. They can turn the knob to highlight the component they wish to control (Filter, Envelope, etc) and engage the encoder push-button to transition to the control context of the highlighted component. The potentiometers would be mapped to adjust the fields within that context and perform specific interpolations of the components. Since some parameters are better controlled logarithmically, the integer value read from the knob can be converted to a float and scaled according to its required range. Some parameters only represent a few discrete values. The large values of the 16-bit numbers would instead be mapped to the smaller ranges required to represent the discrete values of the menu parameter. 

<div style="text-align:center">
    <p>Photos of changing params</p>
    <img src="images/placeholder_duck.png" alt="Daisy" height=100 title=Duck>
</div>

### Power

The 20x4 LCD screen and the optocoupler on the MIDI input circuit require 5v, which Daisy does not put out (another frustrating aspect of the device since there is 5v power available on the USB jack of the Daisy); so I added a standard breadboard power supply to the circuit accepts a barrel jack input with voltages between 5v and 12v. This voltage is fed into the VIN pin on Daisy and is used to power the entire circuit and all its peripherals. A 1/8" inch stereo audio jack is connected to the left and right DAC pins on the MCU and outputs a signal at line level gain out of the box without the need for an amplifier (a very nice feature). Finally, there is a USB to UART converter breakout board connected to a set of RX/TX UART pins on the MCU which allows two-way communication between a computer and the synthesizer via a remote control GUI a wrote in Python.

### Audio DAC

Daisy has a built-in high-resolution digital-to-analog converter capable of outputting audio samples at rates up to 96khz and 24-bit depth which goes well beyond the industry standard quality of 44.1khz and 16-bit depth. There are four DAC channels: two for stereo input and two for stereo output. In this project, I only use the stereo output as processing audio input is beyond the scope of this project.

### UART, I2C, and 16-bit input ADC

General purpose pins were configured to set up UART, I2C, and ADC communications. Two separate UART lines were used for setting up MIDI input and plain serial communication for controlling the system remotely. The ADCs were used for reading the potentiometers. Some GPIO pins were used to read the digital rotary encoder for menu navigation.

<div style="text-align:center">
    <p>Diagram showing wiring of peripherals</p>
    <img src="images/placeholder_duck.png" alt="Daisy" height=100 title=Duck>
</div>

## Software

This section goes into depth about the various software components that make up the actual audio synthesis and processing. I will address the software components in the order that they were implemented throughout the project, as it will also help to highlight the process of generating audio from the initial press of the eventual output of the audio to a speaker. I will cover the topics of Oscillators, Voices, Envelopes, and Filters completely as these comprise the critical components of the actual synthesis aspects of the project. I will only very briefly discuss other features in a few sentences as they have less to do with audio synthesis and more to do with overall system cohesion and functionality.

### Oscillators

The most fundamental part of any subtractive synthesizer is the oscillator. Generally speaking, an oscillator is simply a function generator. The oscillator will maintain a variable representing the phase of the function cycle. The sequence chart below illustrates how audio samples are generated from the oscillator and then placed into the output buffer of the DAC when it triggers an ISR.

<img src="images/oscillator_sequence_diagram.png" alt="Daisy" width=400 title=Daisy>

Note: The diagram above will generate a 1hz sin wave which isn't audible and we'd like to hear our music if possible. So the oscillator must maintain a variable containing the desired frequency and calculate our phase delta so we can generate the function at the desired frequency. **Phase delta** is a value by which we increment the phase to get the next sample of our waveform. To generate a wave at 1hz with a sample rate of 44.1kz, we simply need to divide two pi by 44100. Starting with our phase at zero, if we add our phase_delta to phase 44,100 times, we will have a value of two pi. Two change the pitch, we just need to multiply the phase delta by our desired frequency. If our pitch is set to 100hz, our calculated phase delta will allow our oscillator function to cycle 100 times in a second.

### What about other waveforms? Sin waves are boring.

This is true. Sin waves generally lack harmonic complexity and will generally produce very mellow tones. The oscillator class also maintains an enum describing what *kind* of oscillator it is. The oscillator waveform can be set with the set_waveform method. Next time get_sample is called, the corresponding waveform function will be called returning a sample from your fancy new harmonically rich waveform. For more information about these functions, the snippet below is a good start. Also, see Oscillator.cpp for the full source.

```c++
//Oscillator.h
enum WaveForm {
    Sin,
    Tri,
    Saw,
    Square,
    WhiteNoise,
    WaveFormEnd
};

//Oscillator.cpp
void Oscillator::set_waveform(WaveForm waveform)
{
    _waveform = waveform;
}

float Oscillator::get_sample()
{
    //Some context of this function removed for brevity
    switch (_waveform)
    {
        case WaveForm::Sin: {
            sample = sinf(_phase);
            break;
        }
        case WaveForm::Tri: {
            t   = -1.0f + (2.0f * _phase * TWO_PI_RECIP);
            sample = 2.0f * (fabsf(t) - 0.5f);
            break;
        }
        case WaveForm::Saw: {
            sample = ((_phase * TWO_PI_RECIP * 2.0f) * -1.0f) * -1.0f;
            break;
        }
        case WaveForm::Square: {
            sample = (_phase < _half_cycle) ? 1.0f: -1.0f;
            break;
        }
        case WaveForm::WhiteNoise: {
            sample = noise.process();
            break;
        }
        default: {
            sample = 0.0;
            break;
        }
    }
    //More context removed here for brevity
    return sample;
}
```

When the basic oscillator was set up and I was able to statically compile and upload code to make my little daisy output a continuous 440hz sin wave, I felt a rush of excitement. It was incredibly gratifying to **hear** my code working instead of just seeing it in a terminal or GUI. I played around, writing little loops and time delays to change the pitch and hear the pitch change in real-time and play scales. After that, I figured out the functions to generate the square function: a simple on/off function that sets the sample to 1.0 during the first half of the phase and the flipping it to -1.0 during the second half. Writing sawtooth and white noise functions was easy. I never was satisfied with the triangle waveform. It is supposed to straddle the line between a smooth sin wave and a choppy sawtooth in terms of complexity but it always sounded more like a slightly droopy sawtooth.

After the oscillators were properly implemented, I added the MIDI message processing through the MIDI UART connection. As MIDI messages came through the data line, the pitch data was parsed and converted into the note frequency in hertz and the pitch was set on the oscillator. If a MIDI event message came through indicating that the note was released on the keyboard, the pitch of the oscillator was set to 0hz to silence the oscillator. At this point, I had the bare minimum definition of a synthesizer. The user could play notes on the keyboard, and the configured oscillator waveform playing the corresponding frequency would play out the speakers. But at this point, I could only play one note at a time and could not generate any signals more complex than the 5 basic oscillator functions. Adding polyphony and oscillator mixing was the next feature to add.

## Voices

In the world of synthesizers, a single discrete note playing a certain pitch is called a voice. When your synth is monophonic, it means it only has one voice and can therefore only play one pitch at a time. Most modern synthesizers, whether analog or digital, have polyphonic capabilities. Many analog synthesizers can play four to eight notes at once because adding more voices requires more discrete hardware oscillators and can make the system more complex, expensive, and heavy. Digital synthesizers can accommodate more notes quite easily and will often support at least eight voices with options to play as many as 128 (the maximum number of discrete notes available in the MIDI protocol) which is overkill. I did a little bit of playing on a synthesizer of my own and I paid attention to how many notes I needed at my most advanced and complex playing. I decided that eight would suffice with the option to add more if needed.

I wrote a class intended to capture the functionality of a voice. The class would contain two separate oscillators that can be set to any fundamental waveform. It would also contain separate gain parameters for attenuating or amplifying each oscillator. At the end of the processing chain for the voice, the attenuated oscillators would be combined into one sample and passed to the class's output. Also included in the class is the option to configure a pitch offset to the second oscillator; another common synth feature in synthesizers. By detuning one oscillator from another, you can generate even more complex waveforms with frequency beating, added intensity, or even complete dissonance if desired.

<img src="images/voice_class_diagram.png" alt="Daisy" width=400 title=Daisy>

Once this class was implemented, I wrote a larger class called Synthesizer that would own a group of voice instances. As keys on the keyboard were pressed and released, the synthesizer would take a voice that didn't have a pitch set to 0hz and set the pitch to that note. Some optimization was made to ensure that if a voice wasn't in use, the synth class wouldn't bother telling the voice to generate a sample and waste time calculating the outputs of the oscillator functions. Implementing this opens a whole can of worms. What data structure should be used to hold these voices? There are many considerations to be made but first I just focused on making it work. For this project, I decided to avoid any C++ features other than simple classes and subclassing, as there would be performance and memory usage overhead prices to pay that would add up over time. I chose to use a primitive array. My approach was relatively simple. As a new note was played, I would iterate through the voices until I found a voice without a pitch set to 0, and set its pitch to the new note. When note-off events come in, iterate over the array until the voice with a pitch matching the note-off event is found, and set it to 0 to make it inactive. That was pretty easy. This was even quick to process. Most of the time you wouldn't have to iterate close to the end of the array.

The real rub was to handle when all voices were active and more note-on events occurred. Should the synth just ignore that note, or should it drop some other note and replace it with the newest one? Most synths will change the pitch of the oldest voice in the sequence to be dropped from the note combination and use that voice to play the new incoming note. I decided to stay true to convention and implement this policy. So how do I indicate the oldest note in the sequence? My first approach to this was honestly a poor choice. It involved a rather clunky policy of making sure the voices in the array were sorted according to the order in which the pitches were played on the keyboard. Every time a note-off event occurred, I would have to shuffle around the voices to make them preserve the order in which they occurred. In hindsight, this approach was so bad, it's mind-boggling. I never experienced any breakdown in the audio output because of the time spent sorting, but eventually, cracks started to appear when many notes were being played and released in quick succession. Sometimes, the sorting could take long enough that midi events would miss being consumed or some bug in my logic resulted in not off events not being registered; and that voice would play forever until I found the note that was stuck and then played the key again so that the note off event would clear the voice.

In the end, I reverted my voice organization to simply iterating through the array to turn voices on and off and just ignore events when too many notes were being played. The contrived sorting strategy was dropped and the system instantly became more stable. As I've reflected on this experience, I suspect that the best approach to handle dropping moments when too many notes are being played is to implement a doubly linked list to make it easier to maintain a sorted list that can move arbitrary voices to the back of list when they go inactivate without needing to sort.

At this point in the system, my software architecture looked like this:

<img src="images/voice_graph_diagram.png" alt="Daisy" width=400 title=Daisy>

## Envelopes

The next step in fleshing out the synth was to add some dynamic control of the volume voices. Different instruments get louder and quieter over time in different ways. A percussive instrument like a drum or piano will instantly play at maximum volume and the loudness will diminish over time. A bell when struck will right out for a long time until the user mutes the bell. A snare drum will go quiet almost immediately after it reached its maximum volume. Conversely, a violin will ramp up its volume a bit slower and can play at the same volume indefinitely. Synthesizers can have this kind of dynamic control through a component called an envelope. Envelopes are a signal that can be used to modulate a signal over time and often have four phases in which the signal gain increases and decreases. These phases are known as Attack, Decay, Sustain, and Release.

### Attack
The phase where the signal starts at 0 and increases to its maximum gain. The attack phase indicates how **how long** it takes for the gain to reach max.

### Decay
This phase occurs immediately after the attack phase where the signal gain decreases over time until it reaches **sustain** gain. Like attack, the decay phase indicates **how long** it takes for the gain to decrease sustain.

### Sustain
This phase occurs immediately after the decay phase where the signal will remain at its configure sustain gain. Unlike attack and sustain, sustain does not relate to time, but simply the gain that the signal will remain at after the gain has hit its max. As long as the note is held down on the keyboard, the envelope will remain in the sustain phase indefinitely.

### Release
The final phase of the envelope, the release phase occurs immediately after the key is released on the keyboard. The envelope can enter the release phase from all of the previously mentioned phases. Release refers to **how long** it takes for the envelope gain to return to 0 after the key is released.

<img src="images/amp_envelope_visual.png" alt="Amp Env" height=400 title="Amp Env Visualized">

Envelopes can be used generically to modulate any kind of signal such as volume, filter cutoff frequency, or gain on an audio effect like reverb or distortion. The application of envelopes is limited to your imagination. The application of an envelope to volume is known as an Amp Envelope and is the most common. To make the synthesizer capable of emulating the volume dynamics of many instruments, the next step was to add an Amp Envelope. In a polyphonic synthesizer, every voice (see previous section) must have its own Amp Envelope so that each note can grow and diminish in volume as real-life instruments do.



A new class called Envelope was implemented with a state machine emulating the phases of the Envelope:

```c++
//Envelope.h
class Envelope {
public:
    //Enum for tracking what phase the envelope is in
    enum Phase {
        ATTACK,
        DECAY,
        SUSTAIN,
        RELEASE,
        READY //This became the new way of knowing if a voice was available for use
    };

    float val = 0.0; //the current signal gain of the envelope
    Phase phase = Phase::READY;
    float process(); //called periodically by timer event callback to advance the gain according to envelope phase state
    void note_on();
    void note_off();
    void reset();
    //Some setters and getters for accessing the private variables not included in this snippet
    
private:
    uint16_t _attack_ms, _decay_ms, _release_ms;
    float _sustain;
};
```

The voice class was refactored to have its own instance of the Envelope class so each voice grows in volume independently depending on when the voice becomes active. Every time the audio callback happens and the synth calls get_sample on a voice, the voice will return the product of its generated audio sample and the gain of the envelope.

```c++
//Envelope.cpp
float Voice::get_sample()
{   
    //each oscillator has its own gain for mixing the two waveforms
    //the sample outputs would be divided by the number of playing voices to prevent blowing out speakers and deafening user
    float sample1 = (_osc1.get_sample() * _osc1_amp)/NUM_VOICES; 
    float sample2 = (_osc2.get_sample() * _osc2_amp)/NUM_VOICES;
    return (sample1 + sample2) * amp_env.val;
}
```

Because the envelope does not produce any sound, the process method does not need to be called during the audio callback. The hardware timer callback would call the process method and the envelope would change its gain according to its current phase. The configuration of the timer is used to make sure the various time-based phases affect the envelope gain correctly according to time (Ex. If attack is set to 100 milliseconds, the gain should increase by .01 every millisecond)

| Envelope States | Explanation |
| ------ | ------ |
| Ready | Indicates to the synth owning the voice that it's available to play a note |
| Attack | Gain increases up to 1.0 according to attack rate |
| Decay | Gain decreases to sustain level according to decay rate |
| Sustain | Gain remains at a set level according to sustain |
| Release | Gain decreases to 0.0 according to release |

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

## Filters

The last of the core functions to be implemented in the synth was signal filtering. Up until this point, the control of the harmonic qualities of the signals was fairly limited. You could combine different fundamental waveforms and alter pitch to introduce *some* complexity, but the subtractive aspect of this subtractive synthesizer was conspicuously missing. Filters make up the subtractive nature of the system and allow the user to selectively chop frequency bands from the final signal to emulate real-life instruments. An audio filter typically presents two main parameters: Frequency cutoff and resonance. Cutoff determines what frequencies in the spectrum will be filtered out. Resonance is a gain boost that can be applied around the cutoff frequency that will accentuate a certain range of frequencies in the spectrum.

The most common filters used in subtractive synthesis are low pass, high pass, and band pass filters. Applying a low pass filter to a harmonically rich signal has the effect of removing the harsher upper harmonic frequencies and making the signal more mellow and deep. This type of filter is effective in producing sounds like a bass guitar, bass drum, or cello. Applying a high pass filter will cut out the low frequencies in the signal. This is useful for creating a lead instrument intended to cut through the spectrum without sounding muddy from the lower range of frequencies. Usually, a high pass filter will be used to produce sounds like a flute, violin, or snare drum. A band pass filter is a combination of low and high pass filters, where a small frequency band somewhere in between is desired.

I wrote a generic filter class that could be subclassed into the different types of filters. 

```c++
//Filter.h
class Filter {
public:
    Filter() {};
    Filter(float sampleRate, float cutoffFrequency, float q);
    virtual float process(float input) = 0;
    void set_cutoff(uint32_t freq_hz);
    void set_resonance(uint32_t gain);
    virtual void update_coefs() = 0;

    float cutoffFreq;
    float resonance;
    
protected:
    float sampleRate;
    float b0, b1, b2, a0, a1, a2;
    float x1, x2, y1, y2;
};

//Filter.cpp
//Subclasses override process and update_coefs functions according to their functions
class LowPassFilter : public Filter;
class HighPassFilter : public Filter;
```

This part of the synthesizer was the biggest unknown to me at the time of implementation as I needed to learn more about digital signal processing to understand the math. I did a little research and came across the concept of filtering audio signals using the difference equation. Since my system works by generating audio one sample at a time, it made sense use to a filter that operates on discrete time samples. I was able to grasp the concept with a little extra effort. The process of designing a filter using the difference equation is as follows:
1. Based on your desired filter, you must choose a method of generating appropriate filter coefficients. Some methods include Bilinear Transform, Chebyshev, Finite Impulse Response (FIR), Butterworth, etc.
2. Using your chosen filter method, generate coefficients to be used in the difference equation. You may generate an arbitrary amount of coefficients but it will not necessarily make your filter **better**. It will make it more computationally costly and therefore you must generate several coefficients appropriate to the filter design you choose to implement.
3. Implement the difference equation to operate on your samples with your generated coefficients.

After some research, I decided to use the bilinear transform to generate my filter coefficients for my low-pass and high-pass filters. The filter coefficients are generated by performing the bilinear transform on the cutoff frequency divided by the sample rate of the system. The alpha portion of the filter represents how steep the attenuation of the signal is around the cutoff frequency of the filter. Resonance plays a part in the alpha coefficients as the resonance will factor into how much gain is applied around the cutoff frequency of the filter.

Following is my code implementation of the low and high pass filters in my project with a sequence diagram to help visualize the flow of data through the filter and how previously generated values are stored for use on the next filter cycle.

```c++
//Filter.cpp
void LowPassFilter::update_coefs() {
    float w0 = 2.0f * M_PI * cutoffFreq / sampleRate;
    float cosw0 = std::cos(w0);
    float alpha = std::sin(w0) / (2.0f * resonance);

    b0 = (1.0f - cosw0) / 2.0f;
    b1 = 1.0f - cosw0;
    b2 = (1.0f - cosw0) / 2.0f;
    a0 = 1.0f + alpha;
    a1 = -2.0f * cosw0;
    a2 = 1.0f - alpha;
}

void HighPassFilter::update_coefs() {
    float w0 = 2.0f * M_PI * cutoffFreq / sampleRate;
    float alpha = sin(w0) / (2.0f * resonance);

    b0 = (1.0f + cos(w0)) / 2.0f;
    b1 = -(1.0f + cos(w0));
    b2 = (1.0f + cos(w0)) / 2.0f;
    a0 = 1.0f + alpha;
    a1 = -2.0f * cos(w0);
    a2 = 1.0f - alpha;
}

//This snippet of the second-order difference equation shows how the output is generated based on previous inputs. The input and output at the time of process are preserved and saved to x1 and y1 and the input/output values from the previous process cycle are saved to x2 and y2.
float LowPassFilter::process(float input) {
    float output = b0 / a0 * input + b1 / a0 * x1 + b2 / a0 * x2
                     - a1 / a0 * y1 - a2 / a0 * y2;

    x2 = x1;
    x1 = input;
    y2 = y1;
    y1 = output;

    return output;
}
```
<img src="images/filter_sequence_diagram.png" alt="Filter Sequence Diagram" width=400 title="Filter Sequence Diagram">

Adding filters to the project was a difficult concept to grasp. Because of my lack of previous experience, some concepts had to be taken with a bit of faith and proven by seeing them in action. This first naive attempt at filtering resulted in some stable low and high pass filters and some gained confidence. I believe now I can move forward and be able to do some deeper research into filters and implement more complex and capable filtering in the future.

## Architecture High Level

I only covered a fraction of the overall architecture of the system. The project can be represented at a high level with 

## The little big software features

These are features that I would say are overlooked when they work, but utterly infuriating when they're broken. Some features were so important that they couldn't be ignored but sucked away a critical amount of time from the actual audio processing.

**The LCD Screen**
I set up the I2C line and wrote a driver to interface that allowed me to quickly print menu information to the screen. This was a tediously long process with a lot of trial and error and sucked up about 2-3 weeks of my time. But it works very well and was critical in implementing the menu system.

**The Hardware Timer**
I set up a hardware timer that triggered an interrupt service routine for reading peripherals. This effectively is a polling mechanism because of the lack of support for GPIO interrupts in libDaisy. This timer callback would read the analog potentiometers, the rotary encoder, and the MIDI UART line and respond to any inputs on these peripherals. This timer was also used to advance the state of the Amp Envelope (See envelope section for details).

**The Menu**
Running with the driver code for the LCD screen, I built up a class that will display the menu and its corresponding contexts. The Menu maintains a structure of configurable parameters for the synthesizer such ass oscillator waveforms and filters. The general purpose potentiometers adaptively manipulate menu parameters under the hood according to the currently selected menu context.

**Remote Control GUI Application**
A simple GUI control panel was written in Python using the QT framework to be able to remotely change parameters without the need to physically interact with the synthesizer beyond simply playing keys on the keyboard to generate sound. This was accomplished using a secondary USB cable to communicate via USB Serial to tell the synth what parameters to change. This GUI also allows the user to save custom patches to a file on the host PC's disk, which can be loaded at will to instantly reconfigure the system. This feature is still in development but is mostly complete with the ability to change a few parameters. The USB Serial driver provided by Daisy is susceptible to crashing when too much data is sent down the line too quickly.

# Project Results

Overall the project was a mixed bag of successes and ongoing struggles to implement. 

## Successes and completed features

I accomplished to core requirement of my project. Implement a real-time subtractive synthesizer from scratch on an embedded device. My system generates tones based on user input from a generic MIDI keyboard. The tones are combined and processed through an amp envelope to add dynamics. The output from the envelope is processed through filters to carve frequencies out of the audio signal. The final processed audio signal is output to speakers or headphones which can be heard by the user in real-time without interrupts like pops or clicks in the audio. 

## Failures

I did not manage to implement some of my desired audio components into the system. I did not have time to add low-frequency oscillators (LFO) which could be used for general-purpose signal modulation for things like pitch and filter cutoff. I did not have time to add support for saving patch parameters to flash on the device itself. Bugs were encountered in my chosen library that made this too difficult to accomplish in my timeline. Many setbacks were encountered while adding hardware support that cost valuable time. It took several attempts with weeks of effort to create a handmade circuit board to contain all my hardware components. My first attempt at laying all the electrical components on a circuit board was based on making flying wire harnesses that attach to all the human input peripherals. This turned out to be an absolute mess and had to be scrapped. My thinking at the time was that making flying harnesses would make it easier to put the whole system into an enclosure. What resulted was a noisy mess of wires that made me feel physically ill. In the process of adding an external power supply capable of providing power to the MIDI input circuit and LCD screen, I accidentally wired my power input wrong to my microcontroller and I fried the device, releasing the magic blue smoke. I had to move forward with development and adapt to not having a microcontroller while I waited for a new one to be developed.

## Planned but not completed

I had several features I still wanted to implement in the system. I hoped to add some general purpose Envelopes to the system that the user could route to various components to be used for modulating arbitrary signals like filter cutoff, audio effect gain, oscillator pitch, etc. I planned to implement another common and important component in synthesis: the Low-Frequency Oscillator (LFO). LFOs are non-audible oscillators that are used for arbitrary signal modulation similar to the general-purpose envelope mentioned previously. You can take the generated signal of the LFO and plug it into things like the pitch of an oscillator to add pitch vibrato. You can also apply it to filter cutoff to create filter "swishing" to the signal or apply it to the gain of the left and right audio outputs to create a panning effect between the left and right channels. These two features would not have been much effort to add and I still intend to implement them. Finally, I wanted to add some final audio effects that could process signals at the end of the audio pipeline like reverb, delay, overdrive, and phaser. These final effects (fx) can add spatial features and additional audio coloring and complexity that users of a synth enjoy playing with most.

## Hindsight: Choices were made
1. The Daisy Platform: Choosing to base my master's project on the promising platform of Daisy had serious consequences on development. Daisy was a very attractive option that promised a system that would make the hardware aspects get out of the way and make it easier to write audio processing software. The truth is, this is a new technology that has most things implemented to make it easier for hobbyists to get coding and make their dream system. I would have been better off choosing a more mature audio-centric board from ST Microelectronics and using the tried and true STM32Cube IDE to set up my hardware.
2. Hardware Implementation: Too much time was spent trying to migrate my project off of a breadboard and onto a permanent circuit board. I thought I knew enough of what I was doing that I could make a circuit board that could be the final resting place of my project until I was finished. I tried to set up a prototype with flying harnesses for all the peripherals to make it easy to embed in an enclosure box. What resulted was a noisy mess of wires that made me physically ill to look at. After all that effort, I had to toss that prototype in the garbage. I would have been better off just soldering my components to some perf board and doing some uncomplicated wiring of all my data buses directly to the socket pins of my microcontroller.
3. Spaghetti Code and Tight Coupling to the Microcontroller: As the system rapidly developed, some holes in previously developed features became more obvious with time which had to be ignored in favor of moving forward with development to meet the deadline. At times, refactoring software would create regressions that would cost even more time to fix. Software refactoring and reorganization would help to make it faster and easier to add additional functionality to the system. Looking back, I wish that I had properly planned out making my audio components modular enough that I could easily test them out without the microcontroller and the synth system as a whole. If I had made every component independent enough on the system, I could have been writing most of my audio processing code to work outside independent of the hardware and test them by making them perform processing simple audio samples on my computer instead of having to write my code, compile, flash, test, fix and repeat. It would have even been a desirable option to make the synthesizer capable of compiling to the machine code of the host computer and running in a minimal setting to be able to test features without the hardware in the loop.
## Performance Issues and Bottlenecks

Overall, there doesn't seem to be much of a bottleneck in the audio pipeline within my project. The synth is never late to deliver audio samples to the output DAC, so there are never any audio crackles or pops in the final audio played out the speakers.

# Conclusion

In my eyes, this project was a success, but not nearly the success that I envisioned and hoped for. I spent hundreds of hours and countless late nights probing my code for software bugs, probing hardware pins with a multimeter and oscilloscope. Many late nights were spent in the company of a soldering iron and writing tests to check that my hardware was working. Several weeks were spent writing and testing drivers to interfaces with the LCD screens and flash memory storage. But on the other hand, many hours were spent with the exhilarating feeling of adding a new audio processing feature to my system. Sometimes these wins with adding features just came one after another. Then what followed would be a regression requiring hours of hard work to debug and refactor into the system.

In the end, though, I got the core of what I hoped for. I have a system that works. I can plug in a midi keyboard and play notes and hear them play out my headphones as I would expect that to sound. I can dive into the menu and adjust the envelope, filters, and waveforms and it works the same as commercial synthesizers that I have used over the years. I've learned what I wanted to learn and so much more. I thought I would be getting a great primer on how audio synthesis works, but in the end, I also got a fantastic hands-on lesson in implementing **any** complex software or embedded system. I've gained confidence that I could venture into other foreign domains of software development such as video processing, hardware driver development, etc and I could feasibly learn the ropes. In implementing this system, perhaps an entire four-year undergraduate degree was learned about the best practices in implementing a complex system from the ground up. The failures that happened along the way were so valuable in illustrating the importance of code modularity. These failures further compounded the necessity of not obsessing over adding that extra special features and instead focusing on making the system work while paving a path to improvement. Trying to do too much at once results in wasted time and having to do things over again the right way.

This retrospective is almost cathartic in the clarity it gives me on the past year of my efforts. Surprisingly, the next steps I'm inclined to do not involve finishing the project where it stands now. I want to preserve the code from this project and use it as a reference and redo this whole project from the ground up. I want to take my audio software components and rewrite them to be decoupled from the daisy platform so that they can be tested outside an embedded environment and even used in software applications. I would then reintegrate these components into a new implementation of my synthesizer but on a more mature and robust embedded platform like an STM32. I would then implement all the missing features from this project like the LFO, audio fx components, patch saving, etc to complete my final vision. The last step would be to fabricate my own printed circuit board professionally and put the whole thing in a box to show to the world.

I am so pleased by the results and wisdom gained from this project and I can't wait to see it through to the end.