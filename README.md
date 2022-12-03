Master Project Proposal Paper - ROUGH DRAFT WIP

---

# Masters Capstone - Real Time Audio Synthesizer

### MCS Graduate Project Proposal: Embedded Real-Time Audio Synthesizer
Benjamin Stoneking, Mentor: Dr. Frank Jones
Computer Science Department, Utah Valley University
December 2022

  

# I. Project Statement
The purpose of this project is to design and implement a digital audio synthesizer that runs on an embedded micro controller. All audio generation and digital signal processing will be written from scratch. The system has a hardware control interface consisting of knobs, buttons and displays for controlling settings and audio effects. The device can be played with minimal additional hardware: A MIDI source such as a basic MIDI keyboard or even a MIDI file played from a computer is enough to play notes; A set of headphones or active computer speakers can be used for playing back the audio. The system should output a line level audio signal capable of driving standard headphones. The audio signal output meets the minimum CD standard quality, but can output at higher fidelity. The system is a classic subtractive synthesizer: A combination of harmonically rich wave forms are passed through a series filters and modulated effects to produce a more complex final signal. [1] The system is also polyphonic, meaning that it can play more than one note. It should be capable of playing no less than 8 notes at a time. The synthesizer will have the standard components of subtractive audio synthesis: 

- Oscillators - Sin, saw tooth, square, triangle and white noise. [2]
- Filters - Low pass, high pass and band pass [3]
- Envelopes - Amp, filter and user configurable envelopes [4]
- Low Frequency Oscillators (LFO) - Same as standard oscillators but with much lower frequency limits with the purpose of modulating signals in real time. [5]
- Mixer - A cross fader for blending two oscillator wave forms
- Amplifier - Controls the output signal gain and may be modulated by signal sources such as envelopes and LFOs.
- Patch storage - Saving generated synthesizer parameters as "patches" in permanent storage space (SD card or flash space).
- Audio FX - Audio DSP component 

 All components will be implemented entirely in software (C++) and will be written from scratch without the aid of DSP libraries. By designing the entire audio engine and signal processing pipeline, I hope to develop a deep understanding of audio DSP and the design of complex embedded systems.

**My background and interest:**
My bachelor's degree was in commercial music with an emphasis in composition. During the many hours spent in the studio composing with various kinds of software and hardware interfaces, I realized that my interest lied less within the process of composition and more in the tools that I was using to compose. I realized what I loved most about making music was the instruments and technology rather than the music itself. I decided I would still complete my undergraduate in music and would continue my education to get a masters degree in computer science. Initially, my goal was to work in the music tech industry designing hardware and software solutions for music production. I have since developed a great interest in embedded software development in a broader sense. This project is intended to push my design and programming skills to their limits and give me the experience and background necessary to move deep into the field of embedded systems engineering.

**Why am I doing a synthesizer?**

Embedded programming is where I feel most passionate in software design. I love software best when I can physically touch the hardware components of a system and see my software make physical effects in the real world. By implementing a synthesizer from scratch, I hope to gain an expert level of knowledge and experience in real time audio that I can apply to my work throughout my career. Starting from the ground up, I will know how my generates and processes audio data down to the bits. Since I am a keyboard player, an embedded audio synthesizer is the most obvious choice since it is a world that I know well. I know what other keyboard players want and expect from a synthesizer system. By implementing this system, I will also be primed for exploring deeper topics in real time systems such as audio/video codecs and more practical DSP solutions.

# II. Methods

## 1. Tools


**Platform**: Electo-Smith Daisy Seed
I will be building the synthesizer on an embedded platform dedicated to
audio DSP called Daisy by the company Electro-Smith. The model I am using is called the Daisy Seed. It is a microcontroller that is 48mm in length and 18mm in width capable of generating extremely high fidelity audio up to 96khz sample rate and 24 bit-depth. It has the following specs and peripherals:

- ARM Cortex-M7 MCU running at 480Mhz. Is programmable in high level languages such as C, C++ and Rust
- 31 GPIO pins, 12 ADC inputs (16-bit) pins, 2 DAC outputs capable of audio output up to 96 khz and 24 bit-depth
- 64MB of SDRAM for up to 10 minute long audio buffers, 8MB external flash
- SD card interface, PWM outputs, and various serial protocols for external devices (SPI, UART, SAI/I2S, I2C)

**Software Libraries**:
- libDaisy - The HAL: The Daisy development platform includes an open source hardware abstraction library known as libDaisy [6], which will be used for setting up peripherals, compiling and mapping memory to the Cortex-M7. The manufacturer also provides an open source high level DSP library called DaisySP which will not be used in this project, as the aim of this project is to gain a deep understanding of audio DSP, however, it will be referenced among other written sources such as books and blogs for learning the core principals and practices of DSP. As of now, the only high level library that will be used is the libDaisy HAL.

**Software Tools**:

