#pragma once

#include "filters.h"
#include "parses.h"
struct FilterFactory {

    using Factory = std::shared_ptr<BaseFilter> (*)(const MakeAllFilters::FilterDescriptor&);
    using FilterMap = std::unordered_map<MakeAllFilters::FilterTypes, Factory>;

    FilterFactory(std::vector<MakeAllFilters::FilterDescriptor>& filter_pipeline);

    void RunApplication(Image& image);

    std::shared_ptr<BaseFilter> CreateFilter(const MakeAllFilters::FilterDescriptor& descriptor) const;

protected:
    std::vector<MakeAllFilters::FilterDescriptor> pipeline_;
    FilterMap pair_filter;
};

namespace FilterPipelineFactory {
std::shared_ptr<BaseFilter> MakeNegativeFilter(const MakeAllFilters::FilterDescriptor& desc);
std::shared_ptr<BaseFilter> MakeSharp(const MakeAllFilters::FilterDescriptor& desc);
std::shared_ptr<BaseFilter> MakeCrop(const MakeAllFilters::FilterDescriptor& desc);
std::shared_ptr<BaseFilter> MakeGrayScale(const MakeAllFilters::FilterDescriptor& desc);
std::shared_ptr<BaseFilter> MakeEdge(const MakeAllFilters::FilterDescriptor& desc);
std::shared_ptr<BaseFilter> MakeGauss(const MakeAllFilters::FilterDescriptor& desc);
std::shared_ptr<BaseFilter> MakeTwirl(const MakeAllFilters::FilterDescriptor& desc);
}  // namespace FilterPipelineFactory
