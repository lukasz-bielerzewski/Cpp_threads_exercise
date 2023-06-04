#include <iostream>
#include <thread>
#include <vector>
#include <cmath>
#include <stdlib.h>


/* Liczby pierwsze często znajdują się w okolicy zbioru 2n - 1, gdzie n ∈ N
Napisz program, który wykorzystując tę własność, będzie wyszukiwał liczby pierwsze.
W tym celu wygeneruj wektor - serię wartości należących do powyższego zbioru, dla n
od 2 do 63. Wykorzystaj typ liczbowy uint64_t aby zapewnić, że liczby da się poprawnie
zapisać w zmiennej.

Następnie napisz funkcję, która rozpoczynając od przekazanej do niej pojedynczej liczby,
będzie przeszukiwała zbiór liczb naturalnych w kierunku malejącym, aż do napotkania liczby
pierwszej.

Następnie wywołaj funkcję dla każdego elementu z uprzednio przygotowanego zbioru wartości
początkowych, a wyniki umieszczaj w odpowiednim miejscu w uprzednio przygotowanym wektorze.

Przenieś obliczenia do wielu wątków z wykorzystaniem std::thread.
- Wersja podstawowa: uruchom tyle wątków, ile jest liczb do znalezienia
- Wersja zaawansowana: postaraj się samodzielnie podzielić pracę w zależności od liczby
rdzeni dostępnych w komputerze

Dodaj do programu funkcje pomiaru czasu i porównaj wynik działania na jednym wątku oraz
wersji wielowątkowej. Czy widoczny jest przyrost wydajności? */


namespace
{

uint64_t find_prime_from(uint64_t n, uint64_t &temp)
    {
        // function finding first prime number going down from
        // given number and returning it by reference

        bool isPrime;

        if(n == 0 || n == 1)
        {
            return 0;
        }

        for(uint64_t i = n; i > 1; --i)
        {
            isPrime = true;

            for(uint64_t j = 2; j <= i/2; ++j)
            {
                if(i % j == 0)
                {
                    isPrime = false;
                    break;
                }
            }

            if(isPrime)
            {
                return temp = i;
            }
        }

        return temp = 0;
    }

    std::chrono::duration<double> zadanie2a(std::vector<uint64_t> &liczby)
    {
        // single thread program

        std::chrono::time_point<std::chrono::system_clock> start, end;
        std::chrono::duration<double> elapsed_time;

        std::vector<uint64_t> wyniki;

        uint64_t temp = 0;

        start = std::chrono::system_clock::now();

        for(auto &it : liczby)
        {
            find_prime_from(it, temp);
            wyniki.emplace_back(temp);
        }

        end = std::chrono::system_clock::now();

        std::cout << "Liczby pierwsze : ";

        for(auto &it : wyniki)
        {
            std::cout << it << " ";
        }

        std::cout<<std::endl;

        elapsed_time = end - start;

        return elapsed_time;
    }

    std::chrono::duration<double> zadanie2b(std::vector<uint64_t> &liczby)
    {
        // basic program using as many threads as numbers passed
        // to function

        std::chrono::time_point<std::chrono::system_clock> start, end;
        std::chrono::duration<double> elapsed_time;

        std::vector<uint64_t> wyniki;
        std::vector<std::thread> threads;

        wyniki.resize(liczby.size());

        start = std::chrono::system_clock::now();

        for(size_t i = 0; i < liczby.size(); ++i)
        {
            threads.emplace_back(find_prime_from, liczby[i], std::ref(wyniki[i]));
        }

        for(auto &it : threads)
        {
            if(it.joinable())
            {
                it.join();
            }
        }

        end = std::chrono::system_clock::now();

        std::cout << "Liczby pierwsze : ";

        for(auto &it : wyniki)
        {
            std::cout << it << " ";
        }

        std::cout<<std::endl;

        elapsed_time = end - start;

        return elapsed_time;
    }

