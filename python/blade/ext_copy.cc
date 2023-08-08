#include <nanobind/nanobind.h>
#include <nanobind/ndarray.h>
#include <nanobind/stl/array.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/shared_ptr.h>

#include "blade/base.hh"
#include "blade/modules/base.hh"
#include "blade/memory/custom.hh"

namespace nb = nanobind;
using namespace nb::literals;
using namespace Blade;

template<typename IT, typename OT>
void NB_SUBMODULE(auto& m, const auto& name) {
    using Class = Modules::Copy<IT, OT>;

    nb::class_<Class> mod(m, name);

    nb::class_<typename Class::Config>(mod, "config");

    nb::class_<typename Class::Input>(mod, "input")
        .def(nb::init<const ArrayTensor<Device::CUDA, IT>&>(), "buf"_a);

    mod
        .def(nb::init<const typename Class::Config&, const typename Class::Input&>())
        .def("process", [](Class& instance, const U64& counter) {
            return instance.process(counter);
        })
        .def("get_config", &Class::getConfig, nb::rv_policy::reference)
        .def("get_input", &Class::getInputBuffer, nb::rv_policy::reference)
        .def("get_output", &Class::getOutputBuffer, nb::rv_policy::reference);
}

NB_MODULE(_blade_copy_impl, m) {
    NB_SUBMODULE<CF32, CF32>(m, "to_cf32");
    NB_SUBMODULE<CF16, CF16>(m, "to_cf16");
    NB_SUBMODULE<CI8, CI8>(m, "to_ci8");
}