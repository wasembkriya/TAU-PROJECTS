# Analog Amplifier Design Project in PSpice
made in collabrtion with Juan Shehady https://www.linkedin.com/in/juan-shehady-01687425a/

A final project for the "Analog Electronic Circuits" course (05123513) 
This project involves the design, mathematical analysis, and simulation of a three-stage NMOS-based amplifier circuit using OrCAD PSpice



---

## 🎯 Project Goals & Requirements
The circuit was designed to meet specific requirements derived from the sum of the authors' ID numbers[cite: 2]:

| Parameter | Requirement |
| :--- | :--- |
| **I/O Configuration** | Single Ended input and Differential Output[cite: 2] |
| **Maximum Gain ($A_M$)** | $42\text{ dB}$[cite: 2] |
| **Cutoff Frequencies ($f_1, f_2$)** | $10\text{ kHz}$ (Lower) / $1\text{ MHz}$ (Upper)[cite: 2] |
| **Frequency Response Slopes ($M_1, M_2$)** | $40\text{ dB/dec}$ (Rise) / $-20\text{ dB/dec}$ (Fall)[cite: 2] |
| **Output Resistance ($R_{out}$)** | $22\text{ k}\Omega$[cite: 2] |
| **Max Power Dissipation** | $P_{total} < 0.1\text{ W}$[cite: 2] |
| **Tested Frequency Range** | $100\text{ Hz} \le f \le 100\text{ MHz}$[cite: 2] |

---

## 🏗️ Circuit Architecture
To achieve the high voltage gain requirement ($\approx 125$ linear) and provide a differential output, a three-stage amplifier topology was designed[cite: 2]:

1. **Stage 1 (Common Source):** 
   Receives the single-ended input signal[cite: 2]. This stage utilizes an NMOS Common Source configuration with partial negative feedback (a split Source resistor with partial bypass capacitance), allowing for fine-tuning of the AC gain without altering the DC bias point[cite: 2].
2. **Stage 2 (Common Source):**
   Amplifies the intermediate signal from the first stage to achieve the total required gain without signal distortion[cite: 2]. In this stage, the Source resistor is fully bypassed by a capacitor to maximize the AC gain[cite: 2].
3. **Stage 3 (Differential Amplifier with Current Mirror):**
   Converts the amplified single-ended signal into a differential output[cite: 2]. The amplifier is biased by a current mirror, which ensures a stable tail current, improves symmetry, increases the Common-Mode Rejection Ratio (CMRR), and stabilizes the DC output voltages[cite: 2].

---

## 📊 Key Results & Simulation
The circuit was theoretically analyzed and fully verified through PSpice simulations:
* **DC Analysis:** Verified that all transistors operate in the Saturation region[cite: 2].
* **Frequency Response (Bode Plot):** The circuit behaves as a Band-Pass Filter, achieving a peak midband gain of $\approx 41.9\text{ dB}$ (well within the allowed 5% tolerance of the $42\text{ dB}$ target)[cite: 2].
* **Cutoff Frequencies & Poles:** AC simulations confirm a $-3\text{ dB}$ drop at exactly $10\text{ kHz}$ and $1\text{ MHz}$. The slopes are $40\text{ dB/dec}$ and $-20\text{ dB/dec}$, respectively, matching the designed pole configuration[cite: 2].
* **Output Resistance:** Displayed a stable differential output resistance of $22\text{ k}\Omega$ across the entire frequency band[cite: 2].
* **Power Dissipation:** The total power consumption of the circuit is $\approx 1.829\text{ mW}$, significantly lower than the $0.1\text{ W}$ maximum limit[cite: 2].

---

## 🛠️ Tools & Technologies
* **Software:** OrCAD PSpice Designer
* **Core Components:** NMOS Transistors (Custom parameters: $V_{TH}=0.5\text{V}, K_P=2\text{mA/V}^2, \lambda=0$)[cite: 2], Resistors, Capacitors.
