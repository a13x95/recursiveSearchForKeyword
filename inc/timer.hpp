#include <chrono>
#include <iostream>
/// @brief Class used to measure time performance
class Timer {
public:
    /// Constructor
    Timer();

    /// Destructor
    ~Timer();

    /// Stop and calculate time that has passed.
    void stop();

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
};