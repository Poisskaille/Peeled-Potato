import os
import platform
import sys
import zipfile
from pathlib import Path


def get_platform_info():
    system = platform.system().lower()
    machine = platform.machine().lower()

    if system == "darwin":
        os_name = "macosx"
        plat_dir = "Mac"
        exe_ext = ""
        lib_ext = ".dylib"
        lib_prefix = "lib"
    elif system == "windows":
        os_name = "windows"
        plat_dir = "Win32"
        exe_ext = ".exe"
        lib_ext = ".dll"
        lib_prefix = ""
    elif system == "linux":
        os_name = "linux"
        plat_dir = "Linux"
        exe_ext = ""
        lib_ext = ".so"
        lib_prefix = "lib"
    else:
        raise Exception(f"Unsupported system: {system}")

    if machine in ["amd64", "x86_64"]:
        arch = "x64"
    elif machine in ["arm64", "aarch64"]:
        arch = "arm64"
    else:
        arch = machine

    # xmake arch mapping
    if system == "darwin" and arch == "arm64":
        xmake_arch = "arm64"
    elif system == "windows" and arch == "x64":
        xmake_arch = "x64"
    elif system == "linux" and arch == "x64":
        xmake_arch = "x86_64"
    elif system == "darwin" and arch == "x64":
        xmake_arch = "x86_64"
    else:
        xmake_arch = arch

    return os_name, xmake_arch, plat_dir, exe_ext, lib_prefix, lib_ext


def main():
    # Make sure we're in the project root
    root_dir = Path(__file__).resolve().parent.parent
    os.chdir(root_dir)

    os_name, xmake_arch, plat_dir, exe_ext, lib_prefix, lib_ext = get_platform_info()

    build_dir = Path(f"build/{os_name}/{xmake_arch}/release")
    if not build_dir.exists():
        print(
            f"Error: Build directory '{build_dir}' does not exist. Please run `xmake build -m release` first."
        )
        sys.exit(1)

    release_assets_dir = Path(f"ReleaseAssets/{plat_dir}")
    if not release_assets_dir.exists():
        print(f"Warning: ReleaseAssets/{plat_dir} directory not found.")

    zip_filename = f"Termina-Release-{plat_dir}-{xmake_arch}.zip"
    print(f"Creating release archive: {zip_filename}")

    # Explicitly requested binaries
    binaries = ["Editor", "Launcher", "Runtime"]

    # Also include the shared library since it is likely required to run the engine
    libs = [f"{lib_prefix}Termina{lib_ext}"]

    with zipfile.ZipFile(zip_filename, "w", zipfile.ZIP_DEFLATED) as zipf:
        # 1. Add Binaries
        for bin_name in binaries:
            bin_file = build_dir / f"{bin_name}{exe_ext}"
            if bin_file.exists():
                print(f" -> Adding binary: {bin_file.name}")
                zipf.write(bin_file, arcname=bin_file.name)
            else:
                print(f" -> Warning: Missing binary '{bin_file}'")

        # 2. Add Shared Libraries
        for lib_name in libs:
            lib_file = build_dir / lib_name
            if lib_file.exists():
                print(f" -> Adding library: {lib_file.name}")
                zipf.write(lib_file, arcname=lib_file.name)

        # 3. Add ReleaseAssets/(Platform)
        if release_assets_dir.exists():
            for item in release_assets_dir.rglob("*"):
                if item.is_file():
                    # Preserve relative folder structure inside the zip
                    arcname = item.relative_to(release_assets_dir)
                    print(f" -> Adding asset: {arcname}")
                    zipf.write(item, arcname=str(arcname))

    print(f"\nSuccessfully created {zip_filename} in the project root!")


if __name__ == "__main__":
    main()
