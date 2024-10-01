# kbd48CNC Component Documentation

## Overview

The `kbd48CNC` component is an abstraction for the PoKeys `kbd48CNC` device. It provides a set of pins, parameters, and signals to interface with the device and control various functions such as jogging, button inputs, and LED outputs.

## Pins

### Button Inputs

- `JOG_TOGGLE.Button`: Input pin for the jog toggle button.
- `JOGRATE-PLUS.Button`: Input pin for the jog rate increase button.
- `JOGRATE-MINUS.Button`: Input pin for the jog rate decrease button.
- `JOG-PLUS.Button`: Input pin for the jog plus button.
- `JOG-MINUS.Button`: Input pin for the jog minus button.
- `JOG-CONT.Button`: Input pin for the jog continuous button.
- `JOG-FAST-TOGGLE.Button`: Input pin for the jog fast toggle button.
- `JOG-INC.Button`: Input pin for the jog increment button.
- `JOG-MPG.Button`: Input pin for the jog MPG button.
- `JOG-STEP-1.Button`: Input pin for the jog step 1 button.
- `JOG-STEP-10.Button`: Input pin for the jog step 10 button.
- `JOG-STEP-100.Button`: Input pin for the jog step 100 button.
- `JOG-STEP-1000.Button`: Input pin for the jog step 1000 button.

### LED Outputs

- `JOG-TOGGLE.LED`: Output pin for the jog toggle LED.
- `JOGRATE-PLUS.LED`: Output pin for the jog rate increase LED.
- `JOGRATE-MINUS.LED`: Output pin for the jog rate decrease LED.
- `JOG-PLUS.LED`: Output pin for the jog plus LED.
- `JOG-MINUS.LED`: Output pin for the jog minus LED.
- `JOG-CONT.LED`: Output pin for the jog continuous LED.
- `JOG-FAST-TOGGLE.LED`: Output pin for the jog fast toggle LED.
- `JOG-INC.LED`: Output pin for the jog increment LED.
- `JOG-MPG.LED`: Output pin for the jog MPG LED.
- `JOG-STEP-1.LED`: Output pin for the jog step 1 LED.
- `JOG-STEP-10.LED`: Output pin for the jog step 10 LED.
- `JOG-STEP-100.LED`: Output pin for the jog step 100 LED.
- `JOG-STEP-1000.LED`: Output pin for the jog step 1000 LED.

### Jogging Parameters

- `JOG-MPG.Scale`: Output pin for the jog MPG scale.
- `JOG-MPG.Count-In`: Input pin for the jog MPG count input.
- `JOG-INC.increment.in`: Input pin for the jog increment input.
- `JOG-INC.increment.out`: Input pin for the jog increment output.

### Jog Rate Parameters

- `JOGRATE.MAX`: Input pin for the maximum jog rate in units/sec.
- `JOGRATE.init`: Input pin for the default jog rate in units/sec.
- `JOGRATE.increment`: Input pin for the jog rate increment in units/min.

### Axis Parameters

- `axis.#.jograte-max`: Output pin for the maximum jog rate for each axis.
- `axis.#.jograte-init`: Output pin for the initial jog rate for each axis.
- `axis.#.jograte-increment`: Output pin for the jog rate increment for each axis.
- `axis.#.jograte-debug`: Output pin for the jog rate debug information for each axis.

### Axis/Joint Interface

- `axis.#.selected`: Input pin to map to LED on kbd.
- `axis.#.select`: Input pin for axis selection buttons (xyzabc).
- `axis.#.jog-accel-fraction`: Output pin for setting acceleration for wheel jogging as a fraction of the ini max_acceleration for the axis.
- `axis.#.jog-counts`: Output pin to connect to the "counts" pin of an external encoder for using a physical jog wheel.
- `axis.#.jog-enable`: Output pin to enable jogging when TRUE (and in manual mode).
- `axis.#.jog-scale`: Output pin for setting the distance moved for each count on "jog-counts" in machine units.
- `axis.#.jog-vel-mode`: Output pin for setting the jog wheel to operate in velocity mode when TRUE.
- `axis.#.kb-jog-active`: Input pin for free planner axis jogging active (keyboard or halui).
- `axis.#.wheel-jog-active`: Input pin for free planner axis jogging active (keyboard or halui).
- `axis.#.jog-pos`: Output pin for jog positive direction.
- `axis.#.jog-neg`: Output pin for jog negative direction.
- `axis.#.jog-incpos`: Output pin for jog increment positive direction.
- `axis.#.jog-incneg`: Output pin for jog increment negative direction.
- `axis.#.jog-increment`: Output pin for jog increment value.
- `axis.jog-speed`: Output pin for jog speed.

### Debugging

- `alive`: Output pin to indicate the component is alive.
- `deb`: Output pin for debugging information.
- `axis.#.deb`: Output pin for axis-specific debugging information.
- `axis.#.debCntA`: Output pin for axis-specific debug counter A.
- `axis.#.debCntB`: Output pin for axis-specific debug counter B.

