// to make this file: g++ -std=c++11 -g calc.cpp -o calc

#include <algorithm>
#include <iostream>
#include <string>
#include <limits>
#include <stdexcept>

/*
 * 
 */

using calc_int_t = int;

/*
 * 
 */
namespace impl {

class scanner {
 public:
    scanner();

    void set_expression(std::string);
    size_t get_position() const;
    bool is_eos() const;
    int peek_char() const;
    char read_char();
    calc_int_t read_number();
 private:
    std::string make_err_message(std::string message);
    std::string expression_;
    size_t position_;
};

/*
 * 
 */
class parser {
 public:
    parser();

    calc_int_t evaluate(std::string expression);

 private:
    calc_int_t parse_expression();
    calc_int_t parse_term();
    calc_int_t parse_factor();
    calc_int_t parse_number();

    std::string make_err_message(std::string message);
    scanner scanner_;
};

}  // namespace impl

/*
 *
 */
class calculator {
 public:
    calculator();
    calc_int_t evaluate(std::string expression);
 private:
    impl::parser parser_;
};



#ifdef UNIT_TESTS

#include "gtest/gtest.h"

/*
 * 
 */
TEST(CalcTest, Number) {
    calculator a_calculator;
    EXPECT_EQ(a_calculator.evaluate("0"), 0);
    EXPECT_EQ(a_calculator.evaluate("-0"), 0);
    EXPECT_EQ(a_calculator.evaluate("1"), 1);
    EXPECT_EQ(a_calculator.evaluate("-1"), -1);
    EXPECT_EQ(a_calculator.evaluate("1234"), 1234);
    EXPECT_EQ(a_calculator.evaluate("-1234"), -1234);
    EXPECT_EQ(a_calculator.evaluate("123456789"), 123456789);
    EXPECT_EQ(a_calculator.evaluate("-123456789"), -123456789);
}

/*
 * 
 */
TEST(CalcTest, OutOfRangeNumber) {
    calculator a_calculator;
    EXPECT_THROW(a_calculator.evaluate("3000000000"), std::runtime_error);
    EXPECT_THROW(a_calculator.evaluate("-3000000000"), std::runtime_error);
}

/*
 * 
 */
TEST(CalcTest, Factor) {
    calculator a_calculator;
    EXPECT_EQ(a_calculator.evaluate("(0)"), 0);
    EXPECT_EQ(a_calculator.evaluate("(1)"), 1);
    EXPECT_EQ(a_calculator.evaluate("(-1)"), -1);
    EXPECT_EQ(a_calculator.evaluate("(1234)"), 1234);
    EXPECT_EQ(a_calculator.evaluate("(-1234)"), -1234);
    EXPECT_EQ(a_calculator.evaluate("(123456789)"), 123456789);
    EXPECT_EQ(a_calculator.evaluate("(-123456789)"), -123456789);
}

/*
 * 
 */
TEST(CalcTest, SumNumber) {
    calculator a_calculator;
    EXPECT_EQ(a_calculator.evaluate("0+0"), 0);
    EXPECT_EQ(a_calculator.evaluate("1+2"), 3);
    EXPECT_EQ(a_calculator.evaluate("1000+2000"), 3000);
    EXPECT_EQ(a_calculator.evaluate("(1000000000+1000000001)"), 2000000001);
    EXPECT_EQ(a_calculator.evaluate("-1+(-1)"), -2);
    EXPECT_EQ(a_calculator.evaluate("-1+(-2)"), -3);
    EXPECT_EQ(a_calculator.evaluate("-1000+(-2000)"), -3000);
    EXPECT_EQ(a_calculator.evaluate("(1+2)+3"), 6);
    EXPECT_EQ(a_calculator.evaluate("(((1+2)+3)+4)"), 10);
    EXPECT_EQ(a_calculator.evaluate("(-1+(-3))+(-4)"), -8);
}

/*
 * 
 */
TEST(CalcTest, SumNumberOverflow) {
    calculator a_calculator;
    EXPECT_THROW(a_calculator.evaluate("2000000000+2000000000"), std::runtime_error);
}

/*
 * 
 */
TEST(CalcTest, DiffNumber) {
    calculator a_calculator;
    EXPECT_EQ(a_calculator.evaluate("0-0"), 0);
    EXPECT_EQ(a_calculator.evaluate("3-2"), 1);
    EXPECT_EQ(a_calculator.evaluate("-3-2"), -5);
    EXPECT_EQ(a_calculator.evaluate("(10-5)-3"), 2);
    EXPECT_EQ(a_calculator.evaluate("1000000000-1000000001"), -1);
    EXPECT_EQ(a_calculator.evaluate("-1000000000-1000000001"), -2000000001);
    EXPECT_EQ(a_calculator.evaluate("(-3-2)-4"), -9);
}

/*
 * 
 */