    std::chrono::duration<double> zadanie2c(std::vector<uint64_t> &liczby)
    {
        // more advanced program which takes into account
        // number of available cores

        std::chrono::time_point<std::chrono::system_clock> start, end;
        std::chrono::duration<double> elapsed_time;

        std::vector<uint64_t> wyniki;
        std::vector<std::thread> threads;
        int cores = std::thread::hardware_concurrency();

        // finding number of input data's divisions
        int n = liczby.size() / cores ;

        // if modulo of this division isn't equal 0, then add one more
        // count that will let us have one more loop to use all input data
        if(liczby.size() % cores != 0)
        {
            n++;
        }

        wyniki.resize(liczby.size());

        start = std::chrono::system_clock::now();

        // creating threads in loop in parts based on number of cores and divisions
        for(int j = 0; j < n; ++j)
        {

            // standard start from 1 to CORES number
            for(int i = 0; i < cores; ++i)
            {
                if(static_cast<size_t>(j * cores + i) >= liczby.size())
                {
                    break;
                }

                threads.emplace_back(find_prime_from, liczby[j * cores + i], std::ref(wyniki[j * cores + i]));
            }

            for(auto &it : threads)
            {
                if(it.joinable())
                {
                    it.join();
                }
            }

            // clear threads vector so we don't have to take care of iterating on further data inside
            threads.clear();

            // next we start threading from back in assumption that first used threads have their work
            // finished and can take care of more complex work from further input data, while latest used
            // threads will end their work and take care of easier one from the beginning of this part of
            // input data; it's my idea of making sure threads have as little iddle time as possible
            // without using more advanced thread functions
            for(int i = 2 * cores - 1; i > cores - 1; --i)
            {
                if(static_cast<size_t>(j * cores + i) >= liczby.size())
                {
                    break;
                }

                threads.emplace_back(find_prime_from, liczby[j * cores + i], std::ref(wyniki[j * cores + i]));
            }

            for(auto &it : threads)
            {
                if(it.joinable())
                {
                    it.join();
                }
            }

            threads.clear();

        }

        end = std::chrono::system_clock::now();

        std::cout << "Liczby pierwsze : ";

        for(auto &it : wyniki)
        {
            std::cout << it << " ";
        }

        std::cout<<std::endl;

        elapsed_time = end - start;

        return elapsed_time;
    }

