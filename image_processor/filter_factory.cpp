#include "filter_factory.h"

FilterFactory::FilterFactory(std::vector<MakeAllFilters::FilterDescriptor>& pipeline) : pipeline_(pipeline) {
}

void FilterFactory::RunApplication(Image& image) {
    std::unordered_map<MakeAllFilters::FilterTypes, FilterFactory::Factory> filters = {
        {MakeAllFilters::NegativeFilter, FilterPipelineFactory::MakeNegativeFilter},
        {MakeAllFilters::Edge, FilterPipelineFactory::MakeEdge},
        {MakeAllFilters::Crop, FilterPipelineFactory::MakeCrop},
        {MakeAllFilters::GrayScale, FilterPipelineFactory::MakeGrayScale},
        {MakeAllFilters::Sharp, FilterPipelineFactory::MakeSharp},
        {MakeAllFilters::Gauss, FilterPipelineFactory::MakeGauss},
        {MakeAllFilters::Twirl, FilterPipelineFactory::MakeTwirl}};

    pair_filter = filters;

    for (const auto& desc : pipeline_) {
        std::shared_ptr<BaseFilter> filter = CreateFilter(desc);
        filter->Apply(image);
    }
}

std::shared_ptr<BaseFilter> FilterFactory::CreateFilter(const MakeAllFilters::FilterDescriptor& descriptor) const {
    Factory func = pair_filter.at(descriptor.filter_);
    std::shared_ptr<BaseFilter> current_filter = func(descriptor);
    return current_filter;
}

namespace FilterPipelineFactory {
std::shared_ptr<BaseFilter> MakeNegativeFilter(const MakeAllFilters::FilterDescriptor& desc) {
    if (!desc.filter_params.empty()) {
        throw std::invalid_argument("mistake in NegativeFilter");
    }
    return std::shared_ptr<BaseFilter>(new NegativeFilter);
}

std::shared_ptr<BaseFilter> MakeCrop(const MakeAllFilters::FilterDescriptor& desc) {
    if (desc.filter_params.size() != 2) {
        throw std::invalid_argument("mistake in Crop");
    }

    return std::shared_ptr<BaseFilter>(new Crop(std::stoi(desc.filter_params[0]), std::stoi(desc.filter_params[1])));
}

std::shared_ptr<BaseFilter> MakeGrayScale(const MakeAllFilters::FilterDescriptor& desc) {
    if (!desc.filter_params.empty()) {
        throw std::invalid_argument("mistake in GrayScale");
    }
    return std::shared_ptr<BaseFilter>(new GrayScale);
}

std::shared_ptr<BaseFilter> MakeEdge(const MakeAllFilters::FilterDescriptor& desc) {
    if (desc.filter_params.empty()) {
        throw std::invalid_argument("mistake in Edge");
    }
    return std::shared_ptr<BaseFilter>(new Edge( std::stoi(desc.filter_params[0])));
}

std::shared_ptr<BaseFilter> MakeSharp(const MakeAllFilters::FilterDescriptor& desc) {
    if (!desc.filter_params.empty()) {
        throw std::invalid_argument("mistake in Sharp");
    }
    return std::shared_ptr<BaseFilter>(new Sharp);
}

std::shared_ptr<BaseFilter> MakeGauss(const MakeAllFilters::FilterDescriptor& desc) {
    if (desc.filter_params.empty()) {
        throw std::invalid_argument("mistake in Gauss");
    }
    return std::shared_ptr<BaseFilter>(new Gauss(std::stoi(desc.filter_params[0])));
}

std::shared_ptr<BaseFilter> MakeTwirl(const MakeAllFilters::FilterDescriptor& desc) {
    if (!desc.filter_params.empty()) {
        throw std::invalid_argument("mistake in Twirl");
    }
    return std::shared_ptr<BaseFilter>(new Twirl);
}
}  // namespace FilterPipelineFactory