- Arm gcc toolchain: Compilation, linking and debugging will be done using the standard arm toolchain including tools such as GCC, Make and GDB.
- openOCD: openOCD will be used as an interface for running GDB debugging session and testing during the software development phase.
- STM32CubeIDE and VSCode

**Hardware Tools**: 
- MIDI Keyboard: During the software development phase, I will be utilizing some common MIDI keyboard workstations for conducting manual tests on the platform such as playing notes, using encoders to send MIDI messages for interfacing with the controller to change parameters on the system. Once the system has become sufficiently stable and a hardware interface design is done and ready to be prototyped, these devices will be phased out of the development process in favor of using a design of my own that utilizes digital rotary encoders, push buttons, analog potentiometers, LEDS and and LCD screen. The MIDI keyboards will only be required for playing notes once the project is complete. A from scratch piano keyboard will not be part of this design as it is far beyond the scope of the design and developing a working keyboard represents a massive development undertaking in itself. A simple, small array of buttons may be implemented into the synthesizer module as a stretch goal that can add additional portability to the system but is not the focus of the project.

- Stereo Headphones and Active Audio Speakers to hear and test the audio output, I will be using a pair of studio quality headphones and active (powered) monitor speakers.

- Oscilloscope and Logic Analyzer: An entry level oscilloscope and logic analyzer will be used for testing and reading audio and control signals on the system as well as monitoring communications between peripherals.

## 2. Implementation Approach

My approach to the system will be incremental, focusing first on writing the audio synthesis software. Minimal hardware integration will be required to complete most of the project; The microcontroller will be set up with an audio output jack, MIDI input jack, and a UART line out for serial printing and debugging. I will be focusing on developing a series of modular audio processing software components. Starting from generating the fundamental waveforms, I will add components to the system that will process audio buffers as a pipeline. In the beginning phases of development, the microcontroller, a stereo headphone output jack and external

midi keyboard will be used for testing as features are added to the system. Tasks associated with ”sprints” will be tracked in a GitHub project that is connected to the associated software repository. These tasks will describe the features to be added to the system. After the fundamental software components have been implemented and the system reaches the minimal required features, the development process will transition to implementing a hardware control interface to the system. This is a critical part of the system as the hardware interfaces will introduce a level of intuitive simplicity for the user that will make the system enjoyable and easy to use. The early phase of this capstone project which will run from Fall 2022 semester to the first couple weeks of Spring 2022 semester can be divided into two phases:

 1. Implementing the all the software signal processing components needed to make a device that can perform all the tasks critical to be considered a standard subtractive audio synthesizer.

2. Designing a layout and implementing a hardware interface that intuitively works for the user to properly utilize the system with relatively little effort and little to no need to consult with manuals and documentation. The implementation phase of hardware may need.

During the development phase, I expect setbacks as I will certainly need to spend time refactoring and learning new software concepts necessary to make a satisfactory and reliable product. Any remaining time of the capstone project will be dedicated to adding greater degrees of sophistication to the system in the form of feature stretch goals; These features could be additional audio effects and tools such as reverb, audio delay/echo effects, audio looping and overlay, audio sampling, live recording and playback  

## 3. Setup and Testing

Efforts have already been made in the exploratory phases of my capstone. I have already procured my own Daisy Seed micro controller and have tested things like writing audio callbacks that generate audible high quality sine wave signals. I’ve spent time writing tools for tracking the performance of the MCU through serial messages. I’ve experimented with the workflow of integrating hardware like digital rotary encoders, LCD screens and buttons to the system.

I’ve set up a working development environment using my own fork of the libDaisy hardware abstraction repository and have setup a makefile for compilation and debugging. As development ramps up, I plan to utilize MIDI messaging for integrating automated system testing into my workflow, which will help me to test new features and catch regressions as they come up. The tests can be easily written up on the fly by recording MIDI inputs on a keyboard to a MIDI file. This file can be played back on a computer and remove the need to manually test the system by hand. My design is referencing a popular synthesizer called the microKorg by Korg [6]. Although the microKorg is a very sophisticated instrument with a very rich and deep feature set, it implements the core required features that I wish to have. I plan to reference the audio output from the microKorg and compare it with the output of my system to determine how it measures up in quality with a popular and trusted product.

# III. Project Schedule

