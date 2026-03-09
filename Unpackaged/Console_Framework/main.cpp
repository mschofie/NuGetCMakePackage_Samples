#include <iostream>
#include <utility>
#include <winrt/base.h>
#include <winrt/Microsoft.Windows.System.Power.h>

namespace power = winrt::Microsoft::Windows::System::Power;

int wmain(int argc, wchar_t* argv[]) noexcept
{
    std::ignore = argc;
    std::ignore = argv;

    try
    {
        winrt::init_apartment();

        power::PowerSupplyStatus powerSupplyStatus = power::PowerManager::PowerSupplyStatus();

        std::cout << "powerSupplyStatus: " << static_cast<int>(powerSupplyStatus) << "\n";
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Error: " << ex.what() << "\n";
        return -1;
    }

    return 0;
}
