// AUTHOR OF THIS HEADER: Taylo <github> : https://github.com/i-Taylo
#ifndef PROGRESSMONITOR_H
#define PROGRESSMONITOR_H

#include <cstdlib>
#include <iostream>
#include <functional>
#include <vector>
#include <chrono>
#include <thread>
#include <iomanip>
#include <sstream>
#include <cstdarg>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <termio.h>
#include <unistd.h> 
#include <sys/ioctl.h> 
#include <cmath> 
#include <array>

class ProgressMonitor {
public:
    using Function = std::function<std::string()>;
    using FunctionName = std::string;
    enum AbortT {EXIT, RETURN};
    enum Colors {RED, GREEN, BLUE, YELLOW, ORANGE, PURPLE, PINK, CYAN, MAGENTA, BROWN, BLACK, WHITE, RESET};
    enum ColorTarget {FILL, LEAD, START, END, FUNC_NAME};

    static void setFunction(Function func, FunctionName funcName) {
        functions.push_back(func);
        functionNames.push_back(funcName);
    }

    static void startMonitoring() {
        auto start = std::chrono::high_resolution_clock::now();
        int totalDuration = getTotalDuration();
        int currentProgress = 0;
        int numFunctionsExecuted = 0;

        while (numFunctionsExecuted < functions.size()) {
            for (size_t i = 0; i < functions.size(); ++i) {
                if (numFunctionsExecuted >= functions.size()) break;

                std::string output = functions[i]();
                std::stringstream outputStream(output);
                std::string line;

                int linesProcessed = 0;
                if (CaptureOutput) {
                    while (std::getline(outputStream, line, '\n')) {
                        displayProgressBar(currentProgress, totalDuration, functionNames[i], outputStream.eof() && i == functions.size() - 1, line);
                        std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime)); // Use sleepTime variable
                        ++linesProcessed;
                    }
                } else {
                    displayProgressBar(currentProgress, totalDuration, functionNames[i], outputStream.eof() && i == functions.size() - 1, line);
                    std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime)); // Use sleepTime variable
                    ++linesProcessed;
                }

                int functionProgress = (totalDuration / functions.size());
                currentProgress += functionProgress;

                currentProgress = std::min(currentProgress, totalDuration);

                if (currentProgress >= totalDuration && i == functions.size() - 1) {
                    // Ensure the last function reaches 100%
                    displayProgressBar(totalDuration, totalDuration, functionNames[i], true, "");
                    ++numFunctionsExecuted;
                } else {
                    ++numFunctionsExecuted;
                }
            }
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();

        displaySummary(numFunctionsExecuted, duration);
    }

    static void setSleep(int time) {
        sleepTime = time;
    }

    static auto setFinishText(const char* fmt, ...) -> void {
        finishText = "\n" + std::string(fmt);
    }

    // Set new bar width
    static auto setBarWidth(const int daWidth) -> int {
        return _BarWidth = daWidth;
    }
    // set the bar start symbol defualt is `[`
    static auto setBarStartSymbol(const std::string& symbol) -> std::string {
        return barStartSymbol = symbol;
    }
    // set the bar end symbol defualt is `]`
    static auto setBarEndSymbol(const std::string& symbol) -> std::string {
        return barEndSymbol = symbol;
    }
    // set the bar fill symbol defualt is `-▶`
    static auto setBarFillSymbol(const std::string& symbol) -> std::string {
        return barFillSymbol = symbol;
    }
    // set the bar fill symbol defualt is `▇`
    static auto setBarLeadSymbol(const std::string& symbol) -> std::string {
        return barLeadSymbol = symbol;
    }
    // Coloring function 
    static auto setColorTo(ColorTarget target, Colors color) -> std::string {
        std::string colorCode;
        if (isTerminal()) {
            switch (color) {
            case RED:
                colorCode = "\033[0;31m";
                break;
            case GREEN:
                colorCode = "\033[0;32m";
                break;
            case BLUE:
                colorCode = "\033[0;34m";
                break;
            case YELLOW:
                colorCode = "\033[0;33m";
                break;
            case ORANGE:
                colorCode = "\033[0;91m";
                break;
            case PURPLE:
                colorCode = "\033[0;35m";
                break;
            case PINK:
                colorCode = "\033[0;95m";
                break;
            case CYAN:
                colorCode = "\033[0;36m";
                break;
            case MAGENTA:
                colorCode = "\033[0;95m";
                break;
            case BROWN:
                colorCode = "\033[0;33m";
                break;
            case BLACK:
                colorCode = "\033[0;30m";
                break;
            case WHITE:
                colorCode = "\033[0;37m";
                break;
            case RESET:
                colorCode = "\033[0m";
                break;
            default:
                break;
            }
        }
        switch (target) {
        case FUNC_NAME:
            isFunname = true;
            funname_color = colorCode;
            break;
        case FILL:
            isFill = true;
            barFillSymbol_color = colorCode;
            break;
        case LEAD:
            isLead = true;
            barLeadSymbol_color = colorCode;
            break;
        case START:
            isStart = true;
            barStartSymbol_color = colorCode;
            break;
        case END:
            isEnd = true;
            barEndSymbol_color = colorCode;
            break;
        }
        hasColor = true;
        return colorCode;
    }
    static auto captureFunctionsOutput(bool _CaptureOutput) -> bool {
        return CaptureOutput = _CaptureOutput;
    }
    // abort function
    static int abort(AbortT abortType = RETURN, const char* reason = "", ...) {
        std::va_list args;
        va_start(args, reason);

        std::stringstream errorMessage;
        errorMessage << "\nError: " << reason << "\n";
        std::vfprintf(stderr, errorMessage.str().c_str(), args);
        va_end(args);

        if (abortType == EXIT) {
            std::exit(EXIT_FAILURE);
        }

        return EXIT_FAILURE;
    }

    // Function to get the totol duration of the executed functions
    static auto getTotalDuration() -> int {
        int totalDuration = 0;
        for (const auto& func : functions) {
            totalDuration += 1;
        }
        return totalDuration;
    }

    // Function to execute external commands
    static std::string exec(const char* cmd, ...) {
        std::array<char, 128> buffer;
        std::string result;
        std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
        if (!pipe) {
            throw std::runtime_error("popen() failed!");
        }
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            result += buffer.data();
        }
        return result;
    }

    // Function to disable or enable finish text
    static auto showFinishSummary(const bool choice) -> bool {
        return isShowFinishSummary = choice;
    }

