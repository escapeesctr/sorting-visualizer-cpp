# Sorting Algorithm Visualizer in C++ with SFML

An interactive visualization tool that demonstrates how different sorting algorithms work in real-time, with step-by-step visual feedback and statistics.

## 🎯 Features
- **4 Sorting Algorithms**: Bubble Sort, Quick Sort, Merge Sort, Selection Sort
- **Real-time Visualization**: Watch algorithms sort step-by-step
- **Color-coded Elements**: 
  - 🔵 Blue: Default state
  - 🔴 Red: Elements being compared
  - 🟢 Green: Elements being swapped
  - 🟣 Purple: Sorted elements
- **Live Statistics**: Comparisons, swaps, execution time, speed control
- **Interactive Controls**: Start/pause, reset, algorithm selection, speed adjustment

## 🕹️ Controls
| Key | Action |
|-----|--------|
| **1** | Select Bubble Sort |
| **2** | Select Quick Sort |
| **3** | Select Merge Sort |
| **4** | Select Selection Sort |
| **SPACE** | Start/Pause sorting |
| **R** | Reset data (generate new random array) |
| **+** | Increase sorting speed |
| **-** | Decrease sorting speed |

## 📊 Algorithm Details

### Bubble Sort
- **Time Complexity**: O(n²)
- **Space Complexity**: O(1)
- **Visualization**: Adjacent elements compared and swapped

### Quick Sort
- **Time Complexity**: O(n log n) average, O(n²) worst
- **Space Complexity**: O(log n)
- **Visualization**: Partitioning with pivot element

### Selection Sort
- **Time Complexity**: O(n²)
- **Space Complexity**: O(1)
- **Visualization**: Finding minimum element and swapping

### Merge Sort
- **Time Complexity**: O(n log n)
- **Space Complexity**: O(n)
- **Visualization**: Divide and conquer approach

## 🏗️ Architecture
### Class Structure
- `SortingVisualizer`: Main class managing visualization and algorithms
- Real-time state tracking for each algorithm
- Separate visualization and logic layers

### Visualization System
- Dynamic bar representation of array elements
- Smooth color transitions for state changes
- Axis and legend for better understanding
- Responsive UI with real-time statistics

### Algorithm States
Each algorithm maintains its own state for step-by-step execution:
- **Bubble Sort**: Tracks current pass and comparison indices
- **Quick Sort**: Manages partition stack and pivot positions
- **Selection Sort**: Tracks minimum element search
- **Merge Sort**: (Basic implementation for demonstration)

## 🎨 Visual Design
- **Dark Theme**: Easy on eyes for prolonged viewing
- **Color Psychology**: Intuitive color coding for algorithm states
- **Clean Layout**: Clear separation of visualization and controls
- **Information Hierarchy**: Important statistics prominently displayed

## 📈 Educational Value
This project helps understand:
- How different algorithms approach the same problem
- Time and space complexity in practice
- The importance of algorithm selection for different datasets
- Visual debugging and algorithm analysis techniques

## 🔧 Technical Implementation
### Key Components
1. **SFML Graphics**: For rendering bars and UI elements
2. **State Machines**: For each algorithm's step-by-step execution
3. **Event Handling**: Responsive keyboard controls
4. **Statistics Tracking**: Real-time performance metrics
5. **Random Data Generation**: Dynamic array creation

### Code Highlights
```cpp
// Step-by-step bubble sort implementation
bool stepBubbleSort() {
    if (bubbleI < data.size() - 1) {
        if (bubbleJ < data.size() - bubbleI - 1) {
            highlightCompare(bubbleJ, bubbleJ + 1);
            comparisons++;
            
            if (data[bubbleJ] > data[bubbleJ + 1]) {
                swapElements(bubbleJ, bubbleJ + 1);
                highlightSwap(bubbleJ, bubbleJ + 1);
            }
            bubbleJ++;
            return true;
        }
    }
    return false;
}
```

## 🚀 What I Learned
- Implementing multiple algorithms with shared visualization
- Real-time algorithm state management
- Interactive educational tool design
- SFML advanced features (views, text rendering, color manipulation)
- Performance measurement and visualization techniques
- User interface design for complex simulations

## 📊 Performance Features
- 60 FPS rendering for smooth animations
- Configurable speed from 10ms to 500ms per step
- Efficient state tracking without redundant computations
- Minimal memory overhead for large datasets

## 🎯 Real-World Applications
- Computer Science Education: Algorithm visualization for students
- Interview Preparation: Understanding sorting algorithm behavior
- Algorithm Research: Visual comparison of different approaches
- Performance Analysis: Real-time statistics for optimization

## 🔮 Future Improvements
- Add more algorithms (Heap Sort, Radix Sort, Tim Sort)
- Implement sound feedback for swaps and comparisons
- Add dataset customization (sorted, reverse sorted, random)
- Export visualization as video or GIF
- Multi-threaded algorithm comparison
- Mobile/Web port using different graphics libraries

## 🛠️ Build Instructions
- Install SFML and configure include/library paths
- Build with Visual Studio (x64, Debug configuration)
- Copy SFML DLLs to executable directory
- Run and use keyboard controls to interact

## 🔗 Connect
- Portfolio: https://escapeesctr.github.io/my-portfolio/
- LinkedIn: https://www.linkedin.com/in/melihenesaktas/
- GitHub: @escapeesctr
