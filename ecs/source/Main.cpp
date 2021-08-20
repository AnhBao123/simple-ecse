#include<exception>
#include<iostream>
#include<ctime>

#include<SFML/Graphics.hpp>

#include "gui/Application.hpp"

int main()
{
    try
    {
        Application app;
        app.run();
        return 0;
    }
    catch(std::exception& e)
    {
        return -1;
        std::cerr << e.what() << "\n";
    }
    catch(...)
    {
        return -2;
    }
}