private:
    static std::vector<Function> functions;
    static std::vector<FunctionName> functionNames;
    static int sleepTime;
    static std::string finishText;
    static int _BarWidth;
    static std::string barStartSymbol;
    static std::string barEndSymbol;
    static std::string barFillSymbol;
    static std::string barLeadSymbol;
    static std::string mColor;
    static bool hasColor;
    static bool isLead;
    static bool isFill;
    static bool isStart;
    static bool isEnd;
    static bool isShowFinishSummary;
    static std::string barStartSymbol_color;
    static std::string barEndSymbol_color;
    static std::string barFillSymbol_color;
    static std::string barLeadSymbol_color;
    static bool isFunname;
    static std::string funname_color;
    static bool CaptureOutput;

    static bool isTerminal() {
        return isatty(STDOUT_FILENO) != 0;
    }
    static std::string alignText(const std::string& text, const std::string& alignment = "DEFAULT") {
        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w); // Get terminal size

        int terminalWidth = w.ws_col;

        if (alignment == "CENTER") {
            int padding = std::floor((terminalWidth - text.length()) / 2.0);
            return std::string(padding, ' ') + text + "\n";
        } else {
            return text + "\n";
        }
    }
    static void displayProgressBar(int current, int total, const FunctionName& functionName, bool isLast, const std::string& output) {
        const int barWidth = _BarWidth;

        // Calculate the progress percentage for the current function
        int progress = static_cast<int>(static_cast<float>(current) / total * 100);

        // Limit progress to 100%
        progress = std::min(progress, 100);

        // Clear the entire line before printing the new progress bar
        // I used !isTerminal because it will not clear the line instead it will show `[2K` 
        // a newline will be good for non-terminal.
        isTerminal() ? std::cout << "\033[2K\r": std::cout << "\n";

        std::string progressBar = isStart ? isTerminal() ? barStartSymbol_color + barStartSymbol + "\033[0m": barStartSymbol: barStartSymbol;
        int pos = static_cast<int>(barWidth * progress / 100);
        for (int i = 0; i < barWidth; ++i) {
            if (i < pos) progressBar += isFill ? isTerminal() ? barFillSymbol_color + barFillSymbol + "\033[0m": barFillSymbol: barFillSymbol;
            else if (i == pos) progressBar += isLead ? isTerminal() ? barLeadSymbol_color + barLeadSymbol + "\033[0m": barLeadSymbol: barLeadSymbol;
            else progressBar += " ";
        }
        progressBar += isEnd ? isTerminal() ? barEndSymbol_color + barEndSymbol + "\033[0m": barEndSymbol: barEndSymbol + " ";
        std::string coloredFunctionName = isFunname ? isTerminal() ? funname_color + functionName + "\033[0m" : functionName: functionName;

        std::cout << std::left << std::setw(3) << coloredFunctionName << " ";
        std::cout << progressBar << std::right << std::setw(3) << progress << "% " << output << std::flush;
        // std::cout << std::left << std::setw(3) << coloredFunctionName << " ";
        // std::cout << progressBar << std::right << std::setw(3) << progress << "% " << output << std::flush;

        if (isLast && progress < 100) {
            std::cout << std::endl; // Print a newline if it's the last progress line and not yet 100%
        }
    }

    static void displaySummary(int numFunctions, int duration) {
        if (isShowFinishSummary) {
            if (finishText.empty()) {
                std::cout << "\nFinished " << numFunctions << " functions in " << duration << " seconds." << std::endl;
            } else {
                std::cout << finishText << std::endl;
            }        
        } 
    }

};

std::vector<ProgressMonitor::Function> ProgressMonitor::functions;
std::vector<ProgressMonitor::FunctionName> ProgressMonitor::functionNames;
int ProgressMonitor::sleepTime = 0; // Default sleep time is 1 second
int ProgressMonitor::_BarWidth = 30; // Default bar width to 30
std::string ProgressMonitor::finishText;
std::string ProgressMonitor::barStartSymbol = "["; //defualt
std::string ProgressMonitor::barEndSymbol = "]"; //defualt
std::string ProgressMonitor::barFillSymbol = "▇"; //defualt
std::string ProgressMonitor::barLeadSymbol = "-▶"; //defualt
bool ProgressMonitor::hasColor = false;
std::string ProgressMonitor::mColor;
bool ProgressMonitor::isLead = false;
bool ProgressMonitor::isFill = false;
bool ProgressMonitor::isStart = false;
bool ProgressMonitor::isEnd = false;
bool ProgressMonitor::isFunname = false;
bool ProgressMonitor::CaptureOutput = true;
bool ProgressMonitor::isShowFinishSummary = true;
std::string ProgressMonitor::barStartSymbol_color;
std::string ProgressMonitor::barEndSymbol_color;
std::string ProgressMonitor::barFillSymbol_color;
std::string ProgressMonitor::barLeadSymbol_color;
std::string ProgressMonitor::funname_color;

#endif // PROGRESSMONITOR_H
