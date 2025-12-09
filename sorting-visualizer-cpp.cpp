#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <string>

class SortingVisualizer {
private:
    sf::RenderWindow window;
    std::vector<int> data;
    std::vector<sf::RectangleShape> bars;

    enum class Algorithm {
        BUBBLE_SORT,
        QUICK_SORT,
        MERGE_SORT,
        SELECTION_SORT,
        NONE
    };

    Algorithm currentAlgorithm;
    bool isSorting;
    bool isPaused;
    int sortingSpeed; 

    int comparisons;
    int swaps;
    std::chrono::time_point<std::chrono::steady_clock> startTime;

    sf::Color defaultColor;
    sf::Color compareColor;
    sf::Color swapColor;
    sf::Color sortedColor;

    sf::Font font;
    sf::Text algorithmText;
    sf::Text statsText;
    sf::Text controlsText;

    size_t bubbleI, bubbleJ;

    struct QuickSortState {
        int low, high;
        bool partitioning;
        int pivot, i, j;
        std::vector<std::pair<int, int>> stack;
    } quickSortState;

    struct MergeSortState {
        int left, right, mid;
        bool merging;
        std::vector<int> temp;
        int i, j, k;
    } mergeSortState;

    size_t selectionI, selectionJ, selectionMin;

public:
    SortingVisualizer() :
        window(sf::VideoMode(1200, 700), "Sorting Algorithm Visualizer", sf::Style::Close),
        currentAlgorithm(Algorithm::NONE),
        isSorting(false),
        isPaused(false),
        sortingSpeed(50),
        comparisons(0),
        swaps(0),
        defaultColor(sf::Color(70, 130, 180)),
        compareColor(sf::Color(255, 99, 71)),
        swapColor(sf::Color(50, 205, 50)),
        sortedColor(sf::Color(147, 112, 219))
    {
        initializeData();
        setupUI();
    }

    void initializeData() {
        data.clear();
        bars.clear();

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(50, 600);

        for (int i = 0; i < 100; ++i) {
            data.push_back(dist(gen));
        }

        float barWidth = static_cast<float>(window.getSize().x - 200) / data.size();

        for (size_t i = 0; i < data.size(); ++i) {
            sf::RectangleShape bar(sf::Vector2f(barWidth - 1, static_cast<float>(data[i])));
            bar.setPosition(100 + i * barWidth, 600 - bar.getSize().y);
            bar.setFillColor(defaultColor);
            bars.push_back(bar);
        }
    }

    void setupUI() {
        if (!font.loadFromFile("arial.ttf")) {
            font = sf::Font();
            if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
                std::cout << "System font not found either, text will be invisible\n";
            }
        }

        algorithmText.setFont(font);
        algorithmText.setCharacterSize(24);
        algorithmText.setFillColor(sf::Color::White);
        algorithmText.setPosition(20, 20);
        updateAlgorithmText();

        statsText.setFont(font);
        statsText.setCharacterSize(20);
        statsText.setFillColor(sf::Color::White);
        statsText.setPosition(20, 60);
        updateStatsText();

        controlsText.setFont(font);
        controlsText.setCharacterSize(18);
        controlsText.setFillColor(sf::Color(200, 200, 200));
        controlsText.setPosition(20, 650);
        controlsText.setString(
            "Controls: 1-Bubble | 2-Quick | 3-Merge | 4-Selection | SPACE-Start/Pause | R-Reset | +/- Speed"
        );

