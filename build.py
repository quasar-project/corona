import argparse
import subprocess


def run_conan(build_type, __compiler):
    profile = 'conan/profiles/mingw_windows_x86_64' if __compiler == 'gcc' else 'conan/profiles/msvc_windows_x86_64'
    subprocess.run(
        (
            'conan', 'install', '.',
            '--profile', profile,
            '--build', 'missing',
            '--output-folder=./dependencies',
            f'--settings=build_type={build_type}'
        )
    )


# def run_cmake():
#     subprocess.run(
#         (
#             'cmake', '..',
#         )
#     )


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--compiler', nargs='?', help='Compiler type: gcc or msvc', type=str)
    parser.add_argument('--cmake-prefix-path', nargs='?', help='Prefix path for CMake', type=str)
    args = parser.parse_args()

    compiler = str()
    if args.compiler == 'mingw' or args.compiler == 'gcc' or len(args.compiler) == 0:
        print('Using GCC toolchain')
        compiler = 'gcc'
    elif args.compiler == 'msvc':
        print('Using MSVC toolchain')
        compiler = 'msvc'
    else:
        print(f'Unknown compiler: {args.compiler}')
        exit(1)

    prefix_path = str()
    if len(args.cmake_prefix_path) > 0:
        prefix_path = args.cmake_prefix_path
    else:
        print('Using default CMake prefix path')
        prefix_path = 'C:/Qt/6.7.0/mingw_64'
    print(f'Using CMake prefix path: {prefix_path}')

    run_conan('Debug', compiler)
    run_conan('Release', compiler)


