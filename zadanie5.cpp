#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <stdlib.h>
#include <atomic>


/* Zastanów się, które zmienne w Zadaniu 3 powinny być atomowe,
 * aby uniknąć wyścigu danych. Stwórz odpowiednią zmienną atomową,
 * która umożliwi poprawne działanie programu. */


namespace
{

    void atomic_operation(std::atomic<int>& num)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        num += 100;
    }

    int licznik5()
    {
        std::vector<std::thread> threads;
        std::atomic<int> num = 0;

        for(int i = 0; i < 1000; ++i)
        {
            threads.emplace_back(atomic_operation, std::ref(num));
        }

        for(auto &it : threads)
        {
            if(it.joinable())
            {
                it.join();
            }
        }

        return num;
    }

    int zadanie5()
    {
        system("CLS");

        int liczba;

        std::cout << "Podaj liczbe inkrementowanych liczb calkowitych: ";
        std::cin >> liczba;

        system("CLS");

        std::cout << "Wyniki:" << std::endl;

        for(int i = 0; i < liczba; ++i)
        {
            std::cout << i + 1 << ". " << licznik5() << std::endl;
        }

        char quit = ' ';

        std::cout << std::endl;
        std::cout << "Press q/Q to go back to menu: ";
        std::cin >> quit;

        while(quit != 'q' && quit != 'Q')
        {
            std::cin >> quit;
        }

        system("CLS");

        if(quit == 'q' || quit == 'Q')
        {
            return 0;
        }

        return 1;
    }

}