    int zadanie2()
    {
        // context menu which also compares run times
        std::vector<uint64_t> liczby;

        for(int i = 2; i <= 30; ++i)
        {
            liczby.emplace_back(std::pow(2, i) - 1);
        }

        int choice;
        std::chrono::duration<double> elapsed_time1;
        std::chrono::duration<double> elapsed_time2;
        std::chrono::duration<double> elapsed_time3;
        double perc1;
        double perc2;
        char quit = ' ';

        system("CLS");
        std::cout << "Choose which part of the exersize you want to run:" << std::endl;
        std::cout << "1. Single thread prime numbers finding" << std::endl;
        std::cout << "2. Multi-thread prime numbers finding" << std::endl;
        std::cout << "3. Optimized multi-thread prime numbers finding" << std::endl;
        std::cout << "Writing more then one number at once will let you compare their run times:" << std::endl;
        std::cin >> choice;
        system("CLS");

        while( choice != 1 && choice != 2 && choice != 3
            && choice != 12 && choice != 21 && choice != 13 && choice != 31
            && choice != 23 && choice != 32 && choice != 123 && choice != 132
            && choice != 213 && choice != 231 && choice != 312 && choice != 321)
        {
            std::cout << "Wrong number! Try again:";
            std::cin >> choice;

            system("CLS");
        }

        switch(choice)
        {
        case 1:
        {
            elapsed_time1 = zadanie2a(liczby);
            std::cout << "Czas wykonania zadania 1: " << elapsed_time1.count() << std::endl;
            std::cout << std::endl;
        }
            break;
        case 2:
        {
            elapsed_time2 = zadanie2b(liczby);
            std::cout << "Czas wykonania zadania 2: " << elapsed_time2.count() << std::endl;
            std::cout << std::endl;
        }
            break;
        case 3:
        {
            elapsed_time3 = zadanie2c(liczby);
            std::cout << "Czas wykonania zadania 3: " << elapsed_time3.count() << std::endl;
            std::cout << std::endl;
        }
            break;
        case 12:
        {
            elapsed_time1 = zadanie2a(liczby);
            elapsed_time2 = zadanie2b(liczby);
            perc1 = elapsed_time1.count() / elapsed_time2.count() * 100;
            std::cout << "Czas wykonania zadania 1: " << elapsed_time1.count() << std::endl;
            std::cout << "Czas wykonania zadania 2: " << elapsed_time2.count() << std::endl;
            std::cout << "Zadanie 1 zostalo wykonane w " << perc1 << "% czasu trwania zadania 2" << std::endl;
            std::cout << std::endl;
        }
            break;
        case 21:
        {
            elapsed_time1 = zadanie2a(liczby);
            elapsed_time2 = zadanie2b(liczby);
            perc1 = elapsed_time2.count() / elapsed_time1.count() * 100;
            std::cout << "Czas wykonania zadania 1: " << elapsed_time1.count() << std::endl;
            std::cout << "Czas wykonania zadania 2: " << elapsed_time2.count() << std::endl;
            std::cout << "Zadanie 2 zostalo wykonane w " << perc1 << "% czasu trwania zadania 1" << std::endl;
            std::cout << std::endl;
        }
            break;
        case 13:
        {
            elapsed_time1 = zadanie2a(liczby);
            elapsed_time3 = zadanie2c(liczby);
            perc1 = elapsed_time1.count() / elapsed_time3.count() * 100;
            std::cout << "Czas wykonania zadania 1: " << elapsed_time1.count() << std::endl;
            std::cout << "Czas wykonania zadania 3: " << elapsed_time3.count() << std::endl;
            std::cout << "Zadanie 1 zostalo wykonane w " << perc1 << "% czasu trwania zadania 3" << std::endl;
            std::cout << std::endl;
        }
            break;
        case 31:
        {
            elapsed_time1 = zadanie2a(liczby);
            elapsed_time3 = zadanie2c(liczby);
            perc1 = elapsed_time3.count() / elapsed_time1.count() * 100;
            std::cout << "Czas wykonania zadania 1: " << elapsed_time1.count() << std::endl;
            std::cout << "Czas wykonania zadania 3: " << elapsed_time3.count() << std::endl;
            std::cout << "Zadanie 3 zostalo wykonane w " << perc1 << "% czasu trwania zadania 1" << std::endl;
            std::cout << std::endl;
        }
            break;
        case 23:
        {
            elapsed_time2 = zadanie2b(liczby);
            elapsed_time3 = zadanie2c(liczby);
            perc1 = elapsed_time2.count() / elapsed_time3.count() * 100;
            std::cout << "Czas wykonania zadania 2: " << elapsed_time2.count() << std::endl;
            std::cout << "Czas wykonania zadania 3: " << elapsed_time3.count() << std::endl;
            std::cout << "Zadanie 2 zostalo wykonane w " << perc1 << "% czasu trwania zadania 3" << std::endl;
            std::cout << std::endl;
        }
            break;
        case 32:
        {
            elapsed_time2 = zadanie2b(liczby);
            elapsed_time3 = zadanie2c(liczby);
            perc1 = elapsed_time3.count() / elapsed_time2.count() * 100;
            std::cout << "Czas wykonania zadania 2: " << elapsed_time2.count() << std::endl;
            std::cout << "Czas wykonania zadania 3: " << elapsed_time3.count() << std::endl;
            std::cout << "Zadanie 3 zostalo wykonane w " << perc1 << "% czasu trwania zadania 2" << std::endl;
            std::cout << std::endl;
        }
            break;
        case 123:
        {
            elapsed_time1 = zadanie2a(liczby);
            elapsed_time2 = zadanie2b(liczby);
            elapsed_time3 = zadanie2c(liczby);
            perc1 = elapsed_time1.count() / elapsed_time2.count() * 100;
            perc2 = elapsed_time1.count() / elapsed_time3.count() * 100;
            std::cout << "Czas wykonania zadania 1: " << elapsed_time1.count() << std::endl;
            std::cout << "Czas wykonania zadania 2: " << elapsed_time2.count() << std::endl;
            std::cout << "Czas wykonania zadania 3: " << elapsed_time3.count() << std::endl;
            std::cout << "Zadanie 1 zostalo wykonane w " << perc1 << "% czasu trwania zadania 2" << std::endl;
            std::cout << "i w " << perc2 << "% czasu trwania zadania 3" << std::endl;
            std::cout << std::endl;
        }
            break;
        case 132:
        {
            elapsed_time1 = zadanie2a(liczby);
            elapsed_time2 = zadanie2b(liczby);
            elapsed_time3 = zadanie2c(liczby);
            perc1 = elapsed_time1.count() / elapsed_time2.count() * 100;
            perc2 = elapsed_time1.count() / elapsed_time3.count() * 100;
            std::cout << "Czas wykonania zadania 1: " << elapsed_time1.count() << std::endl;
            std::cout << "Czas wykonania zadania 2: " << elapsed_time2.count() << std::endl;
            std::cout << "Czas wykonania zadania 3: " << elapsed_time3.count() << std::endl;
            std::cout << "Zadanie 1 zostalo wykonane w " << perc1 << "% czasu trwania zadania 2" << std::endl;
            std::cout << "i w " << perc2 << "% czasu trwania zadania 3" << std::endl;
            std::cout << std::endl;
        }
            break;
        case 213:
        {
            elapsed_time1 = zadanie2a(liczby);
            elapsed_time2 = zadanie2b(liczby);
            elapsed_time3 = zadanie2c(liczby);
            perc1 = elapsed_time2.count() / elapsed_time1.count() * 100;
            perc2 = elapsed_time2.count() / elapsed_time3.count() * 100;
            std::cout << "Czas wykonania zadania 1: " << elapsed_time1.count() << std::endl;
            std::cout << "Czas wykonania zadania 2: " << elapsed_time2.count() << std::endl;
            std::cout << "Czas wykonania zadania 3: " << elapsed_time3.count() << std::endl;
            std::cout << "Zadanie 2 zostalo wykonane w " << perc1 << "% czasu trwania zadania 1" << std::endl;
            std::cout << "i w " << perc2 << "% czasu trwania zadania 3" << std::endl;
            std::cout << std::endl;
        }
            break;
        case 231:
        {
            elapsed_time1 = zadanie2a(liczby);
            elapsed_time2 = zadanie2b(liczby);
            elapsed_time3 = zadanie2c(liczby);
            perc1 = elapsed_time2.count() / elapsed_time1.count() * 100;
            perc2 = elapsed_time2.count() / elapsed_time3.count() * 100;
            std::cout << "Czas wykonania zadania 1: " << elapsed_time1.count() << std::endl;
            std::cout << "Czas wykonania zadania 2: " << elapsed_time2.count() << std::endl;
            std::cout << "Czas wykonania zadania 3: " << elapsed_time3.count() << std::endl;
            std::cout << "Zadanie 2 zostalo wykonane w " << perc1 << "% czasu trwania zadania 1" << std::endl;
            std::cout << "i w " << perc2 << "% czasu trwania zadania 3" << std::endl;
            std::cout << std::endl;
        }
            break;
        case 312:
        {
            elapsed_time1 = zadanie2a(liczby);
            elapsed_time2 = zadanie2b(liczby);
            elapsed_time3 = zadanie2c(liczby);
            perc1 = elapsed_time3.count() / elapsed_time1.count() * 100;
            perc2 = elapsed_time3.count() / elapsed_time2.count() * 100;
            std::cout << "Czas wykonania zadania 1: " << elapsed_time1.count() << std::endl;
            std::cout << "Czas wykonania zadania 2: " << elapsed_time2.count() << std::endl;
            std::cout << "Czas wykonania zadania 3: " << elapsed_time3.count() << std::endl;
            std::cout << "Zadanie 3 zostalo wykonane w " << perc1 << "% czasu trwania zadania 1" << std::endl;
            std::cout << "i w " << perc2 << "% czasu trwania zadania 2" << std::endl;
            std::cout << std::endl;
        }
            break;
        case 321:
        {
            elapsed_time1 = zadanie2a(liczby);
            elapsed_time2 = zadanie2b(liczby);
            elapsed_time3 = zadanie2c(liczby);
            perc1 = elapsed_time3.count() / elapsed_time1.count() * 100;
            perc2 = elapsed_time3.count() / elapsed_time2.count() * 100;
            std::cout << "Czas wykonania zadania 1: " << elapsed_time1.count() << std::endl;
            std::cout << "Czas wykonania zadania 2: " << elapsed_time2.count() << std::endl;
            std::cout << "Czas wykonania zadania 3: " << elapsed_time3.count() << std::endl;
            std::cout << "Zadanie 3 zostalo wykonane w " << perc1 << "% czasu trwania zadania 1" << std::endl;
            std::cout << "i w " << perc2 << "% czasu trwania zadania 2" << std::endl;
            std::cout << std::endl;
        }
            break;
        }

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
