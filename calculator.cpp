#include <iostream>
#include <limits>
#include <gmpxx.h>   // CLion не видит её, ну и ладно, хотя-бы работает

const char* version = "1.0";
signed int main(const signed int argc, char* argv[]) {
    setlocale(LC_ALL, "RU");
    if(argc == 1) {
        std::cout << "Использование: " << argv[0] << " <операция(напишите \"" << argv[0] << " --help\" для информации о них)> <числа(можно написать много если выбрана операция +, / или *, иначе же можно написать только 2 числа)>" << std::endl;
    } else if (argc == 2) {
        if(strcmp(argv[1], "--help") == 0) {
            std::cout << "Операторы:\n"
                      << "'+' — оператор сумы(от 2-х чисел включительно)\n"
                      << "'-' — оператор вычитания(от 2-х чисел включительно, последовательно)\n"
                      << "'/' — оператор деления(от 2-х чисел включительно, последовательно)\n"
                      << "'*' — оператор сумы умножения(от 2-х чисел)\n"
                      << "'%' — оператор получения остатка деления(только 2 числа)\n"
                      << "'/%' — оператор деления и получения остатка(на следующей строке, только 2 числа)\n"
                      << "'**' или '^' — оператор вычисления степени чисел(только 2 числа, 1-е — само число, 2-е — его степень)\n"
                      << "'//' — оператор целочисленного деления(оператор '/' возвращает число с точкой)\n"
                      << "'~' — оператор вычисления среднего арифметического(от 2-х чисел)\n"
                      << "'!' — оператор вычисления факториала числа (от 2-х чисел)"
            <<
            std::endl;
        } else if(strcmp(argv[1], "--version") == 0) {
            std::cout << version << std::endl;
        }
        return 0;
    } else if(argc == 3) {
        if(*argv[1] == '!') {
            signed int x;
            try {
                x = std::stoi(argv[2]);
            } catch (const std::invalid_argument&) {
                std::cerr << "не является числом: " << argv[2] << std::endl;
                return(1);
            } catch (const std::out_of_range&) {
                const bool cond = argv[2][0] == '-';   // просто подставится на место всех его использований на этапе компиляции(наверное)
                std::cerr << "слишком "
                          << (cond ? "маленькое" : "большое")
                          << " число: "
                          << argv[2]
                          << '('
                          << (cond ? "минимально" : "максимально")
                          << " доступное число это "
                          << (cond ? std::numeric_limits<signed long long int>::min() : std::numeric_limits<signed long long int>::max())
                          << ')'
                          <<
                std::endl;

                return(1);
            }
            if(x < 0) {
                mpz_class result(-1);
                for(int i = -1; i >= x; i--) {
                    result *= i;
                }
                std::cout << result << std::endl;
                std::cout << sizeof(result) << std::endl;
            } else if(x == 0) std::cout << 1 << std::endl;
            else {
                mpz_class result(1);
                for(int i = 1; i <= x; i++) {
                    result *= i;
                }
                std::cout << result << std::endl;
            }
            return(0);
        }
    } else {
        if(const std::string signs = "~+*-/%^!"; signs.contains(argv[1]) || (argv[1][0] == '*' && argv[1][1] == '*') || (argv[1][0] == '/' && (argv[1][1] == '/' || argv[1][1] == '%'))) {
            const signed short int size = argc-2;
            auto* nums = new signed long long int[size];   // усе эти std::vector'ы для нубов, да и тут он бессмыслен
            auto clean = [&]->void{delete[] nums; nums = nullptr;};
            for (int i = 2; i < argc; i++) {
                try {
                    nums[i-2] = std::stoll(argv[i]);
                } catch (const std::invalid_argument&) {
                    std::cerr << "не является числом: " << argv[i] << std::endl;
                    clean();
                    return(1);
                } catch (const std::out_of_range&) {
                    const bool cond = argv[i][0] == '-';   // просто подставится на место всех его использований на этапе компиляции(наверное)
                    std::cerr << "слишком "
                              << (cond ? "маленькое" : "большое")
                              << " число: "
                              << argv[i]
                              << '('
                              << (cond ? "минимально" : "максимально")
                              << " доступное число это "
                              << (cond ? std::numeric_limits<signed long long int>::min() : std::numeric_limits<signed long long int>::max())
                              << ')'
                              <<
                    std::endl;

                    clean();
                    return(1);
                }
            }
            // std::cout << "List of numbers: "; for(int i = 0; i < size; i++) std::cout << nums[i] << ((i < size - 1) ? ", " : "");
            // std::cout << std::endl;

            auto argveq = [&](const char* s){return(strcmp(argv[1], s) == 0);};
            if(argveq("//")) {
                if(argc > 4) {
                    signed int result = nums[0];
                    for(signed short int i = 1; i < size; i++) result /= nums[i];
                    std::cout << result << std::endl;
                } else
                    std::cout << (nums[0] / nums[1]) << std::endl;
            } else if(argveq("/%")) {
                if(argc > 4) {
                    std::cerr << argv[0] << ": нельзя получить остаток от деления больше чем 2-х чисел";
                    clean();
                    return(1);
                } else {
                    std::cout << (static_cast<double>(nums[0])/static_cast<double>(nums[1])) << '\n' << std::fmod(static_cast<double>(nums[0]), static_cast<double>(nums[1])) << std::endl;
                }
            } else if(argveq("**")) {
                goto power;
            } else {
                switch(*(argv[1])) {   // if-else ветка бессмысленно, так что буду использовать nintendo switch (жаль, что тут нету Steam Deck'а)
                    case '+': {
                        mpz_class result(std::to_string(nums[0]));
                        for(signed short int i = 1; i < size; i++) result += mpz_class(std::to_string(nums[i]));
                        std::cout << result << std::endl;
                        break;
                    }
                    case '*': {
                        if(argc > 4) {
                            mpz_class result(std::to_string(nums[0]));
                            for(signed short int i = 1; i < size; i++) result = result * mpz_class(std::to_string(nums[i]));
                            std::cout << result << std::endl;
                        } else {
                            std::cout << (mpz_class(std::to_string(nums[0])) * mpz_class(std::to_string(nums[1]))) << std::endl;
                        }
                        break;
                    }
                    case '/': {
                        if(argc > 4) {
                            double result = nums[0];
                            for (int i = 1; i < size; i++) {
                                if(nums[i] != 0)
                                    result /= static_cast<double>(nums[i]);
                                else
                                    goto zerodiv;
                            }
                            std::cout << result << std::endl;
                        } else {
                            if(nums[1] != 0)
                                std::cout << static_cast<double>(nums[0]) / static_cast<double>(nums[1]) << std::endl;
                            else {
                                zerodiv:
                                std::cerr << "деление на 0 запрещено!" << std::endl;
                                clean();
                                return(1);
                            }
                        }
                        break;
                    }
                    case '%': {
                        if(argc > 4) {
                            std::cerr << argv[0] << ": нельзя получить остаток от деления больше чем 2-х чисел";
                            clean();
                            return(1);
                        }
                        if(nums[1] != 0)
                        std::cout << nums[0] % nums[1] << std::endl;
                        else goto zerodiv;
                        break;
                    }
                    case '-': {
                        if(argc > 4) {
                            signed long long int result = nums[0];
                            for(signed int i = 1; i < size; i++) result -= nums[i];
                            std::cout << result << std::endl;
                        } else
                            std::cout << nums[0] - nums[1] << std::endl;
                        break;
                    }
                    case '!': {
                        for (int i = 1; i < size; i++) {
                            if(const signed long long int x = nums[i]; x < 0) {
                                mpz_class result(std::to_string(-1));
                                for(int j = -1; j >= x; j--) {
                                    result *= j;
                                }
                                std::cout << result << std::endl;
                            } else if(x == 0) std::cout << 1 << std::endl;
                            else {
                                mpz_class result('1');
                                for(int j = 1; j <= x; j++) {
                                    result *= j;
                                }
                                std::cout << result << std::endl;
                            }
                        }
                        break;
                    }
                    case '^': {
                        power:
                        if(argc > 4) {
                            std::cerr << argv[0] << ": слишком много аргументов(для степени надо 2 числа!)";
                            clean();
                            return(1);
                        }
                        if(nums[1] != 0) {
                            mpz_class result(std::to_string(nums[0]));
                            for(int i = 1; i < nums[1]; ++i) result *= mpz_class(std::to_string(nums[0]));
                            std::cout << result << std::endl;
                        } else {
                            std::cout << 1 << std::endl;
                        }
                        break;
                    }
                    case '~': {
                        mpz_class sum = 0;
                        for(size_t i = 0; i < size; ++i) sum+= mpz_class(std::to_string(nums[i]));
                        std::cout << sum / size << std::endl;

                        break;
                    }
                    default: {
                        clean();
                        goto incarg;
                    }
                }
            }
            clean();
        } else {
            incarg:
            std::cerr << argv[0] << ": 1м аргументом должно быть: '+', '-', '/', '%', '**', '^', '/%', '//', или '*'. Напишите \"" << argv[0] << " --help\" для получения дополнительной информации об операторах" << std::endl;
            return(1);
        }
    };

    return(0);
}