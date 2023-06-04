#include <iostream>
#include <thread>
#include <vector>
#include <stdlib.h>


/* Zmodyfikuj pierwszy przykład tak, aby uruchamiał podaną przez użytkownika liczbę wątków,
 * wykonujących jedną funkcję.Funkcja ma wyświetlić w konsoli wiadomość zawierającą unikalne
 * ID wątku. ID można pobrać z poziomu wątku dzięki funkcji std::this_thread::get_id().
 * Zastanów się jak możesz wygodnie przechowywać informacje o kolekcji wątków i czekać
 * na zakończenie wszystkich. */


namespace
{

    void foo()
    {
        // function returning thread's id
        std::cerr << "ID watku: " << std::this_thread::get_id() << std::endl;
    }

    int zadanie1()
    {
        std::vector<std::thread> collection;
        int liczba;

        system("CLS");

        std::cout << "Podaj liczbe watkow do uruchomienia: ";
        std::cin >> liczba;

        //creating threads
        for(int i = 0; i < liczba; ++i)
        {
            collection.push_back(std::thread(foo));
        }

        std::cerr << "threads started" << std::endl;

        // wait for threads to finish:
        for(int i = 0; i < liczba; ++i)
        {
            collection[i].join();
        }

        std::cerr << "threads finished" << std::endl;

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
