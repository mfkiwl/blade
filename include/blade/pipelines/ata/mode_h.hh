#ifndef BLADE_PIPELINES_ATA_MODE_H_HH
#define BLADE_PIPELINES_ATA_MODE_H_HH

#include <memory>
#include <deque>

#include "blade/pipeline.hh"

#include "blade/modules/channelizer.hh"
#include "blade/modules/detector.hh"

namespace Blade::Pipelines::ATA {

template<typename OT = F32>
class BLADE_API ModeH : public Pipeline {
 public:
    struct Config {
        U64 accumulateRate;

        U64 channelizerNumberOfBeams;
        U64 channelizerNumberOfFrequencyChannels;
        U64 channelizerNumberOfTimeSamples;
        U64 channelizerNumberOfPolarizations;

        U64 detectorNumberOfOutputPolarizations;

        U64 channelizerBlockSize = 512;
        U64 detectorBlockSize = 512;
    };

    explicit ModeH(const Config& config);

    constexpr const U64 getInputSize() const {
        return channelizer->getBufferSize();
    }

    constexpr const U64 getOutputSize() const {
        return detector->getOutputSize();
    }

    constexpr Vector<Device::CUDA, CF32>& getInput() const {
        return channelizer->getInput();
    }

    Result run(Vector<Device::CPU, OT>& output);

 private:
    const Config config;

    Vector<Device::CUDA, CF32> input;

    std::shared_ptr<Modules::Channelizer<CF32, CF32>> channelizer;
    std::shared_ptr<Modules::Detector<CF32, F32>> detector;

    constexpr const Vector<Device::CUDA, OT>& getOutput() {
        return detector->getOutput();
    }
};

}  // namespace Blade::Pipelines::ATA

#endif