        resetAlgorithmStates();
    }

    void resetAlgorithmStates() {
        bubbleI = 0;
        bubbleJ = 0;

        quickSortState = { 0, static_cast<int>(data.size() - 1), false, 0, 0, 0, {} };
        quickSortState.stack.push_back({ 0, static_cast<int>(data.size() - 1) });

        mergeSortState = { 0, static_cast<int>(data.size() - 1), 0, false, {}, 0, 0, 0 };

        selectionI = 0;
        selectionJ = 0;
        selectionMin = 0;
    }

    void updateAlgorithmText() {
        std::string algoName;
        switch (currentAlgorithm) {
        case Algorithm::BUBBLE_SORT: algoName = "Bubble Sort"; break;
        case Algorithm::QUICK_SORT: algoName = "Quick Sort"; break;
        case Algorithm::MERGE_SORT: algoName = "Merge Sort"; break;
        case Algorithm::SELECTION_SORT: algoName = "Selection Sort"; break;
        case Algorithm::NONE: algoName = "No Algorithm Selected"; break;
        }

        std::string status = isSorting ? (isPaused ? " (PAUSED)" : " (RUNNING)") : " (READY)";
        algorithmText.setString("Algorithm: " + algoName + status);
    }

    void updateStatsText() {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
            isSorting ? now - startTime : std::chrono::milliseconds(0)
        ).count();

        statsText.setString(
            "Comparisons: " + std::to_string(comparisons) +
            "  |  Swaps: " + std::to_string(swaps) +
            "  |  Time: " + std::to_string(elapsed) + "ms" +
            "  |  Speed: " + std::to_string(sortingSpeed) + "ms"
        );
    }

    void resetColors() {
        for (auto& bar : bars) {
            bar.setFillColor(defaultColor);
        }
    }

    void highlightCompare(size_t i, size_t j) {
        resetColors();
        if (i < bars.size()) bars[i].setFillColor(compareColor);
        if (j < bars.size()) bars[j].setFillColor(compareColor);
    }

    void highlightSwap(size_t i, size_t j) {
        resetColors();
        if (i < bars.size()) bars[i].setFillColor(swapColor);
        if (j < bars.size()) bars[j].setFillColor(swapColor);
    }

    void highlightSorted(size_t i) {
        if (i < bars.size()) bars[i].setFillColor(sortedColor);
    }

    void swapElements(size_t i, size_t j) {
        std::swap(data[i], data[j]);

        float tempHeight = bars[i].getSize().y;
        bars[i].setSize(sf::Vector2f(bars[i].getSize().x, bars[j].getSize().y));
        bars[j].setSize(sf::Vector2f(bars[j].getSize().x, tempHeight));

        bars[i].setPosition(bars[i].getPosition().x, 600 - bars[i].getSize().y);
        bars[j].setPosition(bars[j].getPosition().x, 600 - bars[j].getSize().y);

        swaps++;
    }

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
            else {
                highlightSorted(data.size() - bubbleI - 1);
                bubbleJ = 0;
                bubbleI++;
                return true;
            }
        }
        return false;
    }

    bool stepQuickSort() {
        if (quickSortState.stack.empty()) {
            return false;
        }

        if (!quickSortState.partitioning) {
            quickSortState.low = quickSortState.stack.back().first;
            quickSortState.high = quickSortState.stack.back().second;
            quickSortState.stack.pop_back();

            if (quickSortState.low < quickSortState.high) {
                quickSortState.pivot = data[quickSortState.high];
                quickSortState.i = quickSortState.low - 1;
                quickSortState.j = quickSortState.low;
                quickSortState.partitioning = true;
            }
            return true;
        }

        if (quickSortState.j <= quickSortState.high - 1) {
            highlightCompare(quickSortState.j, quickSortState.high);
            comparisons++;

            if (data[quickSortState.j] < quickSortState.pivot) {
                quickSortState.i++;
                swapElements(quickSortState.i, quickSortState.j);
                highlightSwap(quickSortState.i, quickSortState.j);
            }

            quickSortState.j++;
            return true;
        }
        else {
            swapElements(quickSortState.i + 1, quickSortState.high);
            highlightSwap(quickSortState.i + 1, quickSortState.high);

            int pi = quickSortState.i + 1;

            quickSortState.stack.push_back({ quickSortState.low, pi - 1 });
            quickSortState.stack.push_back({ pi + 1, quickSortState.high });

            quickSortState.partitioning = false;
            return true;
        }
    }

    bool stepSelectionSort() {
        if (selectionI < data.size() - 1) {
            if (selectionJ < data.size()) {
                highlightCompare(selectionJ, selectionMin);
                comparisons++;

                if (data[selectionJ] < data[selectionMin]) {
                    selectionMin = selectionJ;
                }

                selectionJ++;
                return true;
            }
            else {
                if (selectionMin != selectionI) {
                    swapElements(selectionI, selectionMin);
                    highlightSwap(selectionI, selectionMin);
                }

                highlightSorted(selectionI);
                selectionI++;
                selectionJ = selectionI + 1;
                selectionMin = selectionI;
                return true;
            }
        }
        return false;
    }

    void stepAlgorithm() {
        if (!isSorting || isPaused) return;

        bool continueSorting = true;

        switch (currentAlgorithm) {
        case Algorithm::BUBBLE_SORT:
            continueSorting = stepBubbleSort();
            break;
        case Algorithm::QUICK_SORT:
            continueSorting = stepQuickSort();
            break;
        case Algorithm::SELECTION_SORT:
            continueSorting = stepSelectionSort();
            break;
        case Algorithm::MERGE_SORT:
            continueSorting = false;
            break;
        case Algorithm::NONE:
            return;
        }

        if (!continueSorting) {
            isSorting = false;
            for (size_t i = 0; i < bars.size(); ++i) {
                highlightSorted(i);
            }
        }

        updateStatsText();
    }

    void startSorting() {
        if (currentAlgorithm == Algorithm::NONE) return;

        if (!isSorting) {
            isSorting = true;
            isPaused = false;
            comparisons = 0;
            swaps = 0;
            startTime = std::chrono::steady_clock::now();
            resetAlgorithmStates();
        }
        else {
            isPaused = !isPaused;
        }

        updateAlgorithmText();
    }

    void handleInput() {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
            currentAlgorithm = Algorithm::BUBBLE_SORT;
            isSorting = false;
            updateAlgorithmText();
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
            currentAlgorithm = Algorithm::QUICK_SORT;
            isSorting = false;
            updateAlgorithmText();
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) {
            currentAlgorithm = Algorithm::MERGE_SORT;
            isSorting = false;
            updateAlgorithmText();
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)) {
            currentAlgorithm = Algorithm::SELECTION_SORT;
            isSorting = false;
            updateAlgorithmText();
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            startSorting();
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
            initializeData();
            isSorting = false;
            comparisons = 0;
            swaps = 0;
            updateStatsText();
            updateAlgorithmText();
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Add) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            sortingSpeed = std::max(10, sortingSpeed - 10);
            updateStatsText();
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            sortingSpeed = std::min(500, sortingSpeed + 10);
            updateStatsText();
        }
    }

    void render() {
        window.clear(sf::Color(30, 30, 30));

        for (const auto& bar : bars) {
            window.draw(bar);
        }

        sf::RectangleShape xAxis(sf::Vector2f(window.getSize().x - 200, 2));
        xAxis.setPosition(100, 600);
        xAxis.setFillColor(sf::Color::White);
        window.draw(xAxis);

        sf::RectangleShape yAxis(sf::Vector2f(2, 550));
        yAxis.setPosition(100, 50);
        yAxis.setFillColor(sf::Color::White);
        window.draw(yAxis);

        window.draw(algorithmText);
        window.draw(statsText);
        window.draw(controlsText);

        sf::RectangleShape legendDefault(sf::Vector2f(20, 20));
        legendDefault.setPosition(800, 20);
        legendDefault.setFillColor(defaultColor);
        window.draw(legendDefault);

        sf::Text legendDefaultText("Default", font, 16);
        legendDefaultText.setPosition(830, 20);
        legendDefaultText.setFillColor(sf::Color::White);
        window.draw(legendDefaultText);

        sf::RectangleShape legendCompare(sf::Vector2f(20, 20));
        legendCompare.setPosition(800, 50);
        legendCompare.setFillColor(compareColor);
        window.draw(legendCompare);

        sf::Text legendCompareText("Comparing", font, 16);
        legendCompareText.setPosition(830, 50);
        legendCompareText.setFillColor(sf::Color::White);
        window.draw(legendCompareText);

        sf::RectangleShape legendSwap(sf::Vector2f(20, 20));
        legendSwap.setPosition(800, 80);
        legendSwap.setFillColor(swapColor);
        window.draw(legendSwap);

        sf::Text legendSwapText("Swapping", font, 16);
        legendSwapText.setPosition(830, 80);
        legendSwapText.setFillColor(sf::Color::White);
        window.draw(legendSwapText);

        sf::RectangleShape legendSorted(sf::Vector2f(20, 20));
        legendSorted.setPosition(800, 110);
        legendSorted.setFillColor(sortedColor);
        window.draw(legendSorted);

        sf::Text legendSortedText("Sorted", font, 16);
        legendSortedText.setPosition(830, 110);
        legendSortedText.setFillColor(sf::Color::White);
        window.draw(legendSortedText);

        window.display();
    }

    void run() {
        sf::Clock clock;

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            handleInput();

            if (clock.getElapsedTime().asMilliseconds() > sortingSpeed) {
                stepAlgorithm();
                clock.restart();
            }

            render();
        }
    }
};

int main() {
    std::cout << "=== SORTING ALGORITHM VISUALIZER ===\n";
    std::cout << "Controls:\n";
    std::cout << "1 - Bubble Sort\n";
    std::cout << "2 - Quick Sort\n";
    std::cout << "3 - Merge Sort\n";
    std::cout << "4 - Selection Sort\n";
    std::cout << "SPACE - Start/Pause\n";
    std::cout << "R - Reset Data\n";
    std::cout << "+ - Increase Speed\n";
    std::cout << "- - Decrease Speed\n\n";
    std::cout << "Watch the algorithms sort in real-time!\n";

    SortingVisualizer visualizer;
    visualizer.run();

    return 0;
}