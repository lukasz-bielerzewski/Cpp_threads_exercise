#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <stdlib.h>
#include <mutex>


/* Zastanów się, które operacje w Zadaniu 3 powinny należeć do sekcji
 * krytycznej. Stwórz muteks, który będzie odpowiadał za kontrolę dostępu
 * do zwiększanej zmiennej. Zmodyfikuj program tak, aby działał poprawnie. */


namespace
{

int licznik(std::mutex &mutex)
    {
        std::vector<std::thread> threads;
        int num = 0;

        for(int i = 0; i < 1000; ++i)
        {
            threads.emplace_back([&num , &mutex](){ std::this_thread::sleep_for(std::chrono::milliseconds(10));
                                                           std::lock_guard<std::mutex> lk_guard(mutex);
                                                           num += 100; });
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

    int zadanie4()
    {
        system("CLS");

        int liczba;

        std::cout << "Podaj liczbe inkrementowanych liczb calkowitych: ";
        std::cin >> liczba;

        system("CLS");

        std::cout << "Wyniki:" << std::endl;

        for(int i = 0; i < liczba; ++i)
        {
            std::mutex mutex;
            std::cout << i + 1 << ". " << licznik(mutex) << std::endl;
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
