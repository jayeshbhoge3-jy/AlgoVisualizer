# 📊 AlgoVisualizer — Real-Time Algorithm Visualizer

<div align="center">
  <img src="https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white" alt="C++" />
  <img src="https://img.shields.io/badge/SFML-8CC445?style=for-the-badge&logo=sfml&logoColor=white" alt="SFML" />
  <img src="https://img.shields.io/badge/Architecture-OOP-blue?style=for-the-badge" alt="OOP" />
  <img src="https://img.shields.io/badge/Data_Structures-Graphs_%26_Arrays-orange?style=for-the-badge" alt="DSA" />
</div>

<br/>

An interactive, real-time desktop visualization tool built with C++ and SFML. This application visually simulates classic sorting and pathfinding algorithms, transforming complex multi-step data manipulations into clear, dynamic graphical animations with performance tracking.

---

## ⚡ Algorithms Supported

### 🔄 Sorting Algorithms
* **QuickSort:** Visualizes pivot selection, partitioning, and recursive array division.
* **MergeSort:** Demonstrates stable divide-and-conquer array merging animations.

### 🗺️ Pathfinding Algorithms
* **Dijkstra's Algorithm:** Animates the uniform-cost expansion over a grid system to find the absolute shortest path.
* **A* Search (A-Star):** Shows heuristic-driven search operations targeting the destination efficiently.

---

## 🛠️ Key Technical Features

* **Real-Time Rendering Pipeline:** Smooth 60FPS animations mapping array values and graph states to visual pixel data.
* **Object-Oriented Architecture (OOP):** Built using clean, modular class structures managing game loops and state encapsulation.
* **Dynamic Interactive Grid:** Allows users to dynamically draw custom obstacles/walls and watch pathfinding routines recalculate on the fly.
* **Custom Control Layout:** Features keyboard/mouse interactions to adjust animation states, reset configurations, or switch algorithms instantly.

---

## 🚀 Getting Started

### Prerequisites
* C++ Compiler (GCC/MinGW)
* SFML 2.6.1 Library

### Build Instructions (Windows/MinGW)
```bash
g++ main.cpp -I SFML/include -L SFML/lib -lsfml-graphics -lsfml-window -lsfml-system -o AlgoVisualizer