| Week | Date | Milestone | Task |
|--|--|--|--|
| Week 1-15 | 9/1/22- 12/12/2022 | Prototype Phase | Build up a working skeleton of synthesizer. Has bare minimum features for generating sound. Includes audio callback loop, MIDI processing, oscillators with all fundamental wave forms, 8 voice polyphony, amp envelope, debug tools (serial printing and system CPU monitoring) |
| Week 15-16 | 12/12 - 12/25 | Filters Part 1 | Add filter component with low pass and high pass filtering to system |
| Week 17 | 12/26 - 1/31 | Filters Part 2 | Add band pass filtering. Add filter cutoff and resonance control
| Week 18 | 1/2 - 1/8 | LFO | Add LFO component to system. Add LFO frequency and depth control
| Week 19 | 1/9 - 1 22 | LFO & Env Routing | Add ability to route secondary envelope and LFO for different parameter modulation (Volume, Pitch, Filter cutoff, Filter resonance)
| Week 20 | 1/23 - 1/29 | Patch saving | Add ability to save patches to flash memory |
| Week 21 | 1/30 - 2/5 | Pitch and Wheel Mod | Add MIDI pitch and mod wheel support. Able to select what they change in screen menu |
| Week 22-23 | 2/6 - 2/19 | Audio FX & Start Paper | Add audio FX Chain. Included FX: Delay, Reverb and Distortion Effect. Start writing paper. From here on, drafting and editing final paper is included in every task. |
| Week 24 | 2/20 - 2/26 | Hardware interface setup | Build wire harnesses for all buttons, rotary encoders, sliders, LEDS etc and wire to PCB. Harnesses long enough for trying different layout during layout design phase |
| Week 25-26 | 2/27 - 3/13 | Design Final Release Hardware Interface | Draw out different hardware interface layouts. Drill out panels with different layouts and test different layouts in prototype interface panels. Choose best layout from prototypes. | 
| Week 27 | 3/14 - 3/20 | Build final interface layout. Prepare presentation | Build up the final hardware interface in a permanent enclosure (Metal box or 3D print). Start making final presentation. From here on, work on final presentation is implicit. Prepare a feature demo and demo song |
| Week 28 to Semester End | 3/21-5/9 | Bug fixing, project stretch goals, paper and presentation prep | Fix any bugs, regressions or software design faults. Edit final paper, demo presentation to mentor. Add stretch goal features as time permits: Phaser, flanger, chorus, bitcrushing FX, looping, audio sample playback, recording and storing audio clips to SD card, FM synthesis etc.


# IV. Artifacts
- The software source repository for the final project. Includes all used libraries, source files and unit tests
- Final build binary deployed on the SOC
- Completed synthesizer device with all hardware inputs, SOC and connect circuit board within a custom designed enclosure
- Video demo of the synthesizer showing all of its features
- Video demo of the synthesizer used as instrument in a song.
- Final project powerpoint presentation
- Final project report paper

# V. Mentoring Schedule

Professor Jones and I will be meeting once or sometimes twice a week as needed. We will be primarily be communicating over Microsoft Teams. I have also added him as a main contributor to my GitHub project so that he can track my task board and make comments as desired any time he wishes.

# VI. Bibliography

- Designing Sound by Andy Farnell
- Think DSP by Allen B. Downey
- Audio Effects - Theory, Impementation and Application by Joshua D. Reiss and Andrew P. McPherson
- Embedded Systems Architecture by Daniele Lacamera
- Welsh's Synthesizer Cookbook by Fred Welsh

# VII. Proposal Agreement

This proposal has been reviewed and approved by Benjamin Stoneking and Dr. Frank Jones. The graduate student agrees that this project must be completed according to the planned schedule, and must fulfill the milestones listed. Any extension to the project must be approved by both parties. Changes to this proposal must also be agreed upon by both parties. A copy of the proposal should be given to the Program Director as a courtesy.

Student Name: Benjamin Stoneking |  Date: December 2022

Advisor Name, Frank Jones PhD Date: December 2022

References
[1] https://www.perfectcircuit.com/signal/learning-synthesis-oscillators
[2] https://musictech.com/guides/essential-guide/understanding-synthesis-filtering/
[3] https://www.aulart.com/blog/what-is-a-lfo-and-how-to-use-it/
[4] https://cmtext.indiana.edu/synthesis/chapter4_envelopes.php
[5] http://synthesizeracademy.com/subtractive-synthesis/
[6] https://electro-smith.github.io/libDaisy/index.html
[7] https://www.soundonsound.com/reviews/korg-microkorg

---
End of Proposal - Additional notes for README

Features Implemented:

 - [ ] Oscillators
	 - [x] Sine
	 - [x] Sawtooth
	 - [ ] Triangle - It's technically there but doesn't sound right
	 - [x] Square
	 - [x] Noise
 - [x] MIDI and Polyphony
	 - [x] USB MIDI Input Supported
	 - [x] 8 Voice Polyphony
	 - [x] MIDI Continuous Control (CC) Event Handling
	 - [x] Voice Oscillator Mix Control Via MIDI CC
	 - [ ] No Pops on Note Off Events - Handling this during envelope phase 
 - [ ] Envelope Class and Amp Control
 - [ ] MIDI CC for Attack/Decay/Sustain/Release control
 - [ ] Handle note off events to not pop (minimal release)
 - [ ] Extras
	 - [ ] Frequency Modulation
	 - [ ] Granular Synthesis??


