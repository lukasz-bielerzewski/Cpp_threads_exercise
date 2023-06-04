#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <stdlib.h>


/* Stwórz w programie zmienną całkowitą. Utwórz 1000 wątków, z których każdy
 * odczeka 10 milisekund, a następnie zwiększy wartość stworzonej zmiennej o 100.
 * Poczekaj na zakończenie wszystkich wątków i sprawdź wynik dodawania. Uruchom
 * program kilkukrotnie. */


namespace
{

    int licznik()
    {
        std::vector<std::thread> threads;
        int num = 0;

        for(int i = 0; i < 1000; ++i)
        {
            threads.emplace_back([&num](){ std::this_thread::sleep_for(std::chrono::milliseconds(10));
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

    int zadanie3()
    {
        system("CLS");

        int liczba;

        std::cout << "Podaj liczbe inkrementowanych liczb calkowitych: ";
        std::cin >> liczba;

        system("CLS");

        std::cout << "Wyniki:" << std::endl;

        for(int i = 0; i < liczba; ++i)
        {
            std::cout << i + 1 << ". " << licznik() << std::endl;
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
