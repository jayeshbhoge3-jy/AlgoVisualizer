
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <random>
#include <cmath>
#include <queue>
#include <thread>
#include <chrono>
#include <algorithm>

// --- CONSTANTS ---
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int ARRAY_SIZE = 160;
const int GRID_ROWS = 30;
const int GRID_COLS = 40;
const int CELL_SIZE = 20;

// --- STRUCTURES FOR PATHFINDING ---
struct Node {
    int r, c;
    int gScore = 1e9;
    int fScore = 1e9;
    bool isWall = false;
    bool isVisited = false;
    bool isClosed = false;
    Node* parent = nullptr;
};

// Custom comparator for priority queue (A* and Dijkstra)
struct NodeCompare {
    bool operator()(const Node* n1, const Node* n2) const {
        return n1->fScore > n2->fScore;
    }
};

class AlgoVisualizer {
private:
    sf::RenderWindow window;
    enum class Mode { Menu, Sorting, Pathfinding } currentMode;
    
    // Sorting Data
    std::vector<int> array;
    int activeIdx1 = -1;
    int activeIdx2 = -1;

    // Pathfinding Data
    std::vector<std::vector<Node>> grid;
    Node* startNode = nullptr;
    Node* endNode = nullptr;

public:
    AlgoVisualizer() : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "AlgoVisualizer Pro") {
        currentMode = Mode::Menu;
        resetSortingData();
        resetPathfindingData();
    }

    void resetSortingData() {
        array.clear();
        for (int i = 1; i <= ARRAY_SIZE; ++i) {
            array.push_back(static_cast<int>(i * (WINDOW_HEIGHT - 100) / ARRAY_SIZE));
        }
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(array.begin(), array.end(), g);
        activeIdx1 = -1;
        activeIdx2 = -1;
    }

    void resetPathfindingData() {
        grid.clear();
        grid.resize(GRID_ROWS, std::vector<Node>(GRID_COLS));
        for (int r = 0; r < GRID_ROWS; ++r) {
            for (int c = 0; c < GRID_COLS; ++c) {
                grid[r][c].r = r;
                grid[r][c].c = c;
            }
        }
        startNode = &grid[5][5];
        endNode = &grid[25][35];
    }

    void run() {
        while (window.isOpen()) {
            handleEvents();
            render();
        }
    }