## Usage Examples

### Example 1: Basic Jogging Setup

```hal
loadrt kbd48CNC
addf kbd48CNC.0 servo-thread

net jog-toggle kbd48CNC.0.JOG_TOGGLE.Button => halui.jog.toggle
net jog-plus kbd48CNC.0.JOG-PLUS.Button => halui.jog.plus
net jog-minus kbd48CNC.0.JOG-MINUS.Button => halui.jog.minus
net jog-cont kbd48CNC.0.JOG-CONT.Button => halui.jog.cont
net jog-inc kbd48CNC.0.JOG-INC.Button => halui.jog.inc
net jog-mpg kbd48CNC.0.JOG-MPG.Button => halui.jog.mpg
net jog-step-1 kbd48CNC.0.JOG-STEP-1.Button => halui.jog.step-1
net jog-step-10 kbd48CNC.0.JOG-STEP-10.Button => halui.jog.step-10
net jog-step-100 kbd48CNC.0.JOG-STEP-100.Button => halui.jog.step-100
net jog-step-1000 kbd48CNC.0.JOG-STEP-1000.Button => halui.jog.step-1000

net jog-toggle-led halui.jog.toggle => kbd48CNC.0.JOG-TOGGLE.LED
net jog-plus-led halui.jog.plus => kbd48CNC.0.JOG-PLUS.LED
net jog-minus-led halui.jog.minus => kbd48CNC.0.JOG-MINUS.LED
net jog-cont-led halui.jog.cont => kbd48CNC.0.JOG-CONT.LED
net jog-inc-led halui.jog.inc => kbd48CNC.0.JOG-INC.LED
net jog-mpg-led halui.jog.mpg => kbd48CNC.0.JOG-MPG.LED
net jog-step-1-led halui.jog.step-1 => kbd48CNC.0.JOG-STEP-1.LED
net jog-step-10-led halui.jog.step-10 => kbd48CNC.0.JOG-STEP-10.LED
net jog-step-100-led halui.jog.step-100 => kbd48CNC.0.JOG-STEP-100.LED
net jog-step-1000-led halui.jog.step-1000 => kbd48CNC.0.JOG-STEP-1000.LED
```

### Example 2: Advanced Jogging with Axis Selection

```hal
loadrt kbd48CNC
addf kbd48CNC.0 servo-thread

net axis-select-x kbd48CNC.0.axis.0.select => halui.axis.x.select
net axis-select-y kbd48CNC.0.axis.1.select => halui.axis.y.select
net axis-select-z kbd48CNC.0.axis.2.select => halui.axis.z.select
net axis-select-a kbd48CNC.0.axis.3.select => halui.axis.a.select
net axis-select-b kbd48CNC.0.axis.4.select => halui.axis.b.select
net axis-select-c kbd48CNC.0.axis.5.select => halui.axis.c.select

net axis-jog-pos-x kbd48CNC.0.axis.0.jog-pos => halui.axis.x.jog-pos
net axis-jog-pos-y kbd48CNC.0.axis.1.jog-pos => halui.axis.y.jog-pos
net axis-jog-pos-z kbd48CNC.0.axis.2.jog-pos => halui.axis.z.jog-pos
net axis-jog-pos-a kbd48CNC.0.axis.3.jog-pos => halui.axis.a.jog-pos
net axis-jog-pos-b kbd48CNC.0.axis.4.jog-pos => halui.axis.b.jog-pos
net axis-jog-pos-c kbd48CNC.0.axis.5.jog-pos => halui.axis.c.jog-pos

net axis-jog-neg-x kbd48CNC.0.axis.0.jog-neg => halui.axis.x.jog-neg
net axis-jog-neg-y kbd48CNC.0.axis.1.jog-neg => halui.axis.y.jog-neg
net axis-jog-neg-z kbd48CNC.0.axis.2.jog-neg => halui.axis.z.jog-neg
net axis-jog-neg-a kbd48CNC.0.axis.3.jog-neg => halui.axis.a.jog-neg
net axis-jog-neg-b kbd48CNC.0.axis.4.jog-neg => halui.axis.b.jog-neg
net axis-jog-neg-c kbd48CNC.0.axis.5.jog-neg => halui.axis.c.jog-neg
```

## Signals

### Jogging Signals

- `jog-toggle`: Signal for the jog toggle button.
- `jog-plus`: Signal for the jog plus button.
- `jog-minus`: Signal for the jog minus button.
- `jog-cont`: Signal for the jog continuous button.
- `jog-inc`: Signal for the jog increment button.
- `jog-mpg`: Signal for the jog MPG button.
- `jog-step-1`: Signal for the jog step 1 button.
- `jog-step-10`: Signal for the jog step 10 button.
- `jog-step-100`: Signal for the jog step 100 button.
- `jog-step-1000`: Signal for the jog step 1000 button.

