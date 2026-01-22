#include <iostream>
#include <limits>
#include <gmpxx.h>   // CLion doesn't see this:(

const char* version = "1.0";
signed int main(const signed int argc, char* argv[]) {
    setlocale(LC_ALL, "EN");
    if(argc == 1) {
        std::cout << "Using: " << argv[0] << " <operation(write \"" << argv[0] << " --help\" for get some help)> <numbers(write " << argv[0] << " --help to get some information about allowed number of numbers)>" << std::endl;
    } else if (argc == 2) {
        if(strcmp(argv[1], "--help") == 0) {
            std::cout << "Operators:\n"
                      << "'+' — sum operator(from 2 numbers inclusive)\n"
                      << "'-' — minus operator(from 2 numbers inclusive, sequentially)\n"
                      << "'/' — division operator(from 2 numbers inclusive, sequentially)\n"
                      << "'*' — multiplication operator(from 2 numbers inclusive)\n"
                      << "'%' — getting a remainder from division operator(only 2 numbers)\n"
                      << "'/%' — division and getting а remainder from division operator(on a next line, only 2 numbers)\n"
                      << "'**' or '^' — pow operator(only 2 numbers, 1-е — a number, 2-е — a power)\n"
                      << "'//' — getting integer result of division operator(an operator '/' returns floating-point number)\n"
                      << "'~' — arithmetic mean operator(from 2 numbers inclusive)\n"
                      << "'!' — factorial operator(from 1 number inclusive)"
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
                std::cerr << "is not a number: " << argv[2] << std::endl;
                return(1);
            } catch (const std::out_of_range&) {
                const bool cond = argv[2][0] == '-';
                std::cerr << "too "
                          << (cond ? "small" : "big")
                          << " number: "
                          << argv[2]
                          << '('
                          << (cond ? "minimum" : "maximum")
                          << " available number "
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
            auto* nums = new signed long long int[size];   // std::vectors are for noobs, there is no point in using it here
            auto clean = [&]->void{delete[] nums; nums = nullptr;};
            for (int i = 2; i < argc; i++) {
                try {
                    nums[i-2] = std::stoll(argv[i]);
                } catch (const std::invalid_argument&) {
                    std::cerr << "is not a number: " << argv[i] << std::endl;
                    clean();
                    return(1);
                } catch (const std::out_of_range&) {
                    const bool cond = argv[i][0] == '-';
                    std::cerr << "too "
                              << (cond ? "small" : "big")
                              << " number: "
                              << argv[i]
                              << '('
                              << (cond ? "minimum" : "maximum")
                              << " available number is "
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
                    std::cerr << argv[0] << ": too many arguments";
                    clean();
                    return(1);
                } else {
                    std::cout << (static_cast<double>(nums[0])/static_cast<double>(nums[1])) << '\n' << std::fmod(static_cast<double>(nums[0]), static_cast<double>(nums[1])) << std::endl;
                }
            } else if(argveq("**")) {
                goto power;
            } else {
                switch(*(argv[1])) {   //  nintendo switch
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
                                std::cerr << "division by zero!" << std::endl;
                                clean();
                                return(1);
                            }
                        }
                        break;
                    }
                    case '%': {
                        if(argc > 4) {
                            std::cerr << argv[0] << ": too many arguments";
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
                            std::cerr << argv[0] << ": too many arguments";
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
            std::cerr << argv[0] << ": should be as 1st argument: '+', '-', '/', '%', '**', '^', '/%', '//', or '*'. Write \"" << argv[0] << " --help\" for get some information about available operators" << std::endl;
            return(1);
        }
    };

    return(0);
}
