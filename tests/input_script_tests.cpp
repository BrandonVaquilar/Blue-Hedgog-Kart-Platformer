#include "app/InputScript.h"
#include <sstream>
#include <stdexcept>

int main() {
    std::istringstream valid("1 1 0 -1 0\n");
    if (parseInputScript(valid, 5) != std::vector<int>{1,1,0,-1,0}) return 1;
    for (const auto* text : {"1 0", "1 0 -1 0", "1 2 0", "1 x 0", "1 0.5 0"}) {
        std::istringstream invalid(text);
        try { parseInputScript(invalid, 3); return 1; }
        catch (const std::invalid_argument&) {}
    }
}
