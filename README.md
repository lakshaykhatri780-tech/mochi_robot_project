# 🤖 Mochi: An Interactive 2-Axis Companion Robot

Mochi is an advanced, touch-sensitive companion robot engineered for the *Star Dance Hack Club Challenge*. It features a self-calibrating hardware architecture, organic mathematical motor motion tracking, dynamic chiptune vocalization feedback, and retro chibi-inspired vector display expressions.

---

## 🚀 Key Engineering Accomplishments

*   *🧮 Asynchronous Real-Time Clock Engine:* Built entirely using non-blocking delta-timing thresholds (millis()). This eliminates freezing execution bottlenecks, ensuring touch sensors and display graphics stay completely responsive.
*   *📈 Mathematical Servo Easing (Lerp):* Implemented a custom Linear Interpolation formula (Current + (Target - Current) * Speed) running inside a fast execution block. This gives Mochi fluid, organic neck gestures instead of stiff mechanical loops.
*   *🎙️ Chiptune Vocalization Pipeline:* Engineered a safe audio frequency sound engine using a passive current-restricting setup. This triggers unique 8-bit sound arrays synchronized with Mochi's emotional states.
*   *🔋 Dual-Bus Power Segregation:* Isolated heavy inductive servo motor power demands from the sensitive microcontroller logic rails to prevent system voltage crashes.

---

## 🎨 Interactive Behavior Matrix

Mochi reads human touch-and-hold interactions through a digital filtering engine to navigate smoothly between four distinct states:

| State & Emotion | "No" Motor (Pan) | "Yes" Motor (Tilt) | Audio Vector Feedback | Screen Graphic |
| :--- | :--- | :--- | :--- | :--- |
| *😐 Idle / Calm* | Centered (90°) | Centered (90°) | Periodic Ambient Blink Blips | Rounded Rectangles |
| *😆 Happy Dance* | Sinusoidal Waves | Fast Cosine Bobs | Rising 3-Tone Arpeggio | Joyful Laugh Arches |
| *😢 Sad Droop* | Bashful Turn (65°) | Drooped Down (40°) | Downward Whistle Slide | Anxious Corner Masks |
| *😡 Angry Jerk* | Rigid Cocked (125°) | Stiff Tilt (110°) | High-Frequency Tone Grunts | Slanted Eyebrow Blocks |

---

## 🛠️ System Pin Connections

### Power Routing Architecture
*   *ESP32 Logic Rail (3.3V):* Powers the 0.96" OLED Display logic.
*   *External Power Bus (5V-6V AA Battery Tray):* Sources the heavy current needed for the servo motors.
*   *Shared Ground Configuration:* All ground terminals (Battery negative, Servos brown wires, OLED ground, and Speaker ground) tie to a central baseline node connected back to the ESP32 GND pin.

### Digital Logic Map
*   *GPIO 4 (D4):* Capacitive debounced touch loop switch.
*   *GPIO 21 (D21) / GPIO 22 (D22):* OLED I2C SDA and SCL data pathways.
*   *GPIO 12 (D12):* Lower Pan Servo PWM engine control.
*   *GPIO 13 (D13):* Upper Tilt Servo PWM engine control.
*   *GPIO 25 (D25):* Sound wave audio frequency engine (passes through an inline 150Ω resistor).

---

## 📐 Material & Fabrication Notes
The robot chassis was custom-fabricated from flattened *PVC pipe sheets*. Every individual component plate was manually scored, snapped, and super-glued to create a lightweight, sturdy structure designed to support a compact 2-axis pan-tilt mechanical neck articulation mechanism.
