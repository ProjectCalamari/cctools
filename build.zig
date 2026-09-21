const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});
    const linkage = b.option(std.builtin.LinkMode, "linkage", "Whether to link dynamically or statically") orelse @as(std.builtin.LinkMode, .static);
    const xnu_src = b.option(std.Build.LazyPath, "xnu-src", "Path to the XNU source tree") orelse b.path("../xnu");

    const common_flags = &.{
        "-std=gnu11",
        "-D__private_extern__=",
        "-D__DARWIN_UNIX03=1",
        "-D_DARWIN_C_SOURCE=1",
        "-DCPU_TYPE_RISCV32=19",
        "-DCURRENT_PROJECT_VERSION=\"cctools-1035.1.102\"",
        "-Wno-deprecated-non-prototype",
        "-Wno-implicit-function-declaration",
        "-Wno-format",
        "-Wno-enum-compare",
        "-Wno-macro-redefined",
    };

    const macho_mod = nativeModule(b, target, optimize, xnu_src);
    macho_mod.addCSourceFiles(.{
        .files = &.{
            "libmacho/arch.c",
            "libmacho/i386_swap.c",
            "libmacho/slot_name.c",
            "libmacho/swap.c",
        },
        .flags = common_flags,
    });
    const macho = b.addLibrary(.{ .name = "macho", .root_module = macho_mod, .linkage = linkage });
    b.installArtifact(macho);

    const stuff_mod = nativeModule(b, target, optimize, xnu_src);
    if (target.result.os.tag != .macos) {
        stuff_mod.addCSourceFile(.{ .file = b.path("compat/port.c"), .flags = common_flags });
    }
    stuff_mod.addCSourceFiles(.{
        .files = &.{
            "libstuff/allocate.c",
            "libstuff/apple_version.c",
            "libstuff/arch.c",
            "libstuff/arch_usage.c",
            "libstuff/best_arch.c",
            "libstuff/breakout.c",
            "libstuff/bytesex.c",
            "libstuff/checkout.c",
            "libstuff/coff_bytesex.c",
            "libstuff/crc32.c",
            "libstuff/diagnostics.c",
            "libstuff/dylib_roots.c",
            "libstuff/dylib_table.c",
            "libstuff/errors.c",
            "libstuff/execute.c",
            "libstuff/fatal_arch.c",
            "libstuff/fatals.c",
            "libstuff/get_arch_from_host.c",
            "libstuff/get_toc_byte_sex.c",
            "libstuff/guess_short_name.c",
            "libstuff/hash_string.c",
            "libstuff/hppa.c",
            "libstuff/lto.c",
            "libstuff/llvm.c",
            "libstuff/macosx_deployment_target.c",
            "libstuff/ofile.c",
            "libstuff/ofile_error.c",
            "libstuff/ofile_get_word.c",
            "libstuff/print.c",
            "libstuff/reloc.c",
            "libstuff/rnd.c",
            "libstuff/seg_addr_table.c",
            "libstuff/set_arch_flag_name.c",
            "libstuff/swap_headers.c",
            "libstuff/symbol_list.c",
            "libstuff/SymLoc.c",
            "libstuff/unix_standard_mode.c",
            "libstuff/version_number.c",
            "libstuff/vm_flush_cache.c",
            "libstuff/write64.c",
            "libstuff/writeout.c",
            "libstuff/args.c",
            "libstuff/align.c",
            "libstuff/depinfo.c",
            "libstuff/xcode.c",
        },
        .flags = common_flags,
    });
    const stuff = b.addLibrary(.{
        .name = "stuff",
        .root_module = stuff_mod,
        .linkage = linkage,
    });
    stuff.root_module.linkLibrary(macho);
    b.installArtifact(stuff);

    const tools = [_]Tool{
        .{ .name = "lipo", .source = "misc/lipo.c" },
        .{ .name = "strings", .source = "misc/strings.c" },
        .{ .name = "size", .source = "misc/size.c" },
        .{ .name = "nm", .source = "misc/nm.c" },
        .{ .name = "libtool", .source = "misc/libtool.c" },
        .{ .name = "ranlib", .source = "misc/libtool.c", .define = "RANLIB" },
        .{ .name = "strip", .source = "misc/strip.c" },
        .{ .name = "nmedit", .source = "misc/strip.c", .define = "NMEDIT" },
        .{ .name = "segedit", .source = "misc/segedit.c" },
        .{ .name = "install_name_tool", .source = "misc/install_name_tool.c" },
        .{ .name = "codesign_allocate", .source = "misc/codesign_allocate.c" },
        .{ .name = "ctf_insert", .source = "misc/ctf_insert.c" },
        .{ .name = "bitcode_strip", .source = "misc/bitcode_strip.c", .define = "ALLOW_ARCHIVES" },
        .{ .name = "vtool", .source = "misc/vtool.c" },
    };
    for (tools) |tool| {
        const mod = nativeModule(b, target, optimize, xnu_src);
        if (tool.define) |define| mod.addCMacro(define, "1");
        mod.addCSourceFiles(.{ .files = &.{tool.source}, .flags = common_flags });
        mod.linkLibrary(stuff);
        mod.linkLibrary(macho);
        const exe = b.addExecutable(.{ .name = tool.name, .root_module = mod });
        b.installArtifact(exe);
    }

    const ar_mod = nativeModule(b, target, optimize, xnu_src);
    // ar carries BSD-local functions with names also exported by libstuff.
    ar_mod.addCMacro("error", "ar_error");
    ar_mod.addCMacro("print", "ar_print");
    ar_mod.addCSourceFiles(.{
        .files = &.{
            "ar/append.c",
            "ar/ar.c",
            "ar/archive.c",
            "ar/contents.c",
            "ar/delete.c",
            "ar/extract.c",
            "ar/misc.c",
            "ar/move.c",
            "ar/print.c",
            "ar/replace.c",
        },
        .flags = common_flags,
    });
    ar_mod.linkLibrary(stuff);
    ar_mod.linkLibrary(macho);
    b.installArtifact(b.addExecutable(.{ .name = "ar", .root_module = ar_mod }));

    const otool_mod = nativeModule(b, target, optimize, xnu_src);
    otool_mod.link_libcpp = true;
    otool_mod.addCSourceFiles(.{
        .files = &.{
            "otool/arm_disasm.c",
            "otool/arm64_disasm.c",
            "otool/coff_print.c",
            "otool/dyld_bind_info.c",
            "otool/i386_disasm.c",
            "otool/main.c",
            "otool/ofile_print.c",
            "otool/print_bitcode.c",
            "otool/print_objc.c",
            "otool/print_objc2_32bit.c",
            "otool/print_objc2_64bit.c",
            "otool/print_objc2_util.c",
        },
        .flags = common_flags,
    });
    otool_mod.linkLibrary(stuff);
    otool_mod.linkLibrary(macho);
    b.installArtifact(b.addExecutable(.{ .name = "otool", .root_module = otool_mod }));
}

const Tool = struct {
    name: []const u8,
    source: []const u8,
    define: ?[]const u8 = null,
};

fn nativeModule(
    b: *std.Build,
    target: std.Build.ResolvedTarget,
    optimize: std.builtin.OptimizeMode,
    xnu_src: std.Build.LazyPath,
) *std.Build.Module {
    const mod = b.createModule(.{
        .target = target,
        .optimize = optimize,
        .link_libc = true,
    });
    mod.addIncludePath(b.path("compat/common/include"));
    // On Darwin, use Zig's SDK headers and the platform implementations.  The
    // compatibility layer is for non-Darwin hosts and intentionally contains
    // headers with names such as Availability.h and mach/mach.h.
    if (target.result.os.tag != .macos) {
        mod.addIncludePath(b.path("compat/include"));
    } else {
        mod.addIncludePath(b.path("compat/darwin/include"));
    }
    mod.addIncludePath(b.path("include"));
    mod.addIncludePath(b.path("include/stuff"));
    mod.addIncludePath(b.path("libstuff"));
    mod.addSystemIncludePath(xnu_src.path(b, "EXTERNAL_HEADERS"));
    return mod;
}
