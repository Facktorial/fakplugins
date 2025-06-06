#include <FakPlugins/Plugins.h>
// #include <Fakstd/Utils/Common.h>
#include <iostream>

int main()
{
    const auto *const text = "PLUGINS_TEST";
    // WATCH(text);
    std::cout << text << '\n';

    // WATCH(FakPlugins::some_function());
    std::cout << FakPlugins::some_function() << '\n';

    return 0;
}
