#pragma once
#include <string_view>
#include <vector>
#include <string>
#include <unordered_map>
#include "filters.h"

struct MyException : std::runtime_error {
    explicit MyException(const std::string& message) : std::runtime_error(message.c_str()){};
};

namespace parsing {
class Parser {
public:
    Parser(int argc, char* argv[]);
    const char* InputFileGetter() const {
        return input_path_;
    }
    const char* OutputFileGetter() const {
        return output_path_;
    }
    std::vector<MakeAllFilters::FilterDescriptor> descriptor_;

private:
    enum ParserConstants {
        arg_min = 3,
        input_index = 1,
        output_index = 2,
    };

protected:
    const char* input_path_;
    const char* output_path_;

    const std::unordered_map<std::string, MakeAllFilters::FilterTypes> parser_name_ = {
        {"-crop", MakeAllFilters::Crop},   {"-gs", MakeAllFilters::GrayScale}, {"-neg", MakeAllFilters::NegativeFilter},
        {"-sharp", MakeAllFilters::Sharp}, {"-edge", MakeAllFilters::Edge},    {"-gauss", MakeAllFilters::Gauss},
        {"-twrl", MakeAllFilters::Twirl}};
};
}  // namespace parsing
