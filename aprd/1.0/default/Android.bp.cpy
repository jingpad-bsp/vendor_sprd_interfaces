cc_library_shared {
    // FIXME: this should only be -impl for a passthrough hal.
    // In most cases, to convert this to a binderized implementation, you should:
    // - change '-impl' to '-service' here and make it a cc_binary instead of a
    //   cc_library_shared.
    // - add a *.rc file for this module.
    // - delete HIDL_FETCH_I* functions.
    // - call configureRpcThreadpool and registerAsService on the instance.
    // You may also want to append '-impl/-service' with a specific identifier like
    // '-vendor' or '-<hardware identifier>' etc to distinguish it.
    name: "vendor.sprd.hardware.aprd@1.0-impl",
    relative_install_path: "hw",
    // FIXME: this should be 'vendor: true' for modules that will eventually be
    // on AOSP.
    proprietary: true,
    srcs: [
        "CPxInfo.cpp",
        "ModemExpMonitor.cpp",
        "AprdInfoSync.cpp",
    ],
    shared_libs: [
        "libhidlbase",
        "libhidltransport",
        "liblog",
        "libsysutils",
        "libdl",
        "libutils",
        "libcutils",
        "libhardware",
        "libatci",
        "libc++",
        "libxml2",
        "vendor.sprd.hardware.aprd@1.0",
    ],
}
cc_binary {
    proprietary: true,
    relative_install_path: "hw",
    name: "vendor.sprd.hardware.aprd@1.0-service",
    init_rc: ["vendor.sprd.hardware.aprd@1.0-service.rc"],
    srcs: [
         "service.cpp",
    ],

    cflags: [
        "-Wall",
        "-Werror",
    ],

    shared_libs: [
        "libhidlbase",
        "libhidltransport",
        "liblog",
        "libsysutils",
        "libdl",
        "libutils",
        "libcutils",
        "libhardware",
        "libatci",
        "libc++",
        "libxml2",
        "vendor.sprd.hardware.aprd@1.0",
    ],
}