### LED Signals

- `jog-toggle-led`: Signal for the jog toggle LED.
- `jog-plus-led`: Signal for the jog plus LED.
- `jog-minus-led`: Signal for the jog minus LED.
- `jog-cont-led`: Signal for the jog continuous LED.
- `jog-inc-led`: Signal for the jog increment LED.
- `jog-mpg-led`: Signal for the jog MPG LED.
- `jog-step-1-led`: Signal for the jog step 1 LED.
- `jog-step-10-led`: Signal for the jog step 10 LED.
- `jog-step-100-led`: Signal for the jog step 100 LED.
- `jog-step-1000-led`: Signal for the jog step 1000 LED.

### Axis Signals

- `axis-select-x`: Signal for selecting the X axis.
- `axis-select-y`: Signal for selecting the Y axis.
- `axis-select-z`: Signal for selecting the Z axis.
- `axis-select-a`: Signal for selecting the A axis.
- `axis-select-b`: Signal for selecting the B axis.
- `axis-select-c`: Signal for selecting the C axis.
- `axis-jog-pos-x`: Signal for jogging the X axis in the positive direction.
- `axis-jog-pos-y`: Signal for jogging the Y axis in the positive direction.
- `axis-jog-pos-z`: Signal for jogging the Z axis in the positive direction.
- `axis-jog-pos-a`: Signal for jogging the A axis in the positive direction.
- `axis-jog-pos-b`: Signal for jogging the B axis in the positive direction.
- `axis-jog-pos-c`: Signal for jogging the C axis in the positive direction.
- `axis-jog-neg-x`: Signal for jogging the X axis in the negative direction.
- `axis-jog-neg-y`: Signal for jogging the Y axis in the negative direction.
- `axis-jog-neg-z`: Signal for jogging the Z axis in the negative direction.
- `axis-jog-neg-a`: Signal for jogging the A axis in the negative direction.
- `axis-jog-neg-b`: Signal for jogging the B axis in the negative direction.
- `axis-jog-neg-c`: Signal for jogging the C axis in the negative direction.

## Signals Defined in kbd48CNC.hal

### Jogging Signals

- `jog-toggle`: Signal for the jog toggle button.
- `jog-plus`: Signal for the jog plus button.
- `jog-minus`: Signal for the jog minus button.
- `jog-cont`: Signal for the jog continuous button.
- `jog-inc`: Signal for the jog increment button.
- `jog-mpg`: Signal for the jog MPG button.
- `jog-step-1`: Signal for the jog step 1 button.
- `jog-step-10`: Signal for the jog step 10 button.
- `jog-step-100`: Signal for the jog step 100 button.
- `jog-step-1000`: Signal for the jog step 1000 button.

### LED Signals

- `jog-toggle-led`: Signal for the jog toggle LED.
- `jog-plus-led`: Signal for the jog plus LED.
- `jog-minus-led`: Signal for the jog minus LED.
- `jog-cont-led`: Signal for the jog continuous LED.
- `jog-inc-led`: Signal for the jog increment LED.
- `jog-mpg-led`: Signal for the jog MPG LED.
- `jog-step-1-led`: Signal for the jog step 1 LED.
- `jog-step-10-led`: Signal for the jog step 10 LED.
- `jog-step-100-led`: Signal for the jog step 100 LED.
- `jog-step-1000-led`: Signal for the jog step 1000 LED.

### Axis Signals

- `axis-select-x`: Signal for selecting the X axis.
- `axis-select-y`: Signal for selecting the Y axis.
- `axis-select-z`: Signal for selecting the Z axis.
- `axis-select-a`: Signal for selecting the A axis.
- `axis-select-b`: Signal for selecting the B axis.
- `axis-select-c`: Signal for selecting the C axis.
- `axis-jog-pos-x`: Signal for jogging the X axis in the positive direction.
- `axis-jog-pos-y`: Signal for jogging the Y axis in the positive direction.
- `axis-jog-pos-z`: Signal for jogging the Z axis in the positive direction.
- `axis-jog-pos-a`: Signal for jogging the A axis in the positive direction.
- `axis-jog-pos-b`: Signal for jogging the B axis in the positive direction.
- `axis-jog-pos-c`: Signal for jogging the C axis in the positive direction.
- `axis-jog-neg-x`: Signal for jogging the X axis in the negative direction.
- `axis-jog-neg-y`: Signal for jogging the Y axis in the negative direction.
- `axis-jog-neg-z`: Signal for jogging the Z axis in the negative direction.
- `axis-jog-neg-a`: Signal for jogging the A axis in the negative direction.
- `axis-jog-neg-b`: Signal for jogging the B axis in the negative direction.
- `axis-jog-neg-c`: Signal for jogging the C axis in the negative direction.
