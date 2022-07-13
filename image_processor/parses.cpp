#include "parses.h"

parsing::Parser::Parser(int argc, char *argv[]) {
    if (argc < arg_min) {
        throw MyException("few arguments");
    }
    input_path_ = argv[input_index];
    output_path_ = argv[output_index];
    MakeAllFilters::FilterDescriptor filter;
    bool flag = true;
    filter = MakeAllFilters::FilterDescriptor{};
    for (int i = 3; i < argc; ++i) {
        if (argv[i][0] == '-') {
            if (!flag) {
                descriptor_.push_back(filter);
                filter = MakeAllFilters::FilterDescriptor{};
            }
            filter.filter_ = parser_name_.at(argv[i]);
            flag = false;
        } else {
            filter.filter_params.push_back(argv[i]);
        }
    }
    descriptor_.push_back(filter);
}