private:
    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    currentMode = Mode::Menu;
                } else if (currentMode == Mode::Menu) {
                    if (event.key.code == sf::Keyboard::Num1) {
                        currentMode = Mode::Sorting;
                        resetSortingData();
                    } else if (event.key.code == sf::Keyboard::Num2) {
                        currentMode = Mode::Pathfinding;
                        resetPathfindingData();
                    }
                } else if (currentMode == Mode::Sorting) {
                    if (event.key.code == sf::Keyboard::R) resetSortingData();
                    if (event.key.code == sf::Keyboard::Q) runQuickSort(0, ARRAY_SIZE - 1);
                    if (event.key.code == sf::Keyboard::M) runMergeSort(0, ARRAY_SIZE - 1);
                } else if (currentMode == Mode::Pathfinding) {
                    if (event.key.code == sf::Keyboard::R) resetPathfindingData();
                    if (event.key.code == sf::Keyboard::D) runDijkstra();
                    if (event.key.code == sf::Keyboard::A) runAStar();
                }
            }

            // Mouse handling for drawing walls in Pathfinding mode
            if (currentMode == Mode::Pathfinding && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                int c = mousePos.x / CELL_SIZE;
                int r = mousePos.y / CELL_SIZE;
                if (r >= 0 && r < GRID_ROWS && c >= 0 && c < GRID_COLS) {
                    if (&grid[r][c] != startNode && &grid[r][c] != endNode) {
                        grid[r][c].isWall = true;
                    }
                }
            }
        }
    }

    // --- SORTING VISUALIZATIONS ---
    void runQuickSort(int low, int high) {
        if (low < high) {
            int pIdx = partition(low, high);
            runQuickSort(low, pIdx - 1);
            runQuickSort(pIdx + 1, high);
        }
        activeIdx1 = -1;
        activeIdx2 = -1;
    }

    int partition(int low, int high) {
        int pivot = array[high];
        int i = low - 1;
        for (int j = low; j < high; ++j) {
            activeIdx1 = j;
            activeIdx2 = high;
            if (array[j] < pivot) {
                i++;
                std::swap(array[i], array[j]);
            }
            render();
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
        }
        std::swap(array[i + 1], array[high]);
        render();
        return i + 1;
    }

    void runMergeSort(int left, int right) {
        if (left < right) {
            int mid = left + (right - left) / 2;
            runMergeSort(left, mid);
            runMergeSort(mid + 1, right);
            merge(left, mid, right);
        }
    }

    void merge(int left, int mid, int right) {
        std::vector<int> temp;
        int i = left, j = mid + 1;
        while (i <= mid && j <= right) {
            activeIdx1 = i;
            activeIdx2 = j;
            if (array[i] <= array[j]) {
                temp.push_back(array[i++]);
            } else {
                temp.push_back(array[j++]);
            }
            render();
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
        while (i <= mid) temp.push_back(array[i++]);
        while (j <= right) temp.push_back(array[j++]);
        for (int k = 0; k < temp.size(); ++k) {
            array[left + k] = temp[k];
            render();
        }
    }

    // --- PATHFINDING VISUALIZATIONS ---
    void runDijkstra() { runPathfindingAlgorithm(false); }
    void runAStar() { runPathfindingAlgorithm(true); }

    void runPathfindingAlgorithm(bool useHeuristic) {
        std::priority_queue<Node*, std::vector<Node*>, NodeCompare> openSet;
        startNode->gScore = 0;
        startNode->fScore = useHeuristic ? heuristic(startNode, endNode) : 0;
        openSet.push(startNode);

        int dr[] = {-1, 1, 0, 0};
        int dc[] = {0, 0, -1, 1};

        while (!openSet.empty()) {
            Node* current = openSet.top();
            openSet.pop();

            if (current->isClosed) continue;
            current->isClosed = true;

            if (current == endNode) {
                highlightPath();
                return;
            }

            for (int i = 0; i < 4; ++i) {
                int nr = current->r + dr[i];
                int nc = current->c + dc[i];

                if (nr >= 0 && nr < GRID_ROWS && nc >= 0 && nc < GRID_COLS) {
                    Node* neighbor = &grid[nr][nc];
                    if (neighbor->isWall || neighbor->isClosed) continue;

                    int tentativeG = current->gScore + 1;
                    if (tentativeG < neighbor->gScore) {
                        neighbor->parent = current;
                        neighbor->gScore = tentativeG;
                        neighbor->fScore = tentativeG + (useHeuristic ? heuristic(neighbor, endNode) : 0);
                        neighbor->isVisited = true;
                        openSet.push(neighbor);
                    }
                }
            }
            render();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    int heuristic(Node* a, Node* b) {
        return std::abs(a->r - b->r) + std::abs(a->c - b->c);
    }

    void highlightPath() {
        Node* curr = endNode->parent;
        while (curr != nullptr && curr != startNode) {
            curr->isWall = false; 
            curr->isVisited = false; 
            curr->gScore = -1; 
            curr = curr->parent;
            render();
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
    }

    // --- RENDERING PIPELINE ---
    void render() {
        window.clear(sf::Color(15, 23, 42)); // Smooth Dark Midnight Theme

        if (currentMode == Mode::Menu) {
            sf::RectangleShape background(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
            background.setFillColor(sf::Color(20, 20, 35));
            window.draw(background);
        } 
        else if (currentMode == Mode::Sorting) {
            float barWidth = static_cast<float>(WINDOW_WIDTH) / ARRAY_SIZE;
            for (size_t i = 0; i < array.size(); ++i) {
                sf::RectangleShape bar(sf::Vector2f(barWidth - 1, static_cast<float>(array[i])));
                bar.setPosition(i * barWidth, WINDOW_HEIGHT - array[i]);
                
                if (static_cast<int>(i) == activeIdx1 || static_cast<int>(i) == activeIdx2) {
                    bar.setFillColor(sf::Color(234, 179, 8)); // Gold/Yellow Highlight
                } else {
                    bar.setFillColor(sf::Color(14, 165, 233)); // Clean Blue Core Bars
                }
                window.draw(bar);
            }
        } 
        else if (currentMode == Mode::Pathfinding) {
            for (int r = 0; r < GRID_ROWS; ++r) {
                for (int c = 0; c < GRID_COLS; ++c) {
                    sf::RectangleShape cell(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
                    cell.setPosition(c * CELL_SIZE, r * CELL_SIZE);

                    if (&grid[r][c] == startNode) cell.setFillColor(sf::Color(34, 197, 94)); // Green Start
                    else if (&grid[r][c] == endNode) cell.setFillColor(sf::Color(239, 68, 68)); // Red End
                    else if (grid[r][c].gScore == -1) cell.setFillColor(sf::Color(234, 179, 8)); // Shortest Path Gold
                    else if (grid[r][c].isWall) cell.setFillColor(sf::Color(71, 85, 105)); // Slate Obstacle Walls
                    else if (grid[r][c].isClosed) cell.setFillColor(sf::Color(30, 41, 59)); // Visited Evaluation
                    else if (grid[r][c].isVisited) cell.setFillColor(sf::Color(56, 189, 248)); // Open Frontier
                    else cell.setFillColor(sf::Color(51, 65, 85)); // Untouched Node Grid

                    window.draw(cell);
                }
            }
        }
        window.display();
    }
};

int main() {
    std::cout << "--- AlgoVisualizer Keyboard Mapping ---\n";
    std::cout << "Press [1] -> Switch to Sorting Layout\n";
    std::cout << "  - [Q]: Run QuickSort\n";
    std::cout << "  - [M]: Run MergeSort\n";
    std::cout << "  - [R]: Reshuffle Array\n\n";
    std::cout << "Press [2] -> Switch to Pathfinding Layout\n";
    std::cout << "  - Click & Drag Mouse: Draw Obstacle Walls\n";
    std::cout << "  - [D]: Run Dijkstra Algorithm\n";
    std::cout << "  - [A]: Run A* Optimization Search\n";
    std::cout << "  - [R]: Clear / Reset Grid\n\n";
    std::cout << "Press [ESC] at any time to return to Menu context.\n";

    AlgoVisualizer visualizer;
    visualizer.run();
    return 0;
}