TEST(CalcTest, DiffNumberOverflow) {
    calculator a_calculator;
    EXPECT_THROW(a_calculator.evaluate("-2000000000-2000000000"), std::runtime_error);
    EXPECT_THROW(a_calculator.evaluate("0-3000000000"), std::runtime_error);
}

/*
 * 
 */
TEST(CalcTest, MultiplyNumber) {
    calculator a_calculator;
    EXPECT_EQ(a_calculator.evaluate("0*0"), 0);
    EXPECT_EQ(a_calculator.evaluate("1*2"), 2);
    EXPECT_EQ(a_calculator.evaluate("100*200"), 20000);
    EXPECT_EQ(a_calculator.evaluate("1*0"), 0);
    EXPECT_EQ(a_calculator.evaluate("-1*2"), -2);
    EXPECT_EQ(a_calculator.evaluate("-1*-2"), 2);
    EXPECT_EQ(a_calculator.evaluate("-100*200"), -20000);
}


/*
 * 
 */
TEST(CalcTest, DivideNumber) {
    calculator a_calculator;
    EXPECT_EQ(a_calculator.evaluate("4/2"), 2);
    EXPECT_EQ(a_calculator.evaluate("8/3"), 2);
    EXPECT_EQ(a_calculator.evaluate("-8/3"), -3);
}


/*
 * 
 */
TEST(CalcTest, ModuloNumber) {
    calculator a_calculator;
    EXPECT_EQ(a_calculator.evaluate("0%2"), 0);
    EXPECT_EQ(a_calculator.evaluate("8%2"), 0);
    EXPECT_EQ(a_calculator.evaluate("9%2"), 1);
    EXPECT_EQ(a_calculator.evaluate("10%2"),0);
    EXPECT_EQ(a_calculator.evaluate("8%3"), 2);
    EXPECT_EQ(a_calculator.evaluate("-8%3"), 1);
}

/*
 * 
 */
TEST(CalcTest, InvalidExpression) {
    calculator a_calculator;
    EXPECT_THROW(a_calculator.evaluate("1+2+3"), std::runtime_error);
    EXPECT_THROW(a_calculator.evaluate("1 +2"), std::runtime_error);
    EXPECT_THROW(a_calculator.evaluate("1+ 2"), std::runtime_error);
    EXPECT_THROW(a_calculator.evaluate("( 1)"), std::runtime_error);
    EXPECT_THROW(a_calculator.evaluate("(1 )"), std::runtime_error);
}

/*
 * 
 */
TEST(CalcTest, AssignentExpressions) {
    calculator a_calculator;
    EXPECT_EQ(a_calculator.evaluate("(1+3)*(9/3)"), 12);
    EXPECT_EQ(a_calculator.evaluate("0-123"), -123);
}

#endif

/*
 *
 */
#ifdef UNIT_TESTS

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

#else

int main() {
    calculator a_calculator;
    do {
        std::string expression;
        std::getline(std::cin, expression);
        if (std::cin.eof()) {
            break;
        }
        try {
            auto res = a_calculator.evaluate(std::move(expression));
            std::cout << res << std::endl;
        } catch (std::runtime_error& e) {
            std::cerr << e.what() << std::endl;
        }
    } while (true);

    return 0;
}

#endif


/*
 *
 */

