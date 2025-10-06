# Snake Game on Arduino Uno (8x8 LED Matrix)

## 🧩 Overview  
This project implements the **classic Snake Game** using an **Arduino Uno** and an **8x8 LED Matrix display**.  
The snake moves around the matrix, eats food, and grows in length while avoiding collisions with the walls and itself.  
It’s a simple yet effective demonstration of real-time logic, LED control, and joystick input on Arduino hardware.

---

## ⚙️ Hardware Requirements  
- **Arduino Uno**  
- **8x8 LED Matrix (MAX7219 or compatible)**  
- **Joystick Module** (or 4-direction buttons)  
- **Breadboard and jumper wires**

---

## :gear: Installations 
### Arduino IDE
[Arduino IDE](https://www.arduino.cc/en/software)
-In Arduino IDE you will have to install LedControl extension.

[LedControl library](images/extn.jpg)



## 🔌 Circuit Connections  

### LED Matrix  
| Matrix Pin | Arduino Pin |
|-------------|-------------|
| VCC         | 5V          |
| GND         | GND         |
| DIN         | 12          |
| CLK         | 11          |
| CS          | 10          |

### Joystick  
| Joystick Pin | Arduino Pin |
|---------------|-------------|
| VRx           | A0          |
| VRy           | A1          |
| SW            | 2           |

---

## 🕹️ Game Logic  
- The LED matrix displays the snake and food positions.  
- The joystick controls movement: **Up, Down, Left, Right.**  
- When the snake eats food, its length increases and new food appears randomly.  
- The game ends if the snake collides with itself or the matrix boundary.

---

## 🚀 Getting Started  

1. Open `snake.ino` in **Arduino IDE**.  
2. Select **Tools → Board → Arduino Uno**.  
3. Connect your Arduino via USB.  
4. Upload the sketch to the board.  
5. Play using the joystick once the LED matrix initializes.

---