namespace impl {

/*
 *
 */
scanner::scanner() : position_(0) {
}

/*
 *
 */
std::string scanner::make_err_message(std::string message) {
    std::string res = "position: " + std::to_string(position_)
                      + ", error: " + std::move(message);
    return std::move(res);
}


/*
 *
 */
void scanner::set_expression(std::string expression) {
    expression_ = std::move(expression);
    position_ = 0;
}

/*
 *
 */
bool scanner::is_eos() const {
    return position_ >= expression_.size();
}

/*
 *
 */
int scanner::peek_char() const {
    if (position_ < expression_.size()) {
        return expression_[position_];
    } else {
        return -1;
    }
}

char scanner::read_char() {
    if (position_ < expression_.size()) {
        return expression_[position_++];
    } else {
        throw std::runtime_error(
                make_err_message("no more chars in expression"));
    }
}

/*
 *
 */
calc_int_t scanner::read_number() {
    std::string number;
    auto curr_position = position_;
    while (curr_position < expression_.size() &&
            std::isdigit(expression_[curr_position])) {
        number += expression_[curr_position++];
    }
    if (number.empty()) {
        throw std::runtime_error(
                make_err_message("not a number"));
    }
    try {
        auto res = std::stoi(number);
        position_ = curr_position;
        return res;
    } catch (std::invalid_argument& e) {
        throw std::runtime_error(make_err_message("invalid number"));
    } catch (std::out_of_range& e) {
        throw std::runtime_error(make_err_message("out of range"));
    } catch (std::exception& e) {
        throw std::runtime_error(
            make_err_message("failed to convert to integer"));
    }
}

/*
 *
 */
size_t scanner::get_position() const {
    return position_;
}

/*
 *
 */
parser::parser() : scanner_() {
}

/*
 *
 */
std::string parser::make_err_message(std::string message) {
    std::string res = "position: " + std::to_string(scanner_.get_position()) +
                      + ", error: " + std::move(message);
    return std::move(res);
}

/*
 *
 */
calc_int_t parser::evaluate(std::string expression) {
    scanner_.set_expression(std::move(expression));
    auto res = parse_expression();
    if (!scanner_.is_eos()) {
        throw std::runtime_error(make_err_message("invalid expression"));
    }
    return res;
}

static bool check_add_overflow(calc_int_t lhs, calc_int_t rhs) {
    if (lhs >= 0) {
        if (std::numeric_limits<calc_int_t>::max() - lhs < rhs) {
            return false;
        }
    } else {
        if (rhs < std::numeric_limits<calc_int_t>::lowest() - lhs) {
            return false;
        }
    }
    return true;
}

static bool check_diff_overflow(calc_int_t lhs, calc_int_t rhs) {
    if (lhs < 0) {
        calc_int_t diff = lhs - std::numeric_limits<calc_int_t>::lowest();
        if (diff < rhs) {
            return false;
        }
    }
    return true;
}

/*
 *
 */
calc_int_t parser::parse_expression() {
    if (scanner_.is_eos()) {
        throw std::runtime_error(make_err_message("empty expression"));
    }

    int op1 = parse_term();
    if (scanner_.is_eos()) {
        return op1;
    }

    auto next_char = scanner_.peek_char();
    if (next_char != '+' && next_char != '-') {
        return op1;
    }
    scanner_.read_char();

    int op2 = parse_term();
    switch (next_char) {
        case '+':
            if (check_add_overflow(op1, op2))
                return op1 + op2;
            throw std::runtime_error(
                make_err_message("adding two numbers results in overflow"));
        case '-':
            if (check_diff_overflow(op1, op2))
                return op1 - op2;
            throw std::runtime_error(
                make_err_message(
                    "substraction two numbers results in overflow"));
        default:
            throw std::runtime_error(
                make_err_message("an operation is not supported for terms"));
    }
}

static bool check_multiply_overflow(calc_int_t lhs, calc_int_t rhs) {
    calc_int_t x = lhs * rhs;
    if (rhs != 0 && x / rhs != lhs) {
        return false;
    }
    return true;
}

/*
 *
 */
calc_int_t parser::parse_term() {
    if (scanner_.is_eos()) {
        throw std::runtime_error(make_err_message("empty term"));
    }

    auto op1 = parse_factor();
    if (scanner_.is_eos()) {
        return op1;
    }

    int next_char = scanner_.peek_char();
    if (next_char != '*' && next_char != '/' && next_char != '%') {
        return op1;
    }
    scanner_.read_char();

    auto op2 = parse_factor();
    switch (next_char) {
        case '*':
            if (check_multiply_overflow(op1, op2)) {
                return op1 * op2;
            }
            throw std::runtime_error(make_err_message(
                "multiplying two numbers results in overflow"));
        case '/':
            if (op2==0)
                throw std::runtime_error(make_err_message("division by zero"));
            return std::floor((double)op1/op2);
        case '%':
            if (op2==0)
                throw std::runtime_error(make_err_message("division by zero"));
            if (op1 >= 0) {
                return op1 % op2;
            } else if (op1 < 0) {
                return op2 + (op1 % op2);
            }
        default:
            throw std::runtime_error(
                make_err_message("an operation is not supported for factors"));
    }
}
/*
 *
 */
calc_int_t parser::parse_factor() {
    if (scanner_.is_eos()) {
        throw std::runtime_error(make_err_message("empty factor"));
    }

    int next_char = scanner_.peek_char();
    if (next_char == '(') {
        scanner_.read_char();

        auto res = parse_expression();
        int next_char = scanner_.peek_char();
        if (next_char != ')') {
            throw std::runtime_error(
                    make_err_message("there is no right round bracket"));
        }
        scanner_.read_char();
        return res;
    } else if (next_char == '-') {
        scanner_.read_char();
        auto res = parse_factor();
        // TODO(sk): check boundary
        return -res;
    } else {
        auto res = parse_number();
        return res;
    }
}

/*
 *
 */
calc_int_t parser::parse_number() {
    if (scanner_.is_eos()) {
        throw std::runtime_error(make_err_message("empty number"));
    }
    auto res = scanner_.read_number();
    return res;
}

}  // namespace impl


/*
 *
 */
calculator::calculator() {
}

/*
 *
 */
calc_int_t calculator::evaluate(std::string expression) {
    return parser_.evaluate(std::move(expression));
}